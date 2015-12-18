<h3 class="no-top-margin">
  <?php if (empty($hide_title)) print obiba_mica_commons_get_localized_field($population, 'name') ?>
</h3>

<?php if (!empty($population->description)): ?>
  <p>
    <?php print obiba_mica_commons_get_localized_field($population, 'description'); ?>
  </p>
<?php endif; ?>

<?php if (!empty($population->recruitment->generalPopulationSources)
  || !empty($population->recruitment->studies)
  || !empty($population->recruitment->specificPopulationSources)
  || !empty($population->recruitment->otherSpecificPopulationSource)
  || !empty($population->recruitment->otherSource)
  || !empty($population->recruitment->info)
): ?>
  <h4><?php print t('Sources of Recruitment') ?></h4>

  <div class="scroll-content-tab">
    <table class="table table-striped">
      <tbody>
      <?php if (!empty($population->recruitment->generalPopulationSources)): ?>
        <tr>
          <th><?php print t('General population') ?></th>
          <td><?php obiba_mica_commons_iterate_field($population->recruitment->generalPopulationSources); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->recruitment->studies)): ?>
        <tr>
          <th><?php print t('Participants from existing studies') ?></th>
          <td>
            <?php
            $studies = obiba_mica_commons_get_localized_dtos_field($population->recruitment, 'studies');
            ?>
            <?php obiba_mica_commons_iterate_field($studies); ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->recruitment->specificPopulationSources)): ?>
        <tr>
          <th><?php print t('Specific population') ?></th>
          <td><?php obiba_mica_commons_iterate_field($population->recruitment->specificPopulationSources); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->recruitment->otherSpecificPopulationSource)): ?>
        <tr>
          <th><?php print t('Other specific population') ?></th>
          <td><?php print obiba_mica_commons_get_localized_field($population->recruitment, 'otherSpecificPopulationSource'); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->recruitment->otherSource)): ?>
        <tr>
          <th><?php print t('Supplementary information') ?></th>
          <td><?php print obiba_mica_commons_get_localized_field($population->recruitment, 'otherSource'); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->recruitment->info)): ?>
        <tr>
          <th><?php print t('Supplementary information') ?></th>
          <td><?php print obiba_mica_commons_get_localized_field($population->recruitment, 'info'); ?></td>
        </tr>
      <?php endif; ?>
      </tbody>
    </table>
  </div>
<?php endif; ?>

<?php if ((isset($population->selectionCriteria->gender) && ($population->selectionCriteria->gender === 0 || $population->selectionCriteria->gender === 1))
  || !empty($population->selectionCriteria->ageMin) || !empty($population->selectionCriteria->ageMax)
  || !empty($population->selectionCriteria->countriesIso)
  || !empty($population->selectionCriteria->territory)
  || !empty($population->selectionCriteria->ethnicOrigin)
  || !empty($population->selectionCriteria->healthStatus)
  || !empty($population->selectionCriteria->otherCriteria)
  || !empty($population->selectionCriteria->info)
): ?>
  <h4><?php print t('Selection Criteria') ?></h4>
  <div class="scroll-content-tab">
    <table class="table table-striped">
      <tbody>
      <?php if (isset($population->selectionCriteria->gender) && ($population->selectionCriteria->gender === 0 || $population->selectionCriteria->gender === 1)): ?>
        <tr>
          <th><?php print t('Gender') ?></th>
          <td><?php print  obiba_mica_study_get_gender($population->selectionCriteria->gender); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->ageMin) || !empty($population->selectionCriteria->ageMax)): ?>
        <tr>
          <th><?php print t('Age') ?></th>
          <td>
            <?php !empty($population->selectionCriteria->ageMin) ? print t('Minimum') . ' '
              .$population->selectionCriteria->ageMin
              . ((!empty($population->selectionCriteria->ageMax))? ', ':'' ): NULL;?>
            <?php !empty($population->selectionCriteria->ageMax) ? print t('Maximum') . ' '
              . $population->selectionCriteria->ageMax : NULL; ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->countriesIso)): ?>
        <tr>
          <th><?php print t('Country'); ?></th>
          <td>
            <?php obiba_mica_commons_iterate_field($population->selectionCriteria->countriesIso,
              'countries_country_lookup', 'iso3', 'name'); ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->territory)): ?>
        <tr>
          <th><?php print t('Territory') ?></th>
          <td>
            <?php print obiba_mica_commons_get_localized_field($population->selectionCriteria, 'territory'); ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->ethnicOrigin)): ?>
        <tr>
          <th><?php print t('Ethnic origin') ?></th>
          <td>
            <?php $ehtnic_origins = obiba_mica_commons_get_localized_dtos_field($population->selectionCriteria, 'ethnicOrigin'); ?>
            <?php obiba_mica_commons_iterate_field($ehtnic_origins); ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->healthStatus)): ?>
        <tr>
          <th><?php print t('Health status') ?></th>
          <td>
            <?php $health_status = obiba_mica_commons_get_localized_dtos_field($population->selectionCriteria, 'healthStatus'); ?>
            <?php obiba_mica_commons_iterate_field($health_status); ?>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->otherCriteria)): ?>
        <tr>
          <th><?php print t('Other') ?></th>
          <td><?php print obiba_mica_commons_get_localized_field($population->selectionCriteria, 'otherCriteria'); ?></td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->selectionCriteria->info)): ?>
        <tr>
          <th><?php print t('Supplementary information') ?></th>
          <td><?php print obiba_mica_commons_get_localized_field($population->selectionCriteria, 'info'); ?></td>
        </tr>
      <?php endif; ?>
      </tbody>
    </table>
  </div>
<?php endif; ?>

<?php if (!empty($population->numberOfParticipants->participant->number)
  || !empty($population->numberOfParticipants->sample->number)
  || !empty($population->info)
): ?>
  <h4><?php print t('Sample Size') ?></h4>

  <div class="scroll-content-tab">
    <table class="table table-striped">
      <tbody>

      <?php if (!empty($population->numberOfParticipants->participant->number)): ?>
        <tr>
          <th><?php print t('Number of participants') ?></th>
          <td>
            <p>
              <?php print obiba_mica_commons_format_number($population->numberOfParticipants->participant->number) ?>
              <?php if (!empty($population->numberOfParticipants->participant->noLimit)): ?>
                (<?php print t('No limit'); ?>)
              <?php endif; ?>
              <?php if (!empty($population->numberOfParticipants->info)): ?>

            <p><?php print obiba_mica_commons_get_localized_field($population->numberOfParticipants, 'info'); ?></p>
            <?php endif; ?>
            </p>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->numberOfParticipants->sample->number)): ?>
        <tr>
          <th><?php print t('Number of participants with biological samples') ?></th>
          <td>
            <p>
              <?php print obiba_mica_commons_format_number($population->numberOfParticipants->sample->number) ?>
              <?php if (!empty($population->numberOfParticipants->sample->noLimit)): ?>
                (<?php print t('No limit'); ?>)
              <?php endif; ?>
            </p>
          </td>
        </tr>
      <?php endif; ?>

      <?php if (!empty($population->info)): ?>
        <tr>
          <th><?php print t('Supplementary information') ?></th>
          <td><p> <?php print obiba_mica_commons_get_localized_field($population, 'info'); ?></p></td>
        </tr>
      <?php endif; ?>

      </tbody>
    </table>
  </div>
<?php endif; ?>

<?php if (!empty($attachments)): ?>
      <h4><?php print variable_get_value('files_documents_label'); ?></h4>
  <?php print $attachments; ?>
<?php endif; ?>