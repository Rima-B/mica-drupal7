/*
 * Copyright (c) 2014 OBiBa. All rights reserved.
 *
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
'use strict';

(function ($) {
  Drupal.behaviors.obiba_mica_dataset_variable_crosstab_service = {
    attach: function (context, settings) {

      mica.DatasetVariableCrosstab
        .factory('DatasetCategoricalVariablesResource', ['$resource',
          function ($resource) {
            return $resource(':dsType/:dsId/variables/:query/categorical/ws', {}, {
              'get': {method: 'GET', errorHandler: true}
            });
          }])

        .factory('DatasetVariablesResource', ['$resource',
          function ($resource) {
            return $resource(':dsType/:dsId/variables/:query/ws', {}, {
              'get': {method: 'GET', errorHandler: true}
            });
          }])

        .factory('DatasetVariableResource', ['$resource',
          function ($resource) {
            return $resource('variable/:varId/ws', {}, {
              'get': {method: 'GET', errorHandler: true}
            });
          }])

        .factory('DatasetVariablesCrosstabResource', ['$resource',
          function ($resource) {
            return $resource(':dsType/:dsId/variables/cross/:v1/by/:v2/ws', {}, {
              'get': {method: 'GET', errorHandler: true}
            });
          }])

        .factory('DatasetResource', ['$resource',
          function ($resource) {
            return $resource('dataset/:dsType/:dsId/ws', {}, {
              'get': {method: 'GET', errorHandler: true}
            });
          }])

        .service('ContingencyService',
          function () {

            function searchReplace(pattern, params) {
              return pattern.replace(/:\w+/g, function(all) {
                return params[all] || all;
              });
            }

            return {

              removeVariableFromUrl: function(path) {
                return path.replace(/\/variable\/.*/, '');
              },

              getCrossDownloadUrl: function(params) {
                return searchReplace(':dsType/:dsId/download_:docType/cross/:v1/by/:v2/ws', params);
              },

              createVariableUrlPart: function(var1, var2) {
                var params = {
                  ':var': var1,
                  ':by': var2
                };

                return searchReplace('variable/:var/by/:by', params);
              }
            }
          })

        .service('ChiSquaredCalculator', function() {

          function LogGamma(Z) {
            var S = 1+76.18009173/Z-86.50532033/(Z+1)+24.01409822/(Z+2)-1.231739516/(Z+3)+.00120858003/(Z+4)-.00000536382/(Z+5);
            var LG = (Z-.5)*Math.log(Z+4.5)-(Z+4.5)+Math.log(S*2.50662827465);
            return LG
          }

          function Gcf(X,A) {        // Good for X>A+1
            var A0=0;
            var B0=1;
            var A1=1;
            var B1=X;
            var AOLD=0;
            var N=0;
            while (Math.abs((A1-AOLD)/A1)>.00001) {
              AOLD=A1;
              N=N+1;
              A0=A1+(N-A)*A0;
              B0=B1+(N-A)*B0;
              A1=X*A0+N*A1;
              B1=X*B0+N*B1;
              A0=A0/B1;
              B0=B0/B1;
              A1=A1/B1;
              B1=1;
            }
            var Prob=Math.exp(A*Math.log(X)-X-LogGamma(A))*A1;
            return 1-Prob
          }

          function Gser(X,A) {        // Good for X<A+1.
            var T9=1/A;
            var G=T9;
            var I=1;
            while (T9>G*.00001) {
              T9=T9*X/(A+I);
              G=G+T9;
              I=I+1;
            }
            G=G*Math.exp(A*Math.log(X)-X-LogGamma(A));
            return G
          }

          function Gammacdf(x,a) {
            var GI;
            if (x<=0) {
              GI=0
            } else if (x<a+1) {
              GI=Gser(x,a)
            } else {
              GI=Gcf(x,a)
            }
            return GI
          }

          return {
            compute: function (chiSquaredInfo) {
              var Chisqcdf = null;
              var Z = chiSquaredInfo.sum;
              var DF = chiSquaredInfo.df;
              if (DF <= 0) {
                alert("Degrees of freedom must be positive")
              } else {
                Chisqcdf = Gammacdf(Z / 2, DF / 2)
              }
              Chisqcdf = Math.round(Chisqcdf * 100000) / 100000;
              return Chisqcdf;
            }
          }
        })
    }
  }
}(jQuery));