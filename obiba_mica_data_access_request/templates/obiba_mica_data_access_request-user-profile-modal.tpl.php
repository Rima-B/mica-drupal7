<!--
  ~ Copyright (c) 2015 OBiBa. All rights reserved.
  ~
  ~ This program and the accompanying materials
  ~ are made available under the terms of the GNU Public License v3.0.
  ~
  ~ You should have received a copy of the GNU General Public License
  ~ along with this program.  If not, see <http://www.gnu.org/licenses/>.
  -->

<div class="modal fade" id="UserDetailModal" tabindex="-1" role="dialog"
  aria-labelledby="UserDetailModal">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal"
          aria-label="Close"><span
            aria-hidden="true">&times;</span></button>
        <h4 class="modal-title"
          id="UserDetailModalTilte"><?php print t('User Profile'); ?></h4>
      </div>
      <div class="modal-body">
        <div>
          <label for="recipient-name"
            class="control-label"><?php print t('Name : '); ?></label>
          <span id="data-name-applicant"></span>
        </div>

        <div>
          <label for="recipient-email"
            class="control-label"><?php print t('E-mail : '); ?></label>
          <span id="data-email-applicant"></span>
        </div>
        <div id="user-attributes">

        </div>
        <a id="data-email-applicant" class="btn btn-default" href=""
          target="_blank"><?php print t('Send E-mail'); ?></a>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">
          <?php print t('Close'); ?>
        </button>
      </div>
    </div>
  </div>
</div>
