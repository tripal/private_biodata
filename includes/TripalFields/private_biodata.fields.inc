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
  $fields = array();

  // First add my term.
  tripal_insert_cvterm(array(
    'id' => 'local:private_biodata',
    'name' => 'private_biodata',
    'cv_name' => 'local',
    'definition' => 'Provides a simple checkbox to keep data private.',
  ));

  // Then describe the field defined by that term.
  $field_name = 'local__private_biodata';
  $field_type = 'local__private_biodata';
  $fields[$field_name] = array(
    'field_name' => $field_name,
    'type' => $field_type,
    'cardinality' => 1,
    'locked' => FALSE,
    'storage' => array(
      'type' => 'field_chado_storage',
    ),
  );

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
  $instances = array();

  $field_name = 'local__private_biodata';
  $field_type = 'local__private_biodata';
  $instances[$field_name] =  array(
    'field_name' => $field_name,
    'entity_type' => $entity_type,
    'bundle' => $bundle->name,
    'label' => 'private Tripal Content',
    'description' => 'Provides a simple checkbox to keep data private.',
    'required' => FALSE,
    'settings' => array(
      'term_vocabulary' => 'local',
      'term_name' => 'private_biodata',
      'term_accession' => 'private_biodata',
      'auto_attach' => FALSE,
      'chado_table' => NULL,
      'chado_column' => NULL,
      'base_table' => $bundle->data_table,
    ),
    'widget' => array(
      'type' => 'local__private_biodata_widget',
      'settings' => array(),
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'local__private_biodata_formatter',
        'settings' => array(),
      ),
    ),
  );

  return $instances;
}
