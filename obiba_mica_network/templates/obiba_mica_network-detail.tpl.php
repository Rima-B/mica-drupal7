<?php
//dpm($network_dto);
?>

<div>
  <?php if (!empty($network_dto->description)): ?>
    <p class="md-top-margin">
      <?php if (!empty($network_dto->logo->id)): ?>
        <a
          href="<?php print obiba_mica_commons_safe_expose_server_url($network_dto->id, $network_dto->logo, 'network') ?>"
          class="fancybox-button">
          <img
            src="<?php print obiba_mica_commons_safe_expose_server_url($network_dto->id, $network_dto->logo, 'network') ?>"
            class="imageThumb">
        </a>
      <?php endif; ?>
      <?php print obiba_mica_commons_markdown(obiba_mica_commons_get_localized_field($network_dto, 'description')); ?>
    </p>
  <?php endif; ?>

  <?php if (!empty($network_dto->studyIds)): ?>
    <div class="btn-group pull-right md-bottom-margin">
      <?php if (variable_get_value('mica_statistics_coverage')): ?>
        <button type="button"
                class="btn btn-primary dropdown-toggle <?php print $has_variables ? '' : 'hidden'; ?>"
                data-toggle="dropdown" aria-expanded="false">
          <?php print t('Search') ?> <span class="caret"></span>
        </button>
        <ul class="dropdown-menu" role="menu">
          <li><?php print MicaClientAnchorHelper::coverageNetwork($network_dto->id) ?></li>
          <li><?php print MicaClientAnchorHelper::networkVariables(NULL, $network_dto->id) ?></li>
        </ul>
      <?php
      else:
        print MicaClientAnchorHelper::networkVariables(NULL, $network_dto->id, array('class' => 'btn btn-primary indent'));
        ?>
      <?php endif; ?>

    </div>
  <?php endif; ?>
</div>

<div class="clearfix"></div>

<article class="bordered-article">

  <section>
    <h2><?php print t('Overview') ?></h2>

    <div class="row">
      <div class="col-lg-6 col-xs-12">

        <table class="table table-striped">
          <tbody>
          <?php if (!empty($network_dto->acronym)): ?>
            <tr>
              <th><?php print t('Acronym') ?></th>
              <td>
                <p><?php print obiba_mica_commons_get_localized_field($network_dto, 'acronym'); ?></p>
              </td>
            </tr>
          <?php endif; ?>

          <?php if (!empty($network_dto->website)): ?>
            <tr>
              <th><?php print t('Website') ?></th>
              <td>
                <p><?php
                  print l(obiba_mica_commons_get_localized_field($network_dto, 'acronym') . ' ' . t('website'),
                    $network_dto->website,
                    array('attributes' => array('target' => '_blank')));
                  ?></p>
              </td>
            </tr>
          <?php endif; ?>

          <?php if (!empty($network_dto->investigators)): ?>
            <tr>
              <th><?php print t('Investigators') ?></th>
              <td>
                <ul class="list-unstyled">
                  <?php foreach ($network_dto->investigators as $key_investigator => $investigator) : ?>
                    <li>
                      <a href="#" data-toggle="modal"
                         data-target="#investigator_<?php print $network_dto->id ?>_<?php print $key_investigator ?>">
                        <?php print $investigator->title; ?>
                        <?php print $investigator->firstName; ?>
                        <?php print $investigator->lastName; ?>
                        <?php if (!empty($investigator->academicLevel)) {
                          print ', ' . $investigator->academicLevel;
                        } ?>
                        (<?php print obiba_mica_commons_get_localized_field($investigator->institution, 'name'); ?>
                        )
                      </a>
                    </li>
                  <?php endforeach; ?>
                </ul>
              </td>
            </tr>
          <?php endif; ?>

          <?php if (!empty($network_dto->contacts)): ?>
            <tr>
              <th><?php print t('Contacts') ?></th>
              <td>
                <ul class="list-unstyled">
                  <?php foreach ($network_dto->contacts as $key_contact => $contact) : ?>
                    <li>
                      <a href="#" data-toggle="modal"
                         data-target="#contact_<?php print $network_dto->id ?>_<?php print $key_contact ?>">
                        <?php print $contact->title; ?>
                        <?php print $contact->firstName; ?>
                        <?php print $contact->lastName; ?>
                        <?php if (!empty($contact->academicLevel)) {
                          print ', ' . $contact->academicLevel;
                        } ?>
                        (<?php print obiba_mica_commons_get_localized_field($contact->institution, 'name'); ?>
                        )
                      </a>
                    </li>
                  <?php endforeach; ?>
                </ul>
              </td>
            </tr>
          <?php endif; ?>

          </tbody>
        </table>
        <?php if (!empty($associated_people_button)): ?>
          <?php print render($associated_people_button); ?>
          <?php if (!empty($associated_people_modal)): ?>
            <?php print render($associated_people_modal); ?>
          <?php endif; ?>
        <?php endif; ?>
      </div>

      <?php if (!empty($network_dto->attributes)): ?>
        <div class="col-lg-6 col-xs-12">
          <h4><?php print t('Attributes') ?></h4>

          <p><?php print obiba_mica_dataset_attributes_tab($network_dto->attributes, 'maelstrom'); ?></p>
        </div>
      <?php endif; ?>
    </div>

  </section>
  <?php if (!empty($attachments)): ?>
    <section>
      <h2><?php print variable_get_value('files_documents_label'); ?></h2>
      <?php print $attachments; ?>
    </section>
  <?php endif; ?>

  <!-- STUDIES and NETWORKS -->
  <?php if (!empty($network_dto->studySummaries) || !empty($network_dto->networkSummaries)): ?>
    <section>

      <div class="row">
        <?php if (!empty($network_dto->studySummaries)): ?>
          <div class="<?php echo(empty($network_dto->networkSummaries) ? 'col-lg-12' : 'col-lg-6') ?> col-xs-12">
            <h2><?php print t('Studies') ?></h2>

            <div id="studies-table">
              <div class="row">
                <div class="col-lg-12 col-xs-12">
                  <table class="table table-striped" id="table-studies"></table>
                </div>
              </div>
            </div>
          </div>
        <?php endif; ?>

        <?php if (!empty($network_dto->networkSummaries)): ?>
          <div class="<?php echo(empty($network_dto->studySummaries) ? 'col-lg-12' : 'col-lg-6') ?> col-xs-12">
            <h2><?php print t('Networks') ?></h2>

            <div id="networks-table">
              <div class="row">
                <div class="col-lg-12 col-xs-12">
                  <table class="table table-striped" id="table-networks"></table>
                </div>
              </div>
            </div>
          </div>
        <?php endif; ?>

      </div>

    </section>
  <?php endif; ?>

  <!-- COVERAGE placeholder -->
  <section id="coverage">
    <div><?php print t('Loading ...') ?></div>
  </section>

  <div><?php !empty($investigators_modal) ? print $investigators_modal : ''; ?></div>
  <div><?php !empty($contacts_modal) ? print $contacts_modal : ''; ?></div>

</article>
