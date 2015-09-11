<?php print render($node_page) ?>

<div class="list-page">

  <div class="row">
    <?php
    $count = empty($total_items) ? 0 : $total_items;
    $caption = $count < 2 ? t('Dataset') : t('Datasets');
    ?>
    <div class="col-md-2 col-sm-2 col-xs-4 min-height-align search-count">
      <span id="refresh-count">
        <?php print $count ?>
      </span>
      <span id="refresh-count">
        <?php print $caption ?>
      </span>
    </div>

    <div class="col-md-10  col-sm-10 col-xs-8 min-height-align pull-right">
      <div class="hidden-xs inline pull-left">
        <?php print render($form_search); ?>
      </div>
      <div class="btn-group pull-right">
        <?php if (variable_get_value('mica_statistics_coverage')): ?>
          <button type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown" aria-expanded="false">
            <?php print t('Search') ?> <span class="caret"></span>
          </button>
          <ul class="dropdown-menu" role="menu">
            <li><?php print MicaClientAnchorHelper::coverageDatasets() ?></li>
            <li><?php print MicaClientAnchorHelper::searchDatasets() ?></li>
          </ul>
        <?php else:
          print MicaClientAnchorHelper::searchDatasets(true);
        ?>
        <?php endif; ?>
      </div>
    </div>

  </div>


  <div id="refresh-list">
    <?php if (!empty($list_datasets)): ?>
      <?php print render($list_datasets); ?>
    <?php endif; ?>
  </div>
  <div><?php print $pager_wrap; ?></div>
</div>
