<div ng-cloak>
  <h4>{{dataset.acronym | localizedValue}} {{'dataset.crosstab.title' | translate}}</h4>

  <obiba-alert id="DatasetVariableCrosstabController"></obiba-alert>

  <!-- crosstab variable selection -->
  <div class="well well-sm">
    <div class="row">
      <div class="col-xs-4">
        <ui-select ng-model="crosstab.lhs.variable" theme="bootstrap">
          <ui-select-match placeholder="{{'dataset.crosstab.categorical' | translate}}">{{$select.selected.name}}
          </ui-select-match>
          <ui-select-choices refresh="searchCategoricalVariables($select.search)" refresh-delay="0"
                             repeat="variable in crosstab.lhs.variables">

            <div>{{variable.name}}</div>
            <small>{{variable | variableLabel}}</small>
          </ui-select-choices>
        </ui-select>
      </div>

      <div class="col-xs-1 text-center no-padding no-margin sm-width">
        <i class="fa fa-times fa-2x"></i>
      </div>

      <div class="col-xs-4">
        <ui-select ng-model="crosstab.rhs.variable" theme="bootstrap">
          <ui-select-match placeholder="{{'dataset.crosstab.another' | translate}}">{{$select.selected.name}}
          </ui-select-match>
          <ui-select-choices refresh="searchVariables($select.search)" refresh-delay="0"
                             repeat="variable in crosstab.rhs.variables">

            <div>{{variable.name}}</div>
            <small>{{variable | variableLabel}}</small>
          </ui-select-choices>
        </ui-select>
      </div>

      <div class="col-xs-3">
        <span>
          <button type="button" class="btn btn-primary" aria-hidden="true" ng-click="submit()">{{'submit' | translate}}
          </button>
          <button ng-if="canExchangeVariables()" type="button" class="btn btn-primary"
                  aria-hidden="true" ng-click="exchangeVariables()"> v1 <i class="fa fa-exchange fa-1x"></i> v2
          </button>
          <button type="button" class="btn btn-default" aria-hidden="true" ng-click="clear()">{{'clear' | translate}}
          </button>
        </span>
      </div>
    </div>
    <span class="help-block">{{'dataset.crosstab.query-help' | translate}}</span>
  </div>

  <!-- crosstab results -->
  <div ng-if="crosstab.contingencies && crosstab.contingencies.length > 0">

    <div>
      <div class="btn-group">
        <label class="btn" ng-class="{'btn-info': options.showDetails, 'btn-default': !options.showDetails}"
               ng-model="options.showDetails" btn-checkbox>{{'show-details' | translate}}</label>
        <div class="btn-group" ng-if="!isStatistical(crosstab.rhs.xVariable)">
          <label class="btn" ng-class="{'btn-info': options.showFrequency, 'btn-default': !options.showFrequency}"
                 ng-model="options.showFrequency" btn-radio="true">{{'show-frequency' | translate}}</label>
          <label class="btn" ng-class="{'btn-info': !options.showFrequency, 'btn-default': options.showFrequency}"
                 ng-model="options.showFrequency" btn-radio="false">{{'show-percentage' | translate}}</label>
        </div>
      </div>

      <div class="pull-right sm-bottom-margin">
        <div class="btn-group" role="group">
          <button type="button" class="btn btn-info btn-sm dropdown-toggle" data-toggle="dropdown" aria-haspopup="true"
                  aria-expanded="false">
            <i class="glyphicon glyphicon-download-alt"></i> <span>{{'download' | translate}}</span>
            <span class="caret"></span>
          </button>
          <ul class="dropdown-menu dropdown-menu-compact">
            <li><a ng-click="download(DocType.EXCEL)" href=""><i class="fa fa-file-excel-o"></i> {{'excel' | translate}}</a></li>
            <li><a ng-click="download(DocType.CSV)" href=""><i class="fa fa-file-text-o"></i> {{'csv' | translate}}</a></li>
          </ul>
        </div>
      </div>
    </div>
    <!-- progress image -->
    <div>
      <img
        src="<?php print base_path() . drupal_get_path('theme', obiba_mica_commons_get_current_theme()) ?>/img/spin.gif"
        ng-if="loading">
    </div>
    <table ng-if="!loading" class="table table-striped table-bordered no-margin no-padding">
      <thead>
      <tr>
        <th style="vertical-align: top" class="" ng-if="datasetHarmo" width="20%"
            rowspan="{{crosstab.lhs.xVariable.categories.length}}">{{'study-table' | translate}}
        </th>
        <th style="vertical-align: top" rowspan="{{crosstab.lhs.xVariable.categories.length}}" width="10%">
          {{crosstab.rhs.xVariable.name}}
        </th>
        <th class="text-center" colspan="{{crosstab.lhs.xVariable.categories.length}}">
          {{crosstab.lhs.xVariable.name}}
        </th>
        <th style="vertical-align: top" class="text-center" rowspan="{{crosstab.lhs.xVariable.categories.length}}">
          {{'total' | translate}}
        </th>
      </tr>
      <tr>
        <th class="text-center" ng-repeat="category in crosstab.lhs.xVariable.categories">
          <span class="clickable" title="" data-toggle="popover" data-placement="bottom" data-trigger="hover"
                data-content="{{crosstab.lhs.xVariable.categories | variableCategory:category.name | variableLabel}}">{{category.name}}</span>
        </th>
      </tr>
      </thead>

      <!-- Categorical -->
      <tbody ng-repeat="contingency in crosstab.contingencies track by $index"
             ng-if="!isStatistical(crosstab.rhs.xVariable)"
             ng-include="getTemplatePath(contingency, '<?php print base_path(); ?>')">
      </tbody>
      <tbody ng-if="datasetHarmo && !isStatistical(crosstab.rhs.xVariable)">
      <tr>
        <td colspan="{{crosstab.lhs.xVariable.categories.length + 3}}"></td>
      </tr>
      </tbody>
      <tbody ng-repeat="contingency in [crosstab.all]" ng-init="grandTotal = true"
             ng-if="datasetHarmo && !isStatistical(crosstab.rhs.xVariable)"
             ng-include="'<?php print base_path(); ?>obiba_main_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_variable_crosstab_frequencies'">
      </tbody>

      <!-- Statistical -->
      <tbody ng-repeat="contingency in crosstab.contingencies" ng-if="isStatistical(crosstab.rhs.xVariable)"
             ng-include="getTemplatePath(contingency, '<?php print base_path(); ?>')">
      </tbody>
      <tbody ng-if="datasetHarmo && isStatistical(crosstab.rhs.xVariable)">
      <tr>
        <td colspan="{{crosstab.lhs.xVariable.categories.length + 3}}"></td>
      </tr>
      </tbody>
      <tbody ng-repeat="contingency in [crosstab.all]" ng-init="grandTotal = true"
             ng-if="datasetHarmo && isStatistical(crosstab.rhs.xVariable)"
             ng-include="'<?php print base_path(); ?>obiba_main_app_angular/obiba_mica_data_access_request/obiba_mica_dataset_variable_crosstab_statistics'">
      </tbody>

    </table>
  </div>
</div>