<?php
$dce_name = obiba_mica_commons_get_localized_field($dce, 'name');
?>

<div id="dce-<?php print $dce_id_target ?>" class="modal fade" xmlns="http://www.w3.org/1999/html">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
        <h3 class="modal-title"><?php print $dce_name ?></h3>
      </div>
      <div class="modal-body">
        <section>
          <div>
            <?php if (!empty($dce->description)): ?>
              <p>
                <?php print obiba_mica_commons_get_localized_field($dce, 'description'); ?>
              </p>
            <?php endif; ?>
            <div class="btn-group pull-right md-bottom-margin dce-actions" style="display: none;"  data-dce-name="<?php print $dce_name ?>">
              <?php if (variable_get_value('mica_statistics_coverage')): ?>
                <button id="study-actions" type="button" class="btn btn-primary dropdown-toggle" data-toggle="dropdown"
                        aria-expanded="false">
                  <?php print t('Search') ?> <span class="caret"></span>
                </button>
                <ul class="dropdown-menu" role="menu">
                  <li><?php print MicaClientAnchorHelper::coverageDceStudy($dce_uid) ?></li>
                  <li><?php print MicaClientAnchorHelper::dceStudyVariables(NULL, $dce_uid) ?></li>
                </ul>
              <?php else:
                print MicaClientAnchorHelper::dceStudyVariables(NULL, $dce_uid, true);
                ?>
              <?php endif; ?>
            </div>

          </div>

          <div class="clearfix"></div>

          <table class="table table-striped">
            <tbody>
            <?php if (!empty($dce->startYear)): ?>
              <tr>
                <th><?php print t('Start Year') ?></th>
                <td><p><?php print obiba_mica_commons_format_year($dce->startYear, $dce->startMonth); ?></p></td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($dce->endYear)): ?>
              <tr>
                <th><?php print t('End Year') ?></th>
                <td><p><?php print obiba_mica_commons_format_year($dce->endYear, $dce->endMonth); ?></p></td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($dce->dataSources)): ?>
              <tr>
                <th><?php print t('Data Sources') ?></th>
                <td>
                  <ul>
                    <?php foreach ($dce->dataSources as $dataSource): ?>
                      <li>
                        <?php print obiba_mica_commons_clean_string($dataSource); ?>
                      </li>
                    <?php endforeach; ?>
                    <?php if (!empty($dce->otherDataSources)): ?>
                      <li>
                        <?php print obiba_mica_commons_get_localized_field($dce, 'otherDataSources'); ?>
                      </li>
                    <?php endif; ?>
                  </ul>
                </td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($dce->administrativeDatabases)): ?>
              <tr>
                <th><?php print t('Administrative Databases') ?></th>
                <td>
                  <ul>
                    <?php foreach ($dce->administrativeDatabases as $database): ?>
                      <li>
                        <?php print obiba_mica_commons_clean_string($database); ?>
                      </li>
                    <?php endforeach; ?>
                  </ul>
                </td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($dce->bioSamples)): ?>
              <tr>
                <th><?php print t('Biological Samples') ?></th>
                <td>
                  <ul>
                    <?php foreach ($dce->bioSamples as $samples): ?>
                      <li>
                        <?php print obiba_mica_commons_clean_string($samples); ?>
                      </li>
                    <?php endforeach; ?>
                    <?php if (!empty($dce->otherBioSamples)): ?>
                      <li>
                        <?php print obiba_mica_commons_get_localized_field($dce, 'otherBioSamples'); ?>
                      </li>
                    <?php endif; ?>
                  </ul>
                </td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($dce->tissueTypes)): ?>
              <tr>
                <th><?php print t('Tissue Types') ?></th>
                <td><p><?php print obiba_mica_commons_get_localized_field($dce, 'tissueTypes'); ?></p></td>
              </tr>
            <?php endif; ?>

            <?php if (!empty($attachements)): ?>
              <tr>
                <th><?php print t('Documents') ?></th>
                <td>
                  <ul class="list-group">
                    <?php print $attachements; ?>
                  </ul>
                </td>
              </tr>
            <?php endif; ?>

            </tbody>
          </table>

        </section>
      </div>
    </div>
  </div>
</div>
