<?php // dpm($aggregation_facet);?>
<?php // dpm((((($term->count*200)/$totalhits))*100)/200);?>
<li class="facets"
  data-form-id="facet-search-<?php print $aggregation_facet ?>">
  <table>
    <tr>
      <td>
        <?php
        // $mica_client = new MicaClient();
        if ($aggregation_facet == 'populations-selectionCriteria-countriesIso') {
          $title = t(countries_country_lookup(drupal_strtoupper($term->title), 'iso' . strlen($term->title))->name);
          if (empty($title)) {
            $title = t($term->title);
          }
        }
        else {
          if (!empty($term->key)) {
            if (!empty($term->title)) {
              $title = t($term->title);
            }
            else {
              $title = $term->key;
            }
          }
        }
        $tooltip = empty($term->description) ? (strlen($title) < 30 ? '' : $title) : $term->description;
        ?>
        <span id="checkthebox"
          class="term-field <?php print $type_string . $aggregation_facet; ?> unchecked"
          aggregation="<?php print $type_string . $aggregation_facet . '[]'; ?>"
          value="<?php print  htmlspecialchars($title); ?>"
          data-value="<?php print  $term->key; ?>"
          data-toggle="tooltip"
          title="<?php print htmlspecialchars($tooltip) ?>">
          <i class="glyphicon glyphicon-unchecked"></i>
          <div class="tilte-term"><?php print $title ?></div>
        </span>
      </td>
      <?php if ($query_request && ($_SESSION['request-search-response'] == 'no-empty') && $term->count != 0) : ?>
        <td class='term-count'>
          <span data-toggle="tooltip" data-placement="top" title="Filtered">
            <?php print obiba_mica_commons_format_number($term->count); ?>
          </span>
        </td>
      <?php endif; ?>
      <?php if (!$query_request): ?>
        <td class='term-count'>
          <span data-toggle="tooltip" data-placement="top"
            title="All"><?php print obiba_mica_commons_format_number($term->default); ?></span>
        </td>
      <?php elseif (($term->count != $term->default) || ($_SESSION['request-search-response'] == 'empty')) : ?>
        <td class='term-default'>
          <span data-toggle="tooltip" data-placement="top" title="All">
            <?php print obiba_mica_commons_format_number($term->default); ?>
          </span>
        </td>
      <?php endif; ?>
    </tr>
  </table>
</li>
<input
  id="<?php print $type_string . $aggregation_facet . '[]-' . $term->key; ?>"
  name="<?php print $type_string . 'terms:' . $aggregation_facet . '[]'; ?>"
  type="hidden" value="">
