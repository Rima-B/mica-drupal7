<?php
$description = empty($dataset_dto->description)
  ? NULL
  : obiba_mica_commons_get_localized_field($dataset_dto, 'description');
?>

<div>
  <?php if (!variable_get_value('dataset_description_overview_field') && !empty($description)): ?>
    <p class="md-top-margin"><?php print obiba_mica_commons_markdown($description); ?></p>
  <?php endif; ?>

  <div class="pull-right md-bottom-margin">
    <?php if (variable_get_value('mica_statistics')) print MicaClientAnchorHelper::datasetCrosstab($dataset_dto, TRUE) ?>
    <div class="btn-group">
      <?php if (variable_get_value('mica_statistics_coverage')): ?>
        <button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" aria-expanded="false">
          <?php print t('Search') ?> <span class="caret"></span>
        </button>
        <ul class="dropdown-menu" role="menu">
          <li><?php print MicaClientAnchorHelper::coverageDataset($dataset_dto->id) ?></li>
          <li><?php print MicaClientAnchorHelper::datasetVariables(NULL, $dataset_dto->id) ?></li>
        </ul>
      <?php
      else:
        print MicaClientAnchorHelper::datasetVariables(NULL, $dataset_dto->id, array('class' => 'btn btn-primary indent'));
        ?>
      <?php endif; ?>
    </div>
  </div>

</div>

<div class="clearfix"></div>

<article>

<!-- OVERVIEW -->
<section>
  <h2><?php print t('Overview') ?></h2>

  <div class="row">
    <div class="col-lg-6 col-xs-12 lg-right-indent">
      <table class="table table-striped">
        <tbody>

        <?php if (!empty($dataset_dto->acronym)): ?>
          <tr>
            <th><?php print t('Acronym') ?></th>
            <td><p><?php print obiba_mica_commons_get_localized_field($dataset_dto, 'acronym'); ?></p></td>
          </tr>
        <?php endif; ?>

        <?php if (variable_get_value('dataset_description_overview_field') && !empty($description)): ?>
          <tr>
            <th><?php print t('Description') ?></th>
            <td><p><?php print obiba_mica_commons_markdown($description); ?></p></td>
          </tr>
        <?php endif; ?>

        <tr>
          <th><?php print t('Dataset Type'); ?></th>
          <td>
            <p>
              <?php
              if (!empty($dataset_type_dto->project)):
                echo t('Harmonization dataset');
              else:
                echo t('Study dataset');
              endif;
              ?>
            </p>
          </td>
        </tr>
        <tr>
          <th><?php print t('Number of variables') ?></th>
          <td>
            <p>
              <?php print MicaClientAnchorHelper::datasetVariables(empty($variables_dataset->total) ? 0 :
                obiba_mica_commons_format_number($variables_dataset->total), $dataset_dto->id); ?>
            </p>
          </td>
        </tr>
        </tbody>
      </table>
    </div>
    <div class="col-lg-6 col-xs-12">
      <?php if (!empty($dataset_dto->attributes)): ?>
        <h4><?php print t('Attributes') ?></h4>
        <p><?php print obiba_mica_dataset_attributes_tab($dataset_dto->attributes, 'maelstrom'); ?></p>
      <?php endif; ?>
    </div>
  </div>

</section>

