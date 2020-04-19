<?php
/**
 * @class
 * Purpose:
 *
 * Display:
 * Configuration:
 */
class ncit__private_formatter extends ChadoFieldFormatter {

  // The default label for this field.
  public static $default_label = 'Private Tripal Content';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('ncit__private');

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

    // If this page is private then tell the user so they can change it if it shouldn't be.
    if ((isset($items[0]['is_private'])) && ($items[0]['is_private'] == TRUE)) {
      $element[0] = [
        '#prefix' => '<div class="messages warning">',
        '#markup' => 'The current page is PRIVATE. If this data has been published, please edit this page and uncheck the private setting.',
        '#suffix' => '</div>',
      ];
    }
  }

  /**
   * @see ChadoFieldFormatter::settingsSummary()
   *
   */
  public function settingsSummary($view_mode) {
    return '';
  }

}
