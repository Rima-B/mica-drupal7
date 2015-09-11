<?php

/**
 * @file
 * class tools provided by Mica client
 */

class ObibaDatasetConstants {
  const FACET_TYPE_HIDE = 0;
  const FACET_TYPE_AUTOCOMPLETE = 1;
  const FACET_TYPE_CHECKBOX = 2;

  const  UNDETERMINED_HARMO_ICON = 'glyphicon-question-sign';
  const  COMPLETE_HARMO_ICON = 'glyphicon-ok';
  const  INCOMPLETE_HARMO_ICON = 'glyphicon-minus';
  const  IMPOSSIBLE_HARMO_ICON = 'glyphicon-remove';

  /**
   * Set the harmonization status icon.
   */
  public static function getIcon() {
    $impossible_icon = variable_get_value('dataset_harmonization_impossible_icon');
    $color_icon = (!empty($impossible_icon) && $impossible_icon != ObibaDatasetConstants::IMPOSSIBLE_HARMO_ICON) ?
      ' alert-black' : ' alert-danger';
    return !empty($impossible_icon) ? ($impossible_icon . $color_icon) : (ObibaDatasetConstants::IMPOSSIBLE_HARMO_ICON . $color_icon);
  }
}

class ObibaFacetSearchBlockConstants {
  const EXPAND_GROUPS = 'expand_groups';
  const EXPAND_ALL = 'expand_all';
}
