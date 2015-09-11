<?php //dvm('search var',$studies);

$show_tabs = TRUE;
if (!empty($search_param)) {
  $count = 0;
  foreach ($search_param as $param) {
    $count += !empty($param);
  }

  $show_tabs = $count > 1;
  $border_style = $show_tabs ? '' : 'no-border';
  $padding_style = $show_tabs ? '' : 'no-padding';
}

?>
<div class="alert alert-info" xmlns="http://www.w3.org/1999/html">
  <div id="search-help">
    <i class="glyphicon glyphicon-arrow-left"></i>
    <span class="indent">
      <?php print t('Start searching by selecting a facet'); ?>
    </span>
  </div>
  <div id="search-query"></div>
  <div id="query-help" style="display:none;" class="md-top-margin help-inline">
    <i class="glyphicon glyphicon-question-sign"></i>
    <span class="indent">
      <?php print t('Tips: click on a criteria to remove it, click on AND/OR to switch it.'); ?>
    </span>
  </div>
</div>

<div id="search-result">
  <?php if ($show_tabs): ?>
    <ul class="nav nav-tabs" role="tablist" id="result-search">

      <?php if (!empty($search_param['search_networks'])): ?>
        <li><a href="#networks" role="tab"> <?php print t('Networks') ?>
            (<?php print !empty($network_total_hits) ? $network_total_hits : 0; ?>)
          </a>
        </li>
      <?php endif; ?>

      <?php if (!empty($search_param['search_studies'])): ?>
        <li><a href="#studies" role="tab"> <?php print t('Studies') ?>
            (<?php print !empty($study_total_hits) ? $study_total_hits : 0; ?>)
          </a>
        </li>
      <?php endif; ?>

      <?php if (!empty($search_param['search_datasets'])): ?>
        <li><a href="#datasets" role="tab"> <?php print t('Datasets') ?>
            (<?php print !empty($dataset_total_hits) ? $dataset_total_hits : 0; ?>)
          </a>
        </li>
      <?php endif; ?>

      <?php if (!empty($search_param['search_variables'])): ?>
        <li class="active"><a href="#variables" role="tab"><?php print t('Variables') ?>
            (<?php print !empty($variable_total_hits) ? obiba_mica_commons_format_number($variable_total_hits) : 0; ?>)
          </a>
        </li>
      <?php endif; ?>

    </ul>
  <?php endif; ?>

  <!-- Tab panes -->
  <div class="tab-content search-result">

    <?php if (!empty($search_param['search_variables'])): ?>
      <div class="tab-pane active" id="variables">
        <article class="<?php print $border_style ?>">
          <section class="<?php print $border_style . ' ' . $padding_style ?>">
            <div>
              <?php if ($show_tabs): ?>
                <h2 class="pull-left"><?php print t('Variables') ?></h2>
              <?php else: ?>
                <h3 class="pull-left">
                  <?php print t('Variables') ?>
                  <span style="color: lightgray">
                  <?php print  ' (' . (!empty($variable_total_hits) ?
                      obiba_mica_commons_format_number($variable_total_hits) : 0) . ')'; ?>
                </span>
                </h3>
              <?php endif; ?>

              <div class="pull-right lg-top-margin facet-search-form">
                <div class="row form-button-group">
                  <?php if (variable_get_value('mica_statistics_coverage')): ?>
                    <div class="col-md-2"><?php print render($variable_select_size_form); ?></div>
                    <div class="col-md-5"><?php print render($variable_search_form) ?> </div>
                    <div class="col-md-4 pull-right">
                      <?php
                      print MicaClientAnchorHelper::ajaxFriendlyAnchor(
                        MicaClientPathProvider::COVERAGE,
                        t(variable_get_value('variable_coverage_label')),
                        array('class' => 'btn btn-primary indent'),
                        array('query' => $query, 'group-by' => 'studyIds')
                      );
                      ?>
                    </div>
                  <?php else: ?>
                    <div class="pull-right no-padding">
                      <div
                        class="col-md-4"><?php print render($variable_select_size_form); ?></div>
                      <div
                        class="col-md-8 no-padding"><?php print render($variable_search_form) ?> </div>
                    </div>
                  <?php endif; ?>
                </div>

              </div>
            </div>
            <div class="clearfix"/>
            <?php print $variables_result['data']; ?>
          </section>
        </article>
      </div>
    <?php endif; ?>

    <?php if (!empty($search_param['search_datasets'])): ?>
      <div class="tab-pane" id="datasets">
        <article>
          <section>
            <h2 class="pull-left"><?php print t('Datasets') ?></h2>

            <div class="pull-right lg-top-margin facet-search-form">
              <div class="row form-button-group">
                <div class="col-md-2"><?php print render($dataset_select_size_form); ?></div>
                <div class="col-md-5 "><?php print render($dataset_search_form) ?> </div>
                <div class="col-md-4 pull-right"><?php
                  if (variable_get_value('mica_statistics_coverage')) {
                    print MicaClientAnchorHelper::ajaxFriendlyAnchor(
                      MicaClientPathProvider::COVERAGE,
                      t(variable_get_value('variable_coverage_label')),
                      array('class' => 'btn btn-primary indent'),
                      array('query' => $query, 'group-by' => 'studyIds')
                    );
                  }
                  ?>
                </div>
              </div>

            </div>

            <div class="clearfix"></div>
            <?php print $datasets['data']; ?>
          </section>
        </article>
      </div>
    <?php endif; ?>

    <?php if (!empty($search_param['search_studies'])): ?>
      <div class="tab-pane" id="studies">
        <article>
          <section>
            <h2 class="pull-left"><?php print t('Studies') ?></h2>

            <div class="pull-right lg-top-margin facet-search-form">
              <div class="row form-button-group">
                <div class="col-md-2"><?php print render($study_select_size_form); ?></div>
                <div class="col-md-5 "><?php print render($study_search_form) ?> </div>
                <div class="col-md-4 pull-right"><?php
                  if (variable_get_value('mica_statistics_coverage')) {
                    print MicaClientAnchorHelper::ajaxFriendlyAnchor(
                      MicaClientPathProvider::COVERAGE,
                      t(variable_get_value('variable_coverage_label')),
                      array('class' => 'btn btn-primary indent'),
                      array('query' => $query, 'group-by' => 'studyIds')
                    );
                  }
                  ?>
                </div>
              </div>

            </div>
            <div class="clearfix"></div>
            <?php print $studies['data']; ?>
          </section>
        </article>
      </div>
    <?php endif; ?>

    <?php if (!empty($search_param['search_networks'])): ?>
      <div class="tab-pane" id="networks">
        <article>
          <section>
            <h2 class="pull-left"><?php print t('Networks') ?></h2>

            <div class="pull-right lg-top-margin facet-search-form">
              <div class="row form-button-group">
                <div class="col-md-2"><?php print render($network_select_size_form); ?></div>
                <div class="col-md-5 "><?php print render($network_search_form) ?> </div>
                <div class="col-md-4 pull-right"><?php
                  if (variable_get_value('mica_statistics_coverage')) {
                    print MicaClientAnchorHelper::ajaxFriendlyAnchor(
                      MicaClientPathProvider::COVERAGE,
                      t(variable_get_value('variable_coverage_label')),
                      array('class' => 'btn btn-primary indent'),
                      array('query' => $query, 'group-by' => 'studyIds')
                    );
                  }
                  ?>
                </div>
              </div>

            </div>

            <div class="clearfix"/>
            <?php print $networks['data']; ?>
          </section>
        </article>
      </div>
    <?php endif; ?>

  </div>

</div>