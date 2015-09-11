<?php
//dpm($study_variables_aggs);
//dpm($coverage)
?>
<?php if (!empty($populations)): ?>
  <?php foreach ($populations as $key => $population): ?>
    <?php if (!empty($population['data']) && !empty($population['data']['dce-modal'])): ?>
      <div><?php print $population['data']['dce-modal']; ?></div>
    <?php endif; ?>
  <?php endforeach ?>
<?php endif; ?>
<div>
  <p class="md-top-margin">
    <?php if (!empty($study_dto->logo->id)): ?>
      <a
        href="<?php print obiba_mica_commons_safe_expose_server_url($study_dto->id, $study_dto->logo, 'study', TRUE) ?>"
        class="fancybox-button">
        <img
          src="<?php print obiba_mica_commons_safe_expose_server_url($study_dto->id, $study_dto->logo, 'study') ?>"
          class="imageThumb">
      </a>
    <?php endif; ?>

    <?php print obiba_mica_commons_markdown(obiba_mica_commons_get_localized_field($study_dto, 'objectives')); ?>
  </p>

  <div class="btn-group pull-right md-bottom-margin">
    <?php if (variable_get_value('mica_statistics_coverage')): ?>
      <button id="study-actions" type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown"
        aria-expanded="false">
        <?php print t('Search') ?> <span class="caret"></span>
      </button>
      <ul class="dropdown-menu" role="menu">
        <li><?php print MicaClientAnchorHelper::coverageStudy($study_dto->id) ?></li>

        <?php if (variable_get_value('studies_list_show_search_button')): ?>
          <li><?php print MicaClientAnchorHelper::studyVariables(NULL, $study_dto->id) ?></li>
        <?php endif; ?>
      </ul>
    <?php
    else:
      if (variable_get_value('studies_list_show_search_button')):
        print MicaClientAnchorHelper::studyVariables(NULL, $study_dto->id, TRUE);
      endif;
      ?>
    <?php endif; ?>
  </div>
</div>

<div class="clearfix"></div>

<article>

<section>
<div class="row">
<div class="col-lg-6 col-xs-12 ">
  <!-- GENERAL INFORMATION -->
  <h2 id="overview"><?php print t('Overview') ?></h2>

  <table class="table table-striped">
    <tbody>

    <?php if (!empty($study_dto->acronym)): ?>
      <tr>
        <th><?php print t('Acronym') ?></th>
        <td><p><?php print obiba_mica_commons_get_localized_field($study_dto, 'acronym'); ?></p></td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->website)): ?>
      <tr>
        <th><?php print t('Website') ?></th>
        <td>
          <p><?php
            print l(obiba_mica_commons_get_localized_field($study_dto, 'acronym') . ' ' . t('website'),
              $study_dto->website,
              array('attributes' => array('target' => '_blank')));
            ?></p>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->investigators)): ?>
      <tr>
        <th><?php print t('Investigators') ?></th>
        <td>
          <ul class="list-unstyled">
            <?php foreach ($study_dto->investigators as $key_investigator => $investigator) : ?>
              <li>
                <a href="#" data-toggle="modal"
                  data-target="#investigator_<?php print $study_dto->id ?>_<?php print $key_investigator ?>">
                  <?php print $investigator->title; ?>
                  <?php print $investigator->firstName; ?>
                  <?php print $investigator->lastName; ?>
                  <?php if (!empty($investigator->academicLevel)) {
                    print ', ' . $investigator->academicLevel;
                  } ?>
                  (<?php print obiba_mica_commons_get_localized_field($investigator->institution, 'name'); ?>)
                </a>
              </li>
            <?php endforeach; ?>
          </ul>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->contacts)): ?>
      <tr>
        <th><?php print t('Contacts') ?></th>
        <td>
          <ul class="list-unstyled">
            <?php foreach ($study_dto->contacts as $key_contact => $contact) : ?>
              <li>
                <a href="#" data-toggle="modal"
                  data-target="#contact_<?php print $study_dto->id ?>_<?php print $key_contact ?>">
                  <?php print $contact->title; ?>
                  <?php print $contact->firstName; ?>
                  <?php print $contact->lastName; ?>
                  <?php if (!empty($contact->academicLevel)) {
                    print ', ' . $contact->academicLevel;
                  } ?>
                  (<?php print obiba_mica_commons_get_localized_field($contact->institution, 'name'); ?>)
                </a>
              </li>
            <?php endforeach; ?>
          </ul>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->startYear)): ?>
      <tr>
        <th><?php print t('Study Start Year') ?></th>
        <td><p><?php print $study_dto->startYear; ?></p></td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->endYear)): ?>
      <tr>
        <th><?php print t('Study End Year') ?></th>
        <td><p><?php print $study_dto->endYear; ?></p></td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->networks)): ?>
      <tr>
        <th><?php print t('Networks') ?> :</th>
        <td>
          <p>
            <a href=""><?php //print $study_dto->networks; ?></a>
          </p>
        </td>
      </tr>
    <?php endif; ?>

    </tbody>
  </table>

</div>
<div class="col-lg-6  col-xs-12">
  <!-- GENERAL DESIGN -->
  <h2 id="design"><?php print t('Design') ?></h2>

  <table class="table table-striped">
    <tbody>

    <?php if (!empty($study_dto->methods->designs)): ?>
      <tr>
        <th><?php print t('Study Design') ?></th>
        <td>
          <ul class="list-unstyled">
            <?php foreach ($study_dto->methods->designs as $design): ?>
              <li>
                <?php print obiba_mica_commons_clean_string($design); ?>
                <?php if ($design == 'other'): ?>
                  : <?php print obiba_mica_commons_get_localized_field($study_dto->methods, 'otherDesign'); ?>
                <?php endif; ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->methods->followUpInfo)): ?>
      <tr>
        <th><?php print t('General Information on Follow Up (profile and frequency)') ?></th>
        <td><p><?php print obiba_mica_commons_get_localized_field($study_dto->methods, 'followUpInfo'); ?></p>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->methods->recruitments)): ?>
      <tr>
        <th><?php print t('Recruitment Target') ?></th>
        <td>
          <ul class="list-unstyled">
            <?php foreach ($study_dto->methods->recruitments as $recruitment): ?>
              <li>
                <?php print obiba_mica_commons_clean_string($recruitment) ?>
                <?php if ($recruitment == 'other'): ?>
                  : <?php print obiba_mica_commons_get_localized_field($study_dto->methods, 'otherRecruitment'); ?>
                <?php endif; ?>
              </li>
            <?php endforeach; ?>
          </ul>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->numberOfParticipants->participant->number)): ?>
      <tr>
        <th><?php print t('Target number of participants') ?></th>
        <td>
          <p>
            <?php print obiba_mica_commons_format_number($study_dto->numberOfParticipants->participant->number); ?>
            <?php if (!empty($study_dto->numberOfParticipants->participant->noLimit)): ?>
              (<?php print t('No limit'); ?>)
            <?php endif; ?>
          </p>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->numberOfParticipants->sample->number)): ?>
      <tr>
        <th><?php print t('Target number of participants with biological samples') ?></th>
        <td>
          <p>
            <?php print obiba_mica_commons_format_number($study_dto->numberOfParticipants->sample->number); ?>
            <?php if (!empty($study_dto->numberOfParticipants->sample->noLimit)): ?>
              (<?php print t('No limit'); ?>)
            <?php endif; ?>
          </p>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->numberOfParticipants->info)): ?>
      <tr>
        <th><?php print t('Supplementary information about target number of participants') ?></th>
        <td>
          <p><?php print obiba_mica_commons_get_localized_field($study_dto->numberOfParticipants, 'info'); ?></p>
        </td>
      </tr>
    <?php endif; ?>

    <?php if (!empty($study_dto->methods->info)): ?>
      <tr>
        <th><?php print t('Supplementary information') ?></th>
        <td><p><?php print obiba_mica_commons_get_localized_field($study_dto->methods, 'info'); ?></p></td>
      </tr>
    <?php endif; ?>
    </tbody>
  </table>

</div>
</div>

</section>

<section>

  <div class="row">
    <div class="col-lg-6 col-xs-12">
      <!-- ACCESS -->
      <h2 id="access"><?php print t('Access') ?></h2>

      <?php if (!empty($study_dto->access) && (in_array('data', $study_dto->access) ||
          in_array('bio_samples', $study_dto->access) ||
          in_array('other', $study_dto->access))
      ): ?>

        <p><?php print t('Access to external researchers or third parties provided or foreseen for:'); ?></p>
      <?php else : ?>
        <p><?php print t('Access to external researchers or third parties neither provided nor foreseen.'); ?></p>
      <?php endif; ?>
      <div class="table-responsive">
        <table class="table table-striped valign-table-column">
          <tbody>
          <tr>
            <th><?php print t('Data (questionnaire-derived, measured...)'); ?></th>
            <td>
              <p>
                <?php if (in_array('data', $study_dto->access)): ?>
                  <span class="glyphicon glyphicon-ok"></span>
                <?php else : ?>
                  <span class="glyphicon glyphicon-remove"></span>
                <?php endif; ?>
              </p>
            </td>
          </tr>

          <tr>
            <th><?php print t('Biological samples'); ?></th>
            <td>
              <p>
                <?php if (in_array('bio_samples', $study_dto->access)): ?>
                  <span class="glyphicon glyphicon-ok"></span>
                <?php else : ?>
                  <span class="glyphicon glyphicon-remove"></span>
                <?php endif; ?>
              </p>
            </td>
          </tr>

          <?php if (in_array('other', $study_dto->access)): ?>
            <tr>
              <th><?php print t('Other'); ?></th>
              <td>
                <?php if (in_array('other', $study_dto->access)): ?>
                  <span class="glyphicon glyphicon-ok right-indent"></span>
                <?php endif; ?>
                <?php if (!empty($study_dto->otherAccess)): ?>
                  <?php print obiba_mica_commons_get_localized_field($study_dto, 'otherAccess'); ?>
                <?php endif; ?>
              </td>
            </tr>
          <?php endif; ?>
          </tbody>
        </table>
      </div>
    </div>
    <div class="col-lg-6 col-xs-12">
      <!-- MARKER PAPER -->
      <?php if (!empty($study_dto->markerPaper) || !empty($study_dto->pubmedId)): ?>
        <h2 id="marker"><?php print t('Marker Paper') ?></h2>
        <?php if (!empty($study_dto->markerPaper)): ?>
          <p><?php print $study_dto->markerPaper; ?></p>
        <?php endif; ?>
        <?php if (!empty($study_dto->pubmedId)): ?>
          <p>
            <a href="http://www.ncbi.nlm.nih.gov/pubmed/<?php print $study_dto->pubmedId; ?>" target="_blank">
              PUBMED <?php print $study_dto->pubmedId; ?>
            </a>
          </p>
        <?php endif; ?>
      <?php endif; ?>
    </div>
  </div>

