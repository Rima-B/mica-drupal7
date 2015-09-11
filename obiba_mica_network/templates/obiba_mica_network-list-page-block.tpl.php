<?php //dpm($network) ?>

<div class="row sm-bottom-margin document-item-list flex-row">
  <div class="col-md-2 hidden-xs hidden-sm text-center">
    <?php if (!empty($logo_url)): ?>
      <img src="<?php print $logo_url ?>"
        class="listImageThumb"/>
    <?php else : ?>
      <h1 class="big-character">
        <span class="t_badge color_light i-obiba-N"></span>
      </h1>
    <?php endif; ?>
  </div>
  <div class="col-md-10 col-sm-12 col-xs-12">
    <div>
      <h4>
        <?php print MicaClientAnchorHelper::networkListItem($network); ?>
      </h4>
      <hr class="no-margin">
      <p class="md-top-margin">
        <small>
          <?php
          print MicaClientAnchorHelper::ellipses(
            t('Read more'),
            obiba_mica_commons_get_localized_field($network, 'description'),
            MicaClientPathProvider::NETWORK($network->id)
          );
          ?>
        </small>
      </p>
    </div>
    <div class="sm-top-margin">
      <?php
      $counts = $network->{'obiba.mica.CountStatsDto.networkCountStats'};
      $variables = $counts->variables;
      $vars_caption = $variables < 2 ? t('variable') : t('variables');
      $datasets = $counts->studyDatasets + $counts->harmonizationDatasets;
      $dataset_caption = $datasets < 2 ? "dataset" : "datasets";
      $studies = $counts->studies;
      $caption = $studies < 2 ? 'study' : 'studies';
      ?>
      <?php if (!empty($studies) && variable_get_value('networks_column_studies')): ?>
        <span class="label label-default rounded right-indent">
          <?php print MicaClientAnchorHelper::networkStudies(t('@count ' . $caption, array('@count' => $studies)), $network->id) ?>
        </span>
      <?php endif ?>
      <?php if (!empty($datasets) && (variable_get_value('networks_column_study_datasets') || variable_get_value('networks_column_harmonization_datasets'))): ?>
        <span class="label label-default rounded right-indent">
            <?php print MicaClientAnchorHelper::networkDatasets(t('@count ' . $dataset_caption, array('@count' => $datasets)), $network->id) ?>
          </span>
      <?php endif ?>
      <?php if (!empty($variables) && variable_get_value('networks_column_variables')): ?>
        <span class="label label-default rounded">
            <?php print MicaClientAnchorHelper::networkVariables(t('@count ' . $vars_caption,
              array('@count' => obiba_mica_commons_format_number($variables))), $network->id) ?>
          </span>
      <?php endif ?>
    </div>
  </div>
</div>

