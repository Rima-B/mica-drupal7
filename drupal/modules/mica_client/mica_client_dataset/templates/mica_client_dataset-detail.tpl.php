<?php
//dpm($dataset_dto);
//dpm($dataset_type_dto);
//dpm($dataset_harmonizations_dto);
//dpm($dataset_variables_aggs);
?>

<?php if (!empty($dataset_dto->description)): ?>
  <p><?php print mica_client_commons_get_localized_field($dataset_dto, 'description'); ?></p>
<?php endif; ?>

<article>
  <!-- OVERVIEW -->
  <section>
    <h3><?php print t('Overview') ?></h3>

    <div class="container-fluid">
      <div class="row">
        <div class="col-xs-6 right-indent">
          <?php if (!empty($dataset_dto->acronym)): ?>
            <h5><?php print t('Acronym') ?></h5>
            <p><?php print mica_client_commons_get_localized_field($dataset_dto, 'acronym'); ?></p>
          <?php endif; ?>

          <h5><?php print t('Entity Type'); ?></h5>

          <p><?php print t($dataset_dto->entityType); ?></p>

          <h5><?php print t('Dataset Type'); ?></h5>

          <p>
            <?php
            if (!empty($dataset_type_dto->project)):
              echo t('Harmonization dataset');
            else:
              echo t('Study dataset');
            endif;
            ?>
          </p>
        </div>
        <div class="col-xs-6">
          <?php if (!empty($dataset_dto->attributes)): ?>
            <h5><?php print t('Attributes') ?></h5>
            <p><?php print mica_client_dataset_attributes_tab($dataset_dto->attributes, 'maelstrom'); ?></p>
          <?php endif; ?>
        </div>
      </div>
    </div>
  </section>

  <!-- VARIABLES -->
  <section>
    <div class="container-fluid">
      <div class="row">
        <div class="col-xs-3 right-indent">
          <h3><?php print t('Variables') ?></h3>

          <h5><?php print t('Number of variables') ?></h5>

          <p>
            <?php print empty($dataset_variables_aggs['totalHits']) ? 0 : $dataset_variables_aggs['totalHits']; ?>
          </p>

          <?php
          print l(t('Search Variables'), 'mica/search',
            array(
              'query' => array(
                'type' => 'variables',
                'query' => '{"variables":{"terms":{"datasetId":["' . $dataset_dto->id . '"]}}}'
              ),
              'attributes' => array('class' => 'btn btn-primary')
            ));
          ?>
        </div>
        <div class="col-xs-9">
          <h3><?php print t('Domain Coverage') ?></h3>
          <!-- Variable aggregations can be reported here -->
          <div class="alert alert-info" role="alert"><strong>TODO</strong> charts here</div>
        </div>
      </div>
    </div>
  </section>

  <!-- STUDIES -->
  <section>
    <h3><?php print t('Studies') ?></h3>
    <?php print mica_client_dataset_study_tables_table($dataset_type_dto) ?>
  </section>

  <!-- HARMONIZATION -->
  <?php if (!empty($dataset_harmonizations_dto)): ?>
    <section>
      <h3><?php print t('Harmonization') ?></h3>
      <?php print mica_client_dataset_harmonizations_table($dataset_type_dto, $dataset_harmonizations_dto) ?>
    </section>
  <?php endif; ?>
</article>