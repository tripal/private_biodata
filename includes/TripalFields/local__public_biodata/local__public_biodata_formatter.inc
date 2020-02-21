<?php
/**
 * @class
 * Purpose:
 *
 * Display:
 * Configuration:
 */
class local__public_biodata_formatter extends ChadoFieldFormatter {

  // The default label for this field.
  public static $default_label = 'Public Tripal Content';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('local__public_biodata');

  // The list of default settings for this formatter.
  public static $default_settings = array();

  /**
   * @see ChadoFieldFormatter::settingsForm()
   *
   */
  public function settingsForm($view_mode, $form, &$form_state) {

  }

  /**
   * @see ChadoFieldFormatter::View()
   *
   */
  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {

    // Get the settings
    $settings = $display['settings'];
  }

  /**
   * @see ChadoFieldFormatter::settingsSummary()
   *
   */
  public function settingsSummary($view_mode) {
    return '';
  }

}
