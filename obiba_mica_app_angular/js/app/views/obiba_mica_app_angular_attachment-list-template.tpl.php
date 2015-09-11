<?php
/**
 * @file
 * Code for the obiba_mica_app_angular modules.
 */

?>

<table class="table table-striped" ng-show="attachments.length">
  <tbody>
  <tr ng-repeat="attachment in attachments">
    <th>
      <a target="_self" ng-href="{{attachment.href}}"
        download="{{attachment.fileName}}">{{attachment.fileName}}
      </a>
    </th>
    <td>
      {{attachment.size | bytes}}
    </td>
  </tr>
  </tbody>
</table>