</section>

<!-- SUPPLEMENTARY INFORMATION -->
<?php if (!empty($study_dto->info)): ?>
  <section>
    <h2 id="info"><?php print t('Supplementary Information'); ?></h2>

    <p><?php print obiba_mica_commons_get_localized_field($study_dto, 'info'); ?></p>
  </section>
<?php endif; ?>

<!-- TIMELINE -->
<?php if (!empty($timeline)): ?>
  <section>
    <h2 id="timeline"><?php print t('Timeline'); ?></h2>

    <p class="help-block">
      <?php print t('Each colour in the timeline graph below represents a separate Study Population, while each segment in the graph represents a separate Data Collection Event. Clicking on a segment gives more detailed information on a Data Collection Event.') ?>
    </p>

    <div class="scroll-content-tab">
      <?php print $timeline; ?>
    </div>
  </section>
<?php endif; ?>

<!-- POPULATIONS -->
<?php if (!empty($populations)): ?>
  <section>
    <h2 id="populations"><?php if (count($populations) > 1) {
        print t('Populations');
      }
      else print t('Population') ?></h2>
    <?php if (count($populations) == 1): ?>
      <?php print array_pop($populations)['html']; ?>
    <?php else: ?>

      <div class="row tabbable tabs-left">
        <div class="col-lg-2 col-xs-12  ">
          <ul class="nav nav-pills nav-stacked">
            <?php foreach ($populations as $key => $population): ?>
              <li <?php if ($key == array_keys($populations)[0]) {
                print 'class="active"';
              } ?>>
                <a href="#population-<?php print $key; ?>" data-toggle="pill">
                  <?php print obiba_mica_commons_get_localized_field($population['data'], 'name'); ?>
                </a>
              </li>

            <?php endforeach ?>
          </ul>
        </div>
        <div class="col-lg-10 col-xs-12  ">
          <div class="tab-content indent">
            <?php foreach ($populations as $key => $population): ?>
              <div class="tab-pane  <?php if ($key == array_keys($populations)[0]) {
                print 'active';
              } ?>"
                id="population-<?php print $key; ?>">
                <?php print $population['html']; ?>
              </div>
            <?php endforeach ?>
          </div>
        </div>
      </div>

    <?php endif ?>
  </section>
<?php endif; ?>

<!-- DOCUMENTS -->
<?php if (!empty($study_dto->attachments)): ?>
  <section>
    <h2 id="documents"><?php print t('Documents'); ?></h2>

    <div>
      <?php if (!empty($study_attachements)): ?>
        <ul class="list-group">
          <?php print $study_attachements; ?>
        </ul>
      <?php endif; ?>
    </div>
  </section>
<?php endif; ?>

<!-- NETWORKS placeholder -->
<section id="networks">
  <div><?php print t('Loading ...') ?></div>
</section>

<!-- DATASETS placeholder -->
<section id="datasets">
  <div><?php print t('Loading ...') ?></div>
</section>

<!-- VARIABLES -->
<?php if (!empty($study_variables_aggs)): ?>
  <section id="variables">
    <div class="container-fluid">
      <div class="row">
        <div class="col-xs-3 right-indent">
          <h2 id="variables"><?php print t('Variables') ?></h2>

          <h4><?php print t('Number of variables') ?></h4>

          <p>
            <?php print $study_variables_aggs['total_hits']; ?>
          </p>

          <?php
          // TODO currently the code that feeds the data is commented, see obiba_mica_study-page-detail.inc
          print MicaClientAnchorHelper::ajaxFriendlyAnchor(
            MicaClientPathProvider::SEARCH,
            t('Search Variables'),
            array('class' => 'btn btn-primary'),
            array(
              'type' => 'variables',
              'parent:id[]' => 'id.' . $study_dto->id
            )
          );
          ?>
        </div>
      </div>
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
<div class="back-to-top t_badge"><i class="glyphicon glyphicon-arrow-up"></i></div>