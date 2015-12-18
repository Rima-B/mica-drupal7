<?php

/**
 * @file
 * Query build code.
 */

abstract class AbstractDtoBuilder {
  protected $dto = NULL;

  /**
   * Abstraction of the Build() method.
   */
  public abstract function build();
}

class RangeConditionDtoBuilder extends AbstractDtoBuilder {
  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\RangeConditionDto();
  }

  /**
   * Instantiate the RangeConditionDtoBuilder().
   *
   * @return object
   *   RangeConditionDtoBuilder instance.
   */
  public static function newBuilder() {
    return new RangeConditionDtoBuilder();
  }

  /**
   * Set the less then option to the value.
   *
   * @return object
   *   The current query Dto object.
   */
  public function lt($value) {
    $this->dto->setOp(\obiba\mica\RangeConditionDto\Operator::LT);
    $this->dto->setValue($value);
    return $this;
  }

  /**
   * Set the less then equal option to the value.
   *
   * @return object
   *   The current query Dto object.
   */
  public function lte($value) {
    $this->dto->setOp(\obiba\mica\RangeConditionDto\Operator::LTE);
    $this->dto->setValue($value);
    return $this;
  }

  /**
   * Set the greater then option to the value.
   *
   * @return object
   *   The current query Dto object.
   */
  public function gt($value) {
    $this->dto->setOp(\obiba\mica\RangeConditionDto\Operator::GT);
    $this->dto->setValue($value);
    return $this;
  }

  /**
   * Set the greater then equal option to the value.
   *
   * @return object
   *   The current query Dto object.
   */
  public function gte($value) {
    $this->dto->setOp(\obiba\mica\RangeConditionDto\Operator::GTE);
    $this->dto->setValue($value);
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

abstract class FieldFilterQueryDtoBuilder extends AbstractDtoBuilder {
  private $filter = NULL;

  /**
   * Instance initialisation.
   */
  protected function __construct() {
    $this->dto = new \obiba\mica\FilteredQueryDto();
    $this->filter = new \obiba\mica\FieldFilterQueryDto();
    $this->dto->setExtension('obiba.mica.FieldFilterQueryDto.filter', $this->filter);
  }

  /**
   * Set extension.
   *
   * @param string $key
   *   The key Dto.
   * @param object $extension
   *   The Dto object query
   *
   * @return object
   *   The current query Dto object.
   */
  protected function extension($key, $extension) {
    $this->filter->setExtension($key, $extension);
    return $this;
  }

  /**
   * Set field.
   *
   * @param string $field
   *   The field to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function field($field) {
    $this->filter->setField($field);
    return $this;
  }

  /**
   * The builder filter.
   *
   * @return object
   *   The current query Dto object.
   */
  public function filter() {
    return $this->filter;
  }

}

class RangeFieldFilterQueryDtoBuilder extends FieldFilterQueryDtoBuilder {
  private $range = NULL;

  /**
   * Instance initialisation.
   */
  protected function __construct() {
    parent::__construct();
    $this->range = new \obiba\mica\RangeFilterQueryDto();
    $this->extension("obiba.mica.RangeFilterQueryDto.range", $this->range);
  }

  /**
   * Instantiate the RangeFieldFilterQueryDtoBuilder().
   *
   * @return object
   *   RangeFieldFilterQueryDtoBuilder instance.
   */
  public static function newBuilder() {
    return new RangeFieldFilterQueryDtoBuilder();
  }

  /**
   * Set the from value.
   *
   * @param \obiba\mica\RangeConditionDto $dto
   *   The Dto from value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function from(\obiba\mica\RangeConditionDto $dto) {
    $this->range->setFrom($dto);
    return $this;
  }

  /**
   * Set the fromBuilder value.
   *
   * @param RangeConditionDtoBuilder $builder
   *   The Dto from value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function fromBuilder(RangeConditionDtoBuilder $builder) {
    $this->range->setFrom($builder->build());
    return $this;
  }

  /**
   * Set the to value.
   *
   * @param \obiba\mica\RangeConditionDto $dto
   *   The Dto <to> value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function to(\obiba\mica\RangeConditionDto $dto) {
    $this->range->setTo($dto);
    return $this;
  }

  /**
   * Set the toBuilder value.
   *
   * @param RangeConditionDtoBuilder $builder
   *   The Dto <to> value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function toBuilder(RangeConditionDtoBuilder $builder) {
    $this->range->setTo($builder->build());
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }

}

class TermsFieldFilterQueryDtoBuilder extends FieldFilterQueryDtoBuilder {
  private $terms = NULL;

  /**
   * Instance initialisation.
   */
  protected function __construct() {
    parent::__construct();
    $this->terms = new \obiba\mica\TermsFilterQueryDto();
    $this->extension("obiba.mica.TermsFilterQueryDto.terms", $this->terms);
  }

  /**
   * Instantiate the TermsFieldFilterQueryDtoBuilder().
   *
   * @return object
   *   TermsFieldFilterQueryDtoBuilder instance.
   */
  public static function newBuilder() {
    return new TermsFieldFilterQueryDtoBuilder();
  }

  /**
   * Add a value.
   *
   * @param string $value
   *   The value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function value($value) {
    $this->terms->addValues($value);
    return $this;
  }

  /**
   * Add a set of values array.
   *
   * @param array $values
   *   The values to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function values(array $values) {
    if (!empty($values)) {
      foreach ($values as $value) {
        $this->terms->addValues($value);
      }
    }
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class BoolFilterQueryDtoBuilder extends AbstractDtoBuilder {
  private $filter = NULL;

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\FilteredQueryDto();
    $this->filter = new \obiba\mica\BoolFilterQueryDto();
    $this->dto->setExtension('obiba.mica.BoolFilterQueryDto.filter', $this->filter);
  }

  /**
   * Instantiate the BoolFilterQueryDtoBuilder().
   *
   * @return object
   *   BoolFilterQueryDtoBuilder instance.
   */
  public static function newBuilder() {
    return new BoolFilterQueryDtoBuilder();
  }

  /**
   * Set the <op> value.
   *
   * @param string $value
   *   The <op> value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function operator($value) {
    $this->filter->setOp($value);
    return $this;
  }

  /**
   * Add the filtered queries.
   *
   * @param array $values
   *   The set of FilteredQueryDto to add.
   */
  public function filteredQueries(array $values) {
    foreach ($values as $value) {
      $this->filter->addFilteredQuery($value);
    }
  }

  /**
   * The filter builder queries.
   *
   * @param array $values
   *   The set of FilteredQueryDto to add.
   */
  public function filteredQueriesBuilders(array $values) {
    foreach ($values as $value) {
      $this->filter->addFilteredQuery($value->build);
    }
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class FieldStatementDtoBuilder extends AbstractDtoBuilder {

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\FieldStatementDto();
  }

  /**
   * Instantiate the FieldStatementDtoBuilder().
   *
   * @return object
   *   FieldStatementDtoBuilder instance.
   */
  public static function newBuilder() {
    return new FieldStatementDtoBuilder();
  }

  /**
   * Map the a given operator.
   *
   * @param string $value
   *   The of operator to map
   *
   * @return int
   *   The key of the operator on the Dto definition.
   */
  public static function mapOp($value) {
    switch (strtolower($value)) {
      case 'or':
        return \obiba\mica\FieldStatementDto\Operator::_OR;

      case 'and':
        return \obiba\mica\FieldStatementDto\Operator::_AND;

      case 'not':
        return \obiba\mica\FieldStatementDto\Operator::_AND_NOT;
    }

    return NULL;
  }

  /**
   * Set field.
   *
   * @param \obiba\mica\FieldFilterQueryDto $value
   *   The field to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function field(\obiba\mica\FieldFilterQueryDto $value) {
    $this->dto->setField($value);
    return $this;
  }

  /**
   * Set field builder.
   *
   * @param FieldFilterQueryDtoBuilder $value
   *   The field builder to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function fieldBuilder(FieldFilterQueryDtoBuilder $value) {
    $this->dto->setField($value->build());
    return $this;
  }

  /**
   * Set the <op> value.
   *
   * @param string $value
   *   The <op> value to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function op($value) {
    $this->dto->setOp(self::mapOp($value));
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class LogicalFilterQueryDtoBuilder extends AbstractDtoBuilder {
  private $filter = NULL;

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\FilteredQueryDto();
    $this->filter = new \obiba\mica\LogicalFilterQueryDto();
    $this->dto->setExtension('obiba.mica.LogicalFilterQueryDto.filter', $this->filter);
  }

  /**
   * Instantiate the LogicalFilterQueryDtoBuilder().
   *
   * @return object
   *   LogicalFilterQueryDtoBuilder instance.
   */
  public static function newBuilder() {
    return new LogicalFilterQueryDtoBuilder();
  }

  /**
   * Set field.
   *
   * @param \obiba\mica\FieldFilterQueryDto $value
   *   The field to set.
   * @param string $op
   *   The operator.
   *
   * @return object
   *   The current query Dto object.
   */
  public function field(\obiba\mica\FieldFilterQueryDto $value, $op) {
    $field_statement_dto = FieldStatementDtoBuilder::newBuilder()
      ->field($value)
      ->op($op)
      ->build();
    $this->filter->addFields($field_statement_dto);
    return $this;
  }

  /**
   * Set field builder.
   *
   * @param FieldFilterQueryDtoBuilder $value
   *   The field builder to set.
   * @param string $op
   *   The operator.
   *
   * @return object
   *   The current query Dto object.
   */
  public function fieldBuilder(FieldFilterQueryDtoBuilder $value, $op) {
    $this->field($value->filter(), $op);
    return $this;
  }

  /**
   * Check if has a field.
   *
   * @return bool
   *   TRUE/FALSE.
   */
  public function hasFields() {
    return $this->filter->hasFields();
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}


class FilteredQueryDtoBuilder extends AbstractDtoBuilder {

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\FilteredQueryDto();
  }

  /**
   * Instantiate the FilteredQueryDtoBuilder().
   *
   * @return object
   *   FilteredQueryDtoBuilder instance.
   */
  public static function newBuilder() {
    return new FilteredQueryDtoBuilder();
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class SortDtoBuilder extends AbstractDtoBuilder {

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\QueryDto\SortDto();
  }

  /**
   * Instantiate the SortDtoBuilder().
   *
   * @return object
   *   SortDtoBuilder instance.
   */
  public static function newBuilder() {
    return new SortDtoBuilder();
  }

  /**
   * Set field.
   *
   * @param string $field
   *   The field to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function field($field) {
    $this->dto->setField($field);
    return $this;
  }

  /**
   * Set an asc order.
   *
   * @return object
   *   The current query Dto object.
   */
  public function orderAsc() {
    $this->dto->setOrder(\obiba\mica\QueryDto\SortDto\Order::ASC);
    return $this;
  }

  /**
   * Set an desc order.
   *
   * @return object
   *   The current query Dto object.
   */
  public function orderDesc() {
    $this->dto->setOrder(\obiba\mica\QueryDto\SortDto\Order::DESC);
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}


class QueryStringDtoBuilder extends AbstractDtoBuilder {

  /**
   * Instance initialisation.
   */
  private function __construct() {
    $this->dto = new \obiba\mica\QueryDto\QueryStringDto();
  }

  /**
   * Instantiate the QueryStringDtoBuilder().
   *
   * @return object
   *   QueryStringDtoBuilder instance.
   */
  public static function newBuilder() {
    return new QueryStringDtoBuilder();
  }

  /**
   * Set the query value.
   *
   * @param string $query
   *   The query to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function query($query) {
    $this->dto->setQuery($query);
    return $this;
  }

  /**
   * Set field.
   *
   * @param array $fields
   *   The fields to set.
   *
   * @return object
   *   The current query Dto object.
   */
  public function fields(array $fields) {
    foreach ($fields as $field) {
      $this->dto->addFields($field);
    }
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class QueryDtoBuilder extends AbstractDtoBuilder {

  /**
   * Instance initialisation.
   *
   * @param int $from
   *   The from value.
   * @param int $size
   *   The size value.
   * @param object $sort_field
   *   The field to sort by.
   * @param string $matches
   *   The query.
   * @param string $group_by
   *   The grouped by field.
   */
  private function __construct($from, $size, $sort_field, $matches, $group_by) {
    $this->dto = new \obiba\mica\QueryDto();
    $this->dto->setFrom($from);
    $this->dto->setSize($size);

    if (!empty($matches)) {
      $this->dto->setQueryString(QueryStringDtoBuilder::newBuilder()
        ->query($matches)
        ->build());
    }

    if (!empty($sort_field)) {
      $this->dto->setSort(SortDtoBuilder::newBuilder()
        ->field($sort_field)
        ->build());
    }

    if (!empty($group_by)) {
      if (is_array($group_by)) {
        $this->dto->setAggsBy($group_by);
      }
      else {
        $this->dto->addAggsBy($group_by);
      }
    }
  }

  /**
   * Instantiate the QueryDtoBuilder().
   *
   * @param int $from
   *   The from value.
   * @param int $size
   *   The size value.
   * @param object $sort_field
   *   The field to sort by.
   * @param string $matches
   *   The query'
   * @param string $group_by
   *   The grouped by field.
   *
   * @return object
   *   QueryDtoBuilder instance.
   */
  public static function newBuilder($from, $size, $sort_field, $matches, $group_by) {
    return new QueryDtoBuilder($from, $size, $sort_field, $matches, $group_by);
  }

  /**
   * Set the filtered query.
   *
   * @param \obiba\mica\FilteredQueryDto $dto
   *   The Dto filter.
   *
   * @return object
   *   The current query Dto object.
   */
  public function filteredQuery(\obiba\mica\FilteredQueryDto $dto) {
    $this->dto->setFilteredQuery($dto);
    return $this;
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }
}

class JoinQueryDtoBuilder extends AbstractDtoBuilder {
  const VARIABLES = 'variables';
  const DATASETS = 'datasets';
  const STUDIES = 'studies';
  const NETWORKS = 'networks';
  const MATCHES = 'matches';

  const OR_OPERATOR = 'or';
  const AND_OPERATOR = 'and';

  private $mactehsMap = NULL;
  private $builderMap = NULL;
  private $defaultSortMap = NULL;

  /**
   * Instance initialisation.
   *
   * @param object $data
   *   The query formatted in an array.
   * @param int $default_size
   *   The default size value.
   * @param string $locale
   *   The local language site.
   * @param bool $with_facets
   *   TRUE/FALSE need with facet?
   */
  private function __construct($data, $default_size, $locale, $with_facets = TRUE) {
    $this->dto = new \obiba\mica\JoinQueryDto();
    $this->dto->setLocale($locale);
    $this->dto->setWithFacets($with_facets);
    $this->builderMap = array();
    $this->defaultSize = $default_size;
    $this->initializeDefaultSortMap();
    $this->queryTypes = array(
      self::VARIABLES,
      self::DATASETS,
      self::STUDIES,
      self::NETWORKS,
    );
    if (!empty($data)) {
      $this->parseData($data);
    }
  }

  /**
   * Instantiate the JoinQueryDtoBuilder().
   *
   * @param array $data
   *   The query formatted in an array.
   * @param int $default_size
   *   The default size value.
   * @param string $locale
   *   The local language site.
   * @param bool $with_facets
   *   TRUE/FALSE need with facet?
   *
   * @return object
   *   JoinQueryDtoBuilder instance.
   */
  public static function newBuilder($data, $default_size, $locale, $with_facets = TRUE) {
    return new JoinQueryDtoBuilder($data, $default_size, $locale, $with_facets);
  }

  /**
   * Document query construction.
   *
   * @param string $type
   *   The query type.
   * @param int $from
   *   The from value.
   * @param int $size
   *   The size value.
   * @param string $group_by
   *   The grouped by field.
   *
   * @return object
   *   The current query Dto object.
   */
  public function documentQuery($type, $from, $size, $group_by = NULL) {
    $matches = empty($this->mactehsMap[$type]) ? NULL : $this->mactehsMap[$type];
    $default_sort_field = $this->defaultSortMap[$type];
    $this->setQuery($type, array_key_exists($type, $this->builderMap) ?
      $this->createQuery($this->builderMap[$type], $from, $size, $default_sort_field, $matches, $group_by) :
      QueryDtoBuilder::newBuilder($from, $size, $default_sort_field, $matches, $group_by)
        ->build()
    );

    $query_types = array_filter(
      $this->queryTypes,
      function ($query_type) use ($type) {
      return $query_type !== $type;
      }
    );

    foreach ($query_types as $query_type) {
      $matches = empty($this->mactehsMap[$query_type]) ? NULL : $this->mactehsMap[$query_type];
      $builders = empty($this->builderMap[$query_type]) ? NULL : $this->builderMap[$query_type];
      $default_sort_field = $this->defaultSortMap[$query_type];

      if (!empty($matches) || !empty($builders)) {
        $this->setQuery($query_type, empty($builders) ?
          QueryDtoBuilder::newBuilder(0, $this->defaultSize, $default_sort_field, $matches, $group_by)
            ->build() :
          $this->createQuery($builders, 0, $this->defaultSize, $default_sort_field, $matches, $group_by)
        );
      }
    }

    return $this;
  }

  /**
   * Set the query.
   *
   * @param string $type
   *   The type of the query.
   * @param object $query
   *   The Dto query.
   */
  private function setQuery($type, $query) {
    switch ($type) {
      case self::VARIABLES:
        $this->dto->setVariableQueryDto($query);
        break;

      case self::DATASETS:
        $this->dto->setDatasetQueryDto($query);
        break;

      case self::STUDIES:
        $this->dto->setStudyQueryDto($query);
        break;

      case self::NETWORKS:
        $this->dto->setNetworkQueryDto($query);
        break;
    }
  }

  /**
   * Build the Dto query object.
   *
   * @return object
   *   The current query Dto object.
   */
  public function build() {
    return $this->dto;
  }

  /**
   * Expect to have an array of normalized URL param=value.
   *
   * @param object $data
   *   Query data to parse.
   */
  private function parseData($data) {

    foreach ($data as $type => $search_data) {
      foreach ($search_data as $search_part_type => $parts) {

        if ($search_part_type === self::MATCHES) {
          $this->mactehsMap[$type] = $parts;
          continue;
        }

        $logical_filter_builder = $this->getLogicalFilterBuilder($type);

        foreach ($parts as $agg_name => $agg) {
          if ($this->processException($logical_filter_builder, $type, $agg_name, $agg, $search_part_type)) {
            continue;
          }
          $field_filter_query_dto_builder = $this->getAggregationBuilder($search_part_type, $agg_name, $agg);
          if (!empty($field_filter_query_dto_builder) && !empty($agg->op)) {
            $logical_filter_builder->fieldBuilder(
              $field_filter_query_dto_builder,
              $agg->op
            );
          }
          else {
            $this->fireAnErrorQueryBuilder();
          }
        }
      }

      $this->removeEmptyLogicalFilterBuilder($type);
    }
  }

  /**
   * Process the exception.
   *
   * @param object $logical_filter_builder
   *   The logical query builder.
   * @param string $type
   *   The type of the query.
   * @param string $agg_name
   *   The aggregation name.
   * @param object $agg
   *   The aggregations.
   * @param string $search_part_type
   *   The search part type.
   *
   * @return array
   *   TRUE/FALSE processing errors
   */
  private function processException($logical_filter_builder, $type, $agg_name, $agg, $search_part_type) {
    if (self::NETWORKS === $type && "networkId" === $agg_name) {
      $logical_filter_builder->fieldBuilder(
        $this->getAggregationBuilder($search_part_type, "id", $agg),
        $agg->op
      );
      $this->builderMap[self::NETWORKS] = $logical_filter_builder;
      return TRUE;
    }
    else {
      if (self::STUDIES === $type && "studyIds" === $agg_name) {
        $logical_filter_builder->fieldBuilder(
          $this->getAggregationBuilder($search_part_type, "id", $agg),
          $agg->op
        );
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Get normalized aggregation name.
   *
   * @param string $agg_name
   *   The aggregation name.
   *
   * @return string
   *   The normalized aggregation name.
   */
  private function getNormalizedAggName($agg_name) {
    return preg_replace('/-/', '.', $agg_name);
  }

  /**
   * Get logical filter builder.
   *
   * @param string $type
   *   The query type.
   *
   * @return object
   *   LogicalFilterQueryDtoBuilder instance.
   */
  private function getLogicalFilterBuilder($type) {
    $builder = NULL;

    if (array_key_exists($type, $this->builderMap)) {
      $builder = $this->builderMap[$type];
    }

    if (empty($builder)) {
      $builder = LogicalFilterQueryDtoBuilder::newBuilder();
      $this->builderMap[$type] = $builder;
    }

    return $builder;
  }

  /**
   * Removes the builder that has no fields from the builders map.
   *
   * @param $type string
   *   The query type.
   */
  private function removeEmptyLogicalFilterBuilder($type) {
    $builder = $this->builderMap[$type];

    if (!empty($builder) && !$builder->hasFields()) {
      unset($this->builderMap[$type]);
    }
  }

  /**
   * Get the aggregation builder.
   *
   * @param string $search_part_type
   *   The search part type.
   * @param string $agg_name
   *   The aggregation name.
   * @param object $agg
   *   The aggregation bucket.
   *
   * @return bool
   *   A built aggregation.
   */
  private function getAggregationBuilder($search_part_type, $agg_name, $agg) {
    $normalized_agg_name = $this->getNormalizedAggName($agg_name);
    if (empty($agg->values) || ($search_part_type != 'terms' && $search_part_type != 'range')) {
      return $this->fireAnErrorQueryBuilder();
    }
    else {
      $agg_values = $agg->values;
      return $search_part_type == 'terms' ?
        TermsFieldFilterQueryDtoBuilder::newBuilder()
          ->field($normalized_agg_name)
          ->values($agg_values) :
        RangeFieldFilterQueryDtoBuilder::newBuilder()
          ->field($normalized_agg_name)
          ->fromBuilder(RangeConditionDtoBuilder::newBuilder()
          ->gte($agg_values[0]->min))
          ->toBuilder(RangeConditionDtoBuilder::newBuilder()
          ->lte($agg_values[0]->max));
    }
  }

  /**
   * Create the query.
   *
   * @param \LogicalFilterQueryDtoBuilder $builder
   *   The query builder.
   * @param int $from
   *   From index.
   * @param int $size
   *   The size of the results.
   * @param string $sort_field
   *   The By sorted field.
   * @param string $matches
   *   String matches for free text search.
   * @param string $group_by
   *   The grouped by field.
   *
   * @return object
   *   The Query Dto object to send to server.
   */
  private function createQuery(LogicalFilterQueryDtoBuilder $builder, $from, $size, $sort_field, $matches, $group_by = NULL) {
    $dto = QueryDtoBuilder::newBuilder($from, $size, $sort_field, $matches, $group_by)
      ->filteredQuery($builder->build())->build();

    return $dto;
  }

  /**
   * Initializing helper.
   */
  private function initializeDefaultSortMap() {
    global $language;
    $lang = $language->language;

    $this->defaultSortMap = array(
      self::VARIABLES => "name",
      self::DATASETS => "acronym." . $lang,
      self::STUDIES => "acronym." . $lang,
      self::NETWORKS => "acronym." . $lang,
    );
  }

  /**
   * Fire an error query builder depending on session state.
   *
   * @return bool
   *   TRUE/FALSE error state.
   */
  private function fireAnErrorQueryBuilder() {
    if (empty($_SESSION['errorsQuery'])) {
      drupal_set_message(t("An error occurred when parsing url query, Please make sure to use facet search tools
        to perform your search. <br>This page will be refeshed in 3s "), 'warning');
      $_SESSION['errorsQuery'] = TRUE;
    }
    return FALSE;
  }

}
