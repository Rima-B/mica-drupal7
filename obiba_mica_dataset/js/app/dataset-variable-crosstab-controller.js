/*
 * Copyright (c) 2015 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

'use strict';

(function ($) {
  Drupal.behaviors.obiba_mica_dataset_variable_crosstab_controller = {
    attach: function (context, settings) {
      mica.DatasetVariableCrosstab
        .controller('DatasetVariableCrosstabController', ['$rootScope',
          '$scope',
          '$routeParams',
          '$log',
          '$location',
          '$route',
          'DatasetResource',
          'DatasetCategoricalVariablesResource',
          'DatasetVariablesResource',
          'DatasetVariableResource',
          'DatasetVariablesCrosstabResource',
          'ServerErrorAlertService',
          'ContingencyService',
          'LocalizedStringService',
          'ChiSquaredCalculator',

          function ($rootScope, $scope, $routeParams, $log, $location, $route,
                    DatasetResource,
                    DatasetCategoricalVariablesResource,
                    DatasetVariablesResource,
                    DatasetVariableResource,
                    DatasetVariablesCrosstabResource,
                    ServerErrorAlertService,
                    ContingencyService,
                    LocalizedStringService,
                    ChiSquaredCalculator) {

            startProgess();

            function updateLocation(currentPath, param) {
              var original = $location.path;
              $location.path = function (path, reload) {
                if (reload === false) {
                  var lastRoute = $route.current;
                  var un = $rootScope.$on('$locationChangeSuccess', function () {
                    $route.current = lastRoute;
                    un();
                  });
                }
                return original.apply($location, [path]);
              };
              $location.path(currentPath + '/' + param, false);
              $location.path = original;
            }

            function startProgess() {
              $.ObibaProgressBarController().start();
              $.ObibaProgressBarController().setPercentage(65, true);
            }

            function endProgress() {
              $.ObibaProgressBarController().finish();
            }

            var onError = function (response) {
              $scope.serverError = true;
              ServerErrorAlertService.alert('DataAccessRequestEditController', response);
              ForbiddenDrupalRedirect.redirectDrupalMessage(response);
              endProgress();
            };

            var searchCategoricalVariables = function(queryString) {
              if (!queryString) return;
              DatasetCategoricalVariablesResource.get({
                  dsType: $routeParams.type,
                  dsId: $routeParams.ds,
                  query: queryString
                },
                function onSuccess(response) {
                  $scope.crosstab.lhs.variables = response.variables;
                },
                onError
              );
            };

            var searchVariables = function(queryString) {
              if (!queryString) return;
              DatasetVariablesResource.get({dsType: $routeParams.type, dsId: $routeParams.ds, query: queryString},
                function onSuccess(response) {
                  $scope.crosstab.rhs.variables = response.variables;
                },
                onError
              );
            };

            var canExchangeVariables = function(){
              return $scope.crosstab.lhs.variable
                && $scope.crosstab.rhs.variable
                && !isStatistical($scope.crosstab.rhs.variable);
            };

            var exchangeVariables = function() {
              if (canExchangeVariables()) {
                var temp = $scope.crosstab.lhs.variable;
                $scope.crosstab.lhs.variable = $scope.crosstab.rhs.variable;
                $scope.crosstab.rhs.variable = temp;
                submit();
              }
            };

            var clear = function() {
              initCrosstab();
            };

            var isStatistical = function(variable) {
              return variable && variable.nature.toLowerCase() === 'continuous';
            };

            function normalizeStatistics(contingency, v1Cats) {
              function createEmptyStatistics() {
                return {
                  min: '-',
                  max: '-',
                  mean: '-',
                  stdDeviation: '-'
                }
              }

              contingency.privacyCheck = contingency.aggregations.filter(function(aggregation) {
                return aggregation.statistics !== null;
              }).length === contingency.aggregations.length;

              var terms = contingency.aggregations.map(function (aggregation) {
                return aggregation.term;
              });

              if (!contingency.privacyCheck) {
                // server returns no aggregation, create emptyu ones
                contingency.aggregations.forEach(function(aggregation, i) {
                  aggregation.statistics = createEmptyStatistics();
                });

                contingency.all.statistics = createEmptyStatistics();

              } else {
                // create the missing category aggregations
                v1Cats.forEach(function (cat, i) {
                  if (terms.indexOf(cat) === -1) {
                    // create a cat at the same index
                    contingency.aggregations.splice(i, 0, { n:'-', statistics: createEmptyStatistics()});
                  }

                });
              }
            }

            function normalizeFrequencies(contingency, v2Cats) {

              function percentage(value, total) {
                return total === 0 ? 0 : value / total * 100;
              }

              function expected(cTotal, rTotal, gt) {
                return (cTotal * rTotal) / gt;
              }

              function cellChiSquared(value, expected) {
                return Math.pow(value - expected, 2) / expected;
              }

              function degreeOfFreedom(rows, columns) {
                return (rows - 1) * (columns - 1);
              }

              /**
               * Normalized data; accounts for frequencies with no value (ignored by Elasticsearch)
               * @param aggregation
               */
              function normalize(aggregation) {
                var fCats = aggregation.frequencies.map(function (frq) {
                  return frq.value;
                });

                v2Cats.forEach(function (cat, i) {
                  if (fCats.indexOf(cat) === -1) {
                    // create a cat at the same index
                    aggregation.frequencies.splice(i, 0, {count: aggregation.privacyCheck ? 0 : '-', value: cat});
                  }
                });
              }

              /**
               * Calulates frequency percentages and chi-squared
               * @param aggregation
               * @param grandTotal
               * @param totals
               * @param chiSquaredInfo
               */
              function statistics(aggregation, grandTotal, totals, chiSquaredInfo) {
                if (chiSquaredInfo) {
                  aggregation.percent = percentage(aggregation.n, grandTotal);
                  aggregation.frequencies.forEach(function (frequency, i) {
                    frequency.percent = percentage(frequency.count, totals.frequencies[i].count);
                    frequency.cpercent = percentage(frequency.count, aggregation.n);

                    chiSquaredInfo.sum += cellChiSquared(frequency.count, expected(aggregation.n, totals.frequencies[i].count, grandTotal));
                  });
                } else {
                  aggregation.frequencies.forEach(function (frequency) {
                    frequency.percent = percentage(frequency.count, grandTotal);
                    frequency.cpercent = percentage(frequency.n, grandTotal);
                  });
                }
              }

              /**
               * process contingency
               */

              var privacyThreshold = contingency.privacyThreshold;
              var grandTotal = contingency.all.total;
              contingency.all.privacyCheck = contingency.all.frequencies.length > 0;
              normalize(contingency.all, privacyThreshold);
              statistics(contingency.all, grandTotal, contingency.all);

              if (contingency.aggregations) {
                contingency.chiSquaredInfo = {
                  pValue: 0,
                  sum: 0,
                  df: degreeOfFreedom(
                    $scope.crosstab.rhs.xVariable.categories.length,
                    $scope.crosstab.lhs.xVariable.categories.length
                  )
                };

                contingency.privacyCheck = true;
                contingency.aggregations.forEach(function (aggregation) {
                  aggregation.privacyCheck = aggregation.frequencies.length > 0;
                  contingency.privacyCheck = contingency.privacyCheck && aggregation.privacyCheck;

                  normalize(aggregation);
                  statistics(aggregation, grandTotal, contingency.all, contingency.chiSquaredInfo);
                });

                if (contingency.privacyCheck) {
                  // no cell has an observation < 5
                  contingency.chiSquaredInfo.pValue = (1 - ChiSquaredCalculator.compute(contingency.chiSquaredInfo));
                }
              }
            }

            /**
             * Normalized data; fills collection with dummy values (statistical or categorical)
             * @param contingencies
             * @returns {*}
             */
            function normalizeData(contingencies) {
              var v2Cats = $scope.crosstab.rhs.xVariable.categories.map(function(category) {
                return category.name;
              });
              var v1Cats = $scope.crosstab.lhs.xVariable.categories.map(function(category) {
                return category.name;
              });

              if (contingencies) {
                contingencies.forEach(function (contingency) {
                  $log.debug('>', contingency);
                  contingency.totalPrivacyCheck = contingency.all.n !== -1; // show the details anyway
                  if (!contingency.totalPrivacyCheck || contingency.all.n > 0) {

                    if (isStatistical($scope.crosstab.rhs.xVariable)) {
                      normalizeStatistics(contingency, v1Cats);
                    } else {
                      normalizeFrequencies(contingency, v2Cats);
                    }
                  }
                });
              }

              return contingencies;
            }

            /**
             * Submits the crosstab query
             */
            var submit = function() {
              if ($scope.crosstab.lhs.variable && $scope.crosstab.rhs.variable) {
                $scope.crosstab.lhs.xVariable = $scope.crosstab.lhs.variable;
                $scope.crosstab.rhs.xVariable = $scope.crosstab.rhs.variable;

                updateLocation(
                  ContingencyService.removeVariableFromUrl($location.path()),
                  ContingencyService.createVariableUrlPart(
                    $scope.crosstab.lhs.xVariable.id,
                    $scope.crosstab.rhs.xVariable.id
                  )
                );

                $scope.loading = true;
                startProgess();

                DatasetVariablesCrosstabResource.get({
                    dsType: $routeParams.type,
                    dsId: $routeParams.ds,
                    v1: $scope.crosstab.lhs.xVariable.name,
                    v2: $scope.crosstab.rhs.xVariable.name
                  },
                  function onSuccess(response) {
                    $log.debug('Crosstab Succeeded', response);
                    $scope.crosstab.contingencies = normalizeData(response.contingencies ? response.contingencies : [response]);

                    if ($scope.datasetHarmo) {
                      $scope.crosstab.all = normalizeData(response.all ? [response.all] : [])[0];
                    }

                    endProgress();
                    $scope.loading = false;
                  },
                  onError
                );
              }
            };

            var download = function(docType) {
              var downloadUrl = ContingencyService.getCrossDownloadUrl({
                ':dsType': $routeParams.type,
                ':dsId': $routeParams.ds,
                ':docType': docType,
                ':v1': $scope.crosstab.lhs.xVariable.name,
                ':v2': $scope.crosstab.rhs.xVariable.name
              });

              var form = $("<form action='" + downloadUrl + "' method='get'>");
              $('body').after(form);
              form.submit().remove();
              return false;
            };

            var lhsVariableCategory = function(category) {
              return getVariableCategory($scope.crosstab.lhs.xVariable, category);
            };

            var rhsVariableCategory = function(category) {
              return getVariableCategory($scope.crosstab.rhs.xVariable, category);
            };

            function getVariableCategory(variable, category) {
              var result = null;

              if (variable && variable.categories) {
                result = lhsVar.categories.filter(function(cat) {
                  return cat.name === category;
                });
              }

              return result ? result[0] : category;
            }

            /**
             * Retrieves study table info for the result page
             * @param studtTable
             * @returns {{summary: *, population: *, dce: *, project: *, table: *}}
             */
            var extractStudySummaryInfo = function(studtTable) {
              var summary = studtTable.studySummary;
              var pop = summary.populationSummaries ? summary.populationSummaries[0] : null;
              var dce = pop && pop.dataCollectionEventSummaries
                  ? pop.dataCollectionEventSummaries.filter(function(dce) {
                      return dce.id === studtTable.dataCollectionEventId;
                    })
                  : null;
              return {
                summary: LocalizedStringService.getValue(summary.acronym),
                population: pop ? LocalizedStringService.getValue(pop.name) : '',
                dce: dce ? LocalizedStringService.getValue(dce[0].name) : '',
                project: studtTable.project,
                table: studtTable.table
              }
            };

            var getPrivacyErrorMessage = function(contingency) {
              return !contingency.totalPrivacyCheck
                ? 'dataset.crosstab.total-privacy-check-failed'
                : (!contingency.privacyCheck ? 'dataset.crosstab.privacy-check-failed' : '');
            };

            /**
             * Returns the proper template based on total.n
             * @param contingency
             * @param basePath
             * @returns {string}
             */
            var getTemplatePath = function(contingency, basePath){
              if (!$scope.crosstab.rhs.xVariable) {
                $log.error('RHS variable is not initialized!');
                return;
              }

              var folder = basePath + "obiba_mica_app_angular/obiba_mica_data_access_request/";
              var template = isStatistical($scope.crosstab.rhs.xVariable)
                ? (!contingency.totalPrivacyCheck || contingency.all.n > 0 ? "obiba_mica_dataset_variable_crosstab_statistics" : "obiba_mica_dataset_variable_crosstab_statistics_empty")
                : (!contingency.totalPrivacyCheck || contingency.all.n > 0 ? "obiba_mica_dataset_variable_crosstab_frequencies" : "obiba_mica_dataset_variable_crosstab_frequencies_empty");

              return folder + template;
            };

            var initCrosstab = function() {
              $scope.crosstab = {
                lhs: {
                  variable : null,
                  xVariable : null,
                  variables : []
                },
                rhs: {
                  variable: null,
                  xVariable: null,
                  variables: []
                },
                all: null,
                contingencies: null
              };
            };

            $scope.isStatistical = isStatistical;
            $scope.getTemplatePath = getTemplatePath;
            $scope.getPrivacyErrorMessage = getPrivacyErrorMessage;
            $scope.canExchangeVariables = canExchangeVariables;
            $scope.exchangeVariables = exchangeVariables;
            $scope.extractStudySummaryInfo = extractStudySummaryInfo;
            $scope.lhsVariableCategory = lhsVariableCategory;
            $scope.rhsVariableCategory = rhsVariableCategory;
            $scope.clear = clear;
            $scope.submit = submit;
            $scope.download = download;
            $scope.searchCategoricalVariables = searchCategoricalVariables;
            $scope.searchVariables = searchVariables;
            $scope.DocType = {CSV: 'csv', EXCEL: 'excel'};
            $scope.StatType = {CPERCENT: 1, RPERCENT: 2, CHI: 3};
            $scope.datasetHarmo = $routeParams.type === 'harmonization-dataset';
            $scope.routeParams = $routeParams;
            $scope.options = {
              showDetailedStats: Drupal.settings.angularjsApp.show_detailed_stats,
              showDetails: true,
              statistics: $scope.StatType.CPERCENT
            };

            initCrosstab();
            endProgress();

            DatasetResource.get({dsType: $routeParams.type, dsId: $routeParams.ds},
              function onSuccess(response) {
                $scope.dataset = response;
              },
              onError
            );

            var varCount = 0;
            if ($routeParams.varId) {
              DatasetVariableResource.get({varId: $routeParams.varId},
                function onSuccess(response) {
                  $log.debug('Variable LHS info', response);
                  $scope.crosstab.lhs.variable = response;
                  $scope.crosstab.lhs.variables = [response];
                  varCount++;
                  if (varCount > 1) submit();
                },
                onError
              );
            }

            if ($routeParams.byId) {
              DatasetVariableResource.get({varId: $routeParams.byId},
                function onSuccess(response) {
                  $log.debug('Variable RHS info', response);
                  $scope.crosstab.rhs.variable = response;
                  $scope.crosstab.rhs.variables = [response];
                  varCount++;
                  if (varCount > 1) submit();
                },
                onError
              );
            }



          }]);
    }
  }
}(jQuery));