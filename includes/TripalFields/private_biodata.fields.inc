<?php
/**
 * @file
 * Contains all field specific code outside the classes.
 */

/**
 * Implements hook_bundle_fields_info().
 *
 * @param $entity_type
 *   This should be 'TripalEntity' for all Tripal Content.
 * @param $bundle
 *   This object describes the Type of Tripal Entity (e.g. Organism or Gene).
 *
 * @return
 *   An array of field definitions. Each field in this array will be created
 *   if it doesn't already exist. To trigger create of fields when
 *   developing call tripal_refresh_bundle_fields() for the specific bundle.
 */
function private_biodata_bundle_fields_info($entity_type, $bundle) {
  $fields = [];

  // First add my term.
  tripal_insert_cvterm([
    'id' => 'NCIT:C54104',
    'name' => 'Private',
    'cv_name' => 'ncit',
    'definition' => 'Not public; kept secret or restricted.',
  ]);

  // Then describe the field defined by that term.
  $field_name = 'ncit__private';
  $field_type = 'ncit__private';
  $fields[$field_name] = [
    'field_name' => $field_name,
    'type' => $field_type,
    'cardinality' => 1,
    'locked' => FALSE,
    'storage' => [
      'type' => 'tripal_no_storage',
    ],
  ];

  return $fields;
}

/**
 * Implements hook_bundle_instances_info().
 *
 * @param $entity_type
 *   This should be 'TripalEntity' for all Tripal Content.
 * @param $bundle
 *   This object describes the Type of Tripal Entity (e.g. Organism or Gene).
 * @return
 *   An array of field instance definitions.
 */
function private_biodata_bundle_instances_info($entity_type, $bundle) {
  $instances = [];

  $field_name = 'ncit__private';
  $field_type = 'ncit__private';
  $instances[$field_name] =  [
    'field_name' => $field_name,
    'entity_type' => $entity_type,
    'bundle' => $bundle->name,
    'label' => 'Keep Private',
    'description' => 'Check this checkbox to keep the current page private.',
    'required' => FALSE,
    'settings' => [
      'term_vocabulary' => 'NCIT',
      'term_name' => 'Private',
      'term_accession' => 'C54104',
      'auto_attach' => TRUE,
      'base_table' => $bundle->data_table,
    ],
    'widget' => [
      'type' => 'ncit__private_widget',
      'settings' => [],
    ],
    'display' => [
      'default' => [
        'label' => 'hidden',
        'type' => 'ncit__private_formatter',
        'settings' => [],
      ],
    ],
  ];

  return $instances;
}
