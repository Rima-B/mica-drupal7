<?php
//dpm($dataset_dto);
//dpm($dataset_type_dto);
//dpm($variables_dataset);
//dpm($dataset_variables_aggs);
?>

<div>
  <?php if (!empty($dataset_dto->description)): ?>
    <p
      class="md-top-margin"><?php print obiba_mica_commons_markdown(obiba_mica_commons_get_localized_field($dataset_dto, 'description')); ?></p>
  <?php endif; ?>

  <div class="pull-right md-bottom-margin">
    <?php
    $query_array = array("variables" => array("terms" => array("datasetId" => $dataset_dto->id)));
    $query = MicaClient::create_query_dto_as_string($query_array);

    print l(t('Search Variables'), 'mica/search',

      array(
        'query' => array(
          'type' => 'variables',
          'query' => $query
        ),
        'attributes' => array('class' => 'btn btn-primary')
      ));
    ?>
    <?php
    print l(t('View Coverage'), 'mica/coverage',
      array(
        'query' => array(
          'type' => 'variables',
          'query' => $query
        ),
        'attributes' => array('class' => 'btn btn-primary indent')
      ));
    ?>
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
              <?php print MicaClientAnchorHelper::dataset_variables(empty($variables_dataset->total) ? 0 : $variables_dataset->total, $dataset_dto->id); ?>
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
<?php if ($dataset_type == "study-dataset" || !empty($dataset_type_dto->studyTables)): ?>
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
                <?php print l(obiba_mica_commons_get_localized_field($dataset_type_dto->studyTable->studySummary, 'name'), 'mica/study/' . $dataset_type_dto->studyTable->studySummary->id . '/' . obiba_mica_commons_to_slug(obiba_mica_commons_get_localized_field($dataset_type_dto->studyTable->studySummary, 'name'))); ?>
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
                  $population_anchor = l(obiba_mica_commons_get_localized_field($population_summary, 'name'), '#', array(
                    'external' => TRUE,
                    'attributes' => array(
                      'data-toggle' => 'modal',
                      'data-target' => '#population-' . $population_summary->id
                    )
                  ));

                  print $population_anchor;
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
                  $dce_anchor = l(obiba_mica_commons_get_localized_field($dce_summary, 'name'), '#', array(
                    'external' => TRUE,
                    'attributes' => array(
                      'data-toggle' => 'modal',
                      'data-target' => '#dce-' . $dataset_type_dto->studyTable->studyId . '-' . $dataset_type_dto->studyTable->populationId . '-' . $dce_summary->id
                    )
                  ));

                  print $dce_anchor;
                  break;
                }
              }
              ?>

            </td>
          </tr>
          <tr>
            <th><?php print t('Study Design') ?></th>
            <td>
              <?php print implode(', ', t(obiba_mica_commons_clean_string($dataset_type_dto->studyTable->studySummary->designs))); ?>
            </td>
          </tr>
          <tr>
            <th><?php print t('Target number of participants') ?></th>
            <td>
              <?php print isset($dataset_type_dto->studyTable->studySummary->targetNumber->noLimit) ? t('No Limit') :
                isset($dataset_type_dto->studyTable->studySummary->targetNumber->number) ? $dataset_type_dto->studyTable->studySummary->targetNumber->number : NULL; ?>
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
    <h2><?php print t('Classifications Coverage') ?></h2>
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
