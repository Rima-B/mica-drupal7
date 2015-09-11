<?php
//dpm($coverages);
//dpm($vocabulary_coverage_outputs);
//dpm($query);
//dpm($group_by);

// protect agaist empty queries
if (empty($query)) {
  $query = array();
}
?>

<div class="alert alert-info">
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

<div>

  <div class="pull-right facet-search-form"><?php print render($variable_search_form); ?></div>
  <div class="clearfix"></div>
  <p class="pull-left">
  <?php if (!empty($coverages->totalHits)): ?>
      <?php print t('%hits variables', array(
        '%hits' => $coverages->totalHits,
      )) ?>
    <?php else: ?>
      <?php print t('%hits variables', array(
        '%hits' => 0
      )) ?>
    <?php endif ?>
    <?php
      print MicaClientAnchorHelper::ajaxFriendlyAnchor(
        MicaClientPathProvider::SEARCH,
        t(variable_get_value('variables_search_label')),
        array('class' => 'btn btn-primary indent'),
        array('type' => 'variables', 'query' => $query)
      );
    ?>
    <span id="download-coverage">
      <a href="" class="btn btn-success"><i
          class='glyphicon glyphicon-download'></i> <?php print t('Download') ?></a>
    </span>
  </p>

  <ul class="nav nav-pills pull-right">
    <li class="<?php if (!empty($group_by) && $group_by == 'studyIds') print 'active' ?>" data-toggle="tooltip"
      data-placement="top" title="<?php print t('Group by study') ?>">
      <?php
        print MicaClientAnchorHelper::ajaxFriendlyAnchor(
          MicaClientPathProvider::COVERAGE,
          t('Study'),
          array('class' => 'group-by'),
          array('group-by' => 'studyIds', 'query' => $query, 'with-facets' => 'false')
        );
      ?>
    </li>
    <li class="<?php if (!empty($group_by) && $group_by == 'dceIds') print 'active' ?>" data-toggle="tooltip"
      data-placement="top" title="<?php print t('Group by data collection event') ?>">
      <?php
        print MicaClientAnchorHelper::ajaxFriendlyAnchor(
          MicaClientPathProvider::COVERAGE,
          t('Data Collection Event'),
          array('class' => 'group-by'),
          array('group-by' => 'dceIds', 'query' => $query, 'with-facets' => 'false')
        );
      ?>
    </li>
    <li class="<?php if (!empty($group_by) && $group_by == 'datasetId') print 'active' ?>" data-toggle="tooltip"
      data-placement="top" title="<?php print t('Group by dataset') ?>">
      <?php
        print MicaClientAnchorHelper::ajaxFriendlyAnchor(
          MicaClientPathProvider::COVERAGE,
          t('Dataset'),
          array('class' => 'group-by'),
          array('group-by' => 'datasetId', 'query' => $query, 'with-facets' => 'false')
        );
      ?>
    </li>
  </ul>
</div>

<?php
$has_coverage = !empty($coverages->totalHits);
?>

<article id="coverages" class="pull-left">
</article>

<?php if (!$has_coverage): ?>
  <p class="md-top-margin pull-left">
    <i><?php print t(variable_get_value('variables_empty_label')); ?></i>
  </p>
<?php endif ?>

<div class="back-to-top t_badge"><i class="glyphicon glyphicon-arrow-up"></i></div>