<!-- STUDIES -->
<?php if (variable_get_value('dataset_show_studies') && ($dataset_type == "study-dataset" || !empty($dataset_type_dto->studyTables))): ?>
  <section>
    <h2>
      <?php
      if (!empty($dataset_type_dto->project)):
        echo t('Studies');
      else:
        echo t('Study');
      endif;
      ?>
    </h2>
    <?php if (!empty($dataset_type_dto->project)): ?>
      <div id="studies-table">
        <div class="row">
          <div class="col-lg-12 col-xs-12">
            <table class="table table-striped" id="table-studies"></table>
          </div>
        </div>
      </div>
    <?php else: ?>
    <div class="row">
      <div class="col-lg-6 col-xs-12">
        <table class="table table-striped">
          <tbody>
          <tr>
            <th><?php print t('Acronym') ?></th>
            <td>
              <p>
                <?php print obiba_mica_commons_get_localized_field($dataset_type_dto->studyTable->studySummary, 'acronym'); ?>
              </p>
            </td>
          </tr>
          <tr>
            <th><?php print t('Name') ?></th>
            <td>
              <p>
                <?php print MicaClientAnchorHelper::study($dataset_type_dto->studyTable->studySummary); ?>
              </p>
            </td>
          </tr>
          <tr>
            <th><?php print t('Population') ?></th>
            <td>
              <?php $population_summary = NULL; ?>
              <?php foreach ($dataset_type_dto->studyTable->studySummary->populationSummaries as $pop_summary) {
                if ($pop_summary->id == $dataset_type_dto->studyTable->populationId) {
                  $population_summary = $pop_summary;
                  print MicaClientAnchorHelper::studyPopulationModal($pop_summary);
                  break;
                }
              }
              ?>
            </td>
          </tr>
          <tr>
            <th><?php print t('Data Collection Event') ?></th>
            <td>
              <?php
              $dce_anchor = NULL;
              foreach ($population_summary->dataCollectionEventSummaries as $dce_summary) {
                if ($dce_summary->id == $dataset_type_dto->studyTable->dataCollectionEventId) {
                  print MicaClientAnchorHelper::studyPopulationDceModal(
                    $dataset_type_dto->studyTable->studyId,
                    $dataset_type_dto->studyTable->populationId,
                    $dce_summary
                  );

                  break;
                }
              }
              ?>

            </td>
          </tr>
          <tr>
            <th><?php print t('Study Design') ?></th>
            <td>
              <?php print implode(', ', obiba_mica_commons_clean_string($dataset_type_dto->studyTable->studySummary->designs)); ?>
            </td>
          </tr>
          <tr>
            <th><?php print t('Target number of participants') ?></th>
            <td>
              <?php print isset($dataset_type_dto->studyTable->studySummary->targetNumber->noLimit) ? t('No Limit') :
                isset($dataset_type_dto->studyTable->studySummary->targetNumber->number) ?
                  obiba_mica_commons_format_number($dataset_type_dto->studyTable->studySummary->targetNumber->number) : NULL; ?>
            </td>
          </tr>
          <tr>
            <th><?php print t('Countries') ?></th>
            <td>
              <?php print obiba_mica_commons_countries($dataset_type_dto->studyTable->studySummary->countries); ?>
            </td>
          </tr>
          </tbody>
        </table>
      </div>
      <?php endif ?>
      <?php
      if (!empty($modals)) {
        if (!empty($modals['population'])) {
          print render($modals['population'][0]);
        }
        if (!empty($modals['dce'])) {
          print render($modals['dce'][0]);
        }
      }
      ?>
  </section>
<?php endif ?>

<!-- COVERAGE -->
<?php if (!empty($coverage)): ?>
  <section>
    <h2><?php print t('Variable Classification') ?></h2>
    <?php foreach ($coverage as $taxonomy_coverage): ?>
      <h3><?php print obiba_mica_commons_get_localized_field($taxonomy_coverage['taxonomy'], 'titles'); ?></h3>
      <p class="help-block">
        <?php print obiba_mica_commons_get_localized_field($taxonomy_coverage['taxonomy'], 'descriptions'); ?>
      </p>
      <div class="scroll-content-tab">
        <?php print render($taxonomy_coverage['chart']); ?>
      </div>
    <?php endforeach ?>
  </section>
<?php endif; ?>

<!-- VARIABLES -->
<?php if ($dataset_type != "study-dataset"): ?>
  <section class="table-variables">
    <h2><?php print t('Harmonization') ?></h2>
    <?php print render($harmonization_table_legend); ?>
    <div id="download-btn">
      <a href="" class="btn btn-success pull-right"><i
          class='glyphicon glyphicon-download'></i> <?php print t('Download') ?></a>
    </div>
    <div class="clearfix">
    </div>
    <div id="variables-table"
      type-dataset="<?php print $dataset_type; ?>"
      id-dataset="<?php print $dataset_dto->id; ?>">

    <div class="row">
        <div class="col-lg-12 col-xs-12">
          <table class="table table-striped" id="table-variables"></table>
        </div>
      </div>
    </div>

    <div class="clearfix"></div>
  </section>
<?php endif; ?>

</article>
