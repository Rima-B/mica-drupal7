<div class="legend lg-bottom-margin pull-left">
  <?php if (!empty($clickable)): ?>
    <p>
      <?php print t('Click on each status icon to get more details on the corresponding harmonization results') ?>:
    </p>
  <?php endif ?>
  <div>
    <i class="glyphicon glyphicon-question-sign alert-warning"></i>
    <h6><?php print t('Undetermined') ?></h6>
    <?php print ' - ' . t('the harmonization potential of this variable has not yet been evaluated.') ?>
  </div>
  <div>
    <i class="glyphicon glyphicon-ok alert-success"></i>
    <h6><?php print t('Complete') ?></h6>
    <?php print ' - ' . t('the study assessment item(s) (e.g. survey question, physical measure, biochemical measure) allow construction of the variable as defined in the dataset.') ?>
  </div>
  <div>
    <i class="glyphicon <?php print  ObibaDatasetConstants::getIcon(); ?> "></i>
    <h6><?php print t(variable_get('dataset_harmonization_impossible_label')); ?></h6>
    <?php print ' - ' . t('there is no information or insufficient information collected by this study to allow the construction of the variable as defined in the dataset.') ?>
  </div>
</div>
