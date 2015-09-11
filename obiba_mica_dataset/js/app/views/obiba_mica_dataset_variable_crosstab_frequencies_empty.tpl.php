<tr>
  <td ng-if="datasetHarmo" rowspan="{{crosstab.rhs.xVariable.categories.length + 1}}">
    <span ng-include="'<?php print base_path(); ?>obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_study_table'"></span>
  </td>
  <td colspan="{{crosstab.lhs.xVariable.categories.length + 2}}"><em>{{'no-results' | translate}}</em></td>
</tr>
