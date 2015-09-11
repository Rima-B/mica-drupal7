<tr>
  <td ng-if="datasetHarmo">
    <span ng-include="'<?php print base_path(); ?>obiba_mica_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_study_table'"></span>
  </td>
  <td colspan="4" class="danger"><em>{{'no-results' | translate}}</em></td>
</tr>
