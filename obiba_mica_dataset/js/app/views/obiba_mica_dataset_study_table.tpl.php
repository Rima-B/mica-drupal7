<span ng-init="info = extractStudySummaryInfo(contingency.studyTable)" title="{{info.population + ':' + info.dce}}">
  <a href="<?php print base_path(); ?>mica/study/{{contingency.studyTable.studyId}}">{{contingency.studyTable.studySummary.acronym | localizedValue}}</a>
  <span>
  {{contingency.studyTable.name | localizedValue}}
  </span>
</span>
