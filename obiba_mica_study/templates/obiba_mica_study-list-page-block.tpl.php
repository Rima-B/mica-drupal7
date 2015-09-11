<?php //dpm($study); ?>
<?php //dpm($network_digests); ?>

<div class="row sm-bottom-margin document-item-list flex-row">
  <div class="col-md-2 hidden-xs hidden-sm text-center">
    <?php if (!empty($logo_url)): ?>
      <img src="<?php print $logo_url ?>"
        class="listImageThumb img-responsive"/>
    <?php else : ?>
      <h1 class="big-character">
        <span class="t_badge color_light i-obiba-S"></span>
      </h1>
    <?php endif; ?>
  </div>
  <div class="col-md-10  col-sm-12 col-xs-12">
    <div>
      <h4>
        <?php print MicaClientAnchorHelper::studyListItem($study); ?>
      </h4>
      <hr class="no-margin">
      <p class="md-top-margin">
        <small>
          <?php if (variable_get_value('studies_list_show_trimmed_description_study')): ?>
            <?php  print MicaClientAnchorHelper::ellipses(
              t('Read more'),
              obiba_mica_commons_get_localized_field($study, 'objectives'),
              MicaClientPathProvider::study($study->id)
            ); ?>
          <?php else: ?>
            <div class="objectives-studies">

              <?php $first_paragraph = explode('</p>', obiba_mica_commons_get_localized_field($study, 'objectives')); ?>
              <div><?php print $first_paragraph['0'] ?></div>
              <div class="objectives-trimed collapse"
                id="objectives-<?php print $study->id; ?>">
                <?php if (!empty($first_paragraph['1'])): ?>
                  <?php print $first_paragraph['1'] ?>
                <?php else: ?>
                  <?php print obiba_mica_commons_get_localized_field($study, 'objectives'); ?>
                <?php endif; ?>
              </div>
              <?php if (count($first_paragraph) > 2): ?>
                <button class="btn btn-primary btn-xs 5-top-margin trim-studies" type="button" data-toggle="collapse"
                  data-target="#objectives-<?php print $study->id; ?>" aria-expanded="false"
                  aria-controls="collapseExample" id="btn-objectives-<?php print $study->id; ?>">
                  <?php print t('Read more'); ?>
                </button>
              <?php endif; ?>

            </div>
          <?php  endif; ?>
        </small>
      </p>
    </div>
    <?php if (variable_get('studies_list_show_study_sup_info')): ?>
      <ul class="search-item-list-no-style sm-top-margin help-block">
        <li>
          <?php foreach ($network_digests as $digest) {
            $names = array();
            array_push($names, obiba_mica_commons_get_localized_field($digest, 'name'));
          }

          if (!empty($names)) {
            print t('Member of ') . implode(', and', $names);
          }
          ?>
        </li>
        <?php if (!empty($study->designs) || !empty($study->targetNumber)) : ?>
          <li>
            <?php if (!empty($study->designs)): print t('Study design') ?>:
              <span><?php print implode(', ', obiba_mica_commons_clean_string($study->designs)) ?></span>
            <?php endif; ?>
            <?php
            if (!empty($study->targetNumber)):
              print (empty($study->designs) ? '' : '; ') . t('Target number of participants');
              ?>:
              <span>
              <?php print $study->targetNumber->noLimit === TRUE
                ? t('No limits')
                : (empty($study->targetNumber->number) ? t('n/a') : obiba_mica_commons_format_number($study->targetNumber->number))
              ?>
            </span>
            <?php endif; ?>
          </li>
        <?php endif; ?>
      </ul>
      <div class="sm-top-margin">
        <?php
        $counts = $study->{'obiba.mica.CountStatsDto.studyCountStats'};
        $vars = $counts->variables;
        $var_caption = $vars < 2 ? "variable" : "variables";
        $datasets = $counts->studyDatasets + $counts->harmonizationDatasets;
        $dataset_caption = $datasets < 2 ? "dataset" : "datasets";
        $networks = $counts->networks;
        $network_caption = $networks < 2 ? "network" : "networks";
        ?>
        <?php if (!empty($networks) && variable_get_value('studies_column_networks')): ?>
          <span class="label label-default rounded right-indent">
            <?php print MicaClientAnchorHelper::studyNetworks(t('@count ' . $network_caption, array('@count' => $networks)), $study->id) ?>
          </span>
        <?php endif ?>
        <?php if (!empty($datasets) && (variable_get_value('studies_column_study_datasets') || variable_get_value('studies_column_harmonization_datasets'))): ?>
          <span class="label label-default rounded right-indent">
            <?php print MicaClientAnchorHelper::studyDatasets(t('@count ' . $dataset_caption, array('@count' => $datasets)), $study->id) ?>
          </span>
        <?php endif ?>
        <?php if (!empty($vars) && variable_get_value('studies_column_variables')): ?>
          <span class="label label-default rounded">
            <?php print MicaClientAnchorHelper::studyVariables(t('@count ' . $var_caption, array('@count' => $vars)), $study->id) ?>
          </span>
        <?php endif ?>
      </div>
    <?php endif; ?>
  </div>
</div>

