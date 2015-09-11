<tr ng-if="options.showDetails">
  <td ng-if="datasetHarmo" rowspan="{{!grandTotal && !contingency.privacyCheck ? '6' : '5'}}">
    <span ng-if="!grandTotal" ng-include="'<?php print base_path(); ?>obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_study_table'"></span>
    <span ng-if="grandTotal"><strong>{{'total' | translate}}</strong></span>
  </td>
  <td translate>min</td>
  <td class="text-center" ng-repeat="aggregation in contingency.aggregations">{{aggregation.statistics.min | roundNumber}}</td>
  <td class="text-center">{{contingency.all.statistics.min | roundNumber}}</td>
</tr>
<tr ng-if="options.showDetails">
  <td translate>max</td>
  <td class="text-center" ng-repeat="aggregation in contingency.aggregations">{{aggregation.statistics.max | roundNumber}}</td>
  <td class="text-center">{{contingency.all.statistics.max | roundNumber}}</td>
</tr>
<tr ng-if="options.showDetails">
  <td translate>mean</td>
  <td class="text-center" ng-repeat="aggregation in contingency.aggregations">{{aggregation.statistics.mean | roundNumber}}</td>
  <td class="text-center">{{contingency.all.statistics.mean | roundNumber}}</td>
</tr>
<tr ng-if="options.showDetails">
  <td translate>std-deviation</td>
  <td class="text-center" ng-repeat="aggregation in contingency.aggregations">{{aggregation.statistics.stdDeviation | roundNumber}}</td>
  <td class="text-center">{{contingency.all.statistics.stdDeviation | roundNumber}}</td>
</tr>
<tr>
  <td ng-if="datasetHarmo && !options.showDetails">
    <span ng-if="!grandTotal" ng-include="'<?php print base_path(); ?>obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_study_table'"></span>
    <span ng-if="grandTotal"><strong>{{'total' | translate}}</strong></span>
  </td>
  <td>N</td>
  <td class="text-center" ng-repeat="aggregation in contingency.aggregations">
    <span ng-if="contingency.totalPrivacyCheck">
      {{aggregation.n}}
    </span>
    <span ng-if="!contingency.totalPrivacyCheck">
      -
    </span>
  </td>
  <td class="text-center">
    <span ng-if="contingency.totalPrivacyCheck">
      {{contingency.all.n}}
    </span>
    <span ng-if="!contingency.totalPrivacyCheck">
      -
    </span>
  </td>
</tr>
<tr>
  <td ng-if="!grandTotal && !contingency.privacyCheck" colspan="{{crosstab.lhs.xVariable.categories.length + 3}}">
    <span class="text-danger" ng-if="!contingency.privacyCheck">
      {{getPrivacyErrorMessage(contingency) | translate:{arg0:contingency.privacyThreshold} }}
    <span>
  </td>
</tr>
