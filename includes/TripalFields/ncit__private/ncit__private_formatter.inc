<?php
/**
 * @class
 * Purpose: Display a message to the user telling them if the current page
 *   is private. This will only be seen by users with permission to see
 *   private content. All other users will simply see "Access Denied".
 *
 * Display: Warning message on private content.
 * Configuration: You can set the message and it's style.
 */
class ncit__private_formatter extends TripalFieldFormatter {

  // The default label for this field.
  public static $default_label = 'Private Tripal Content';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = ['ncit__private'];

  // The list of default settings for this formatter.
  public static $default_settings = [
    'private_msg' => 'The current page is PRIVATE. If this data has been published, please edit this page and uncheck the private setting.',
    'use_dsm' => TRUE,
  ];

  /**
   * @see ChadoFieldFormatter::settingsForm()
   *
   */
  public function settingsForm($view_mode, $form, &$form_state) {
    $element = [];

    // Get the settings
    $settings = $this->instance['display'][$view_mode]['settings'];
    // Fill in defaults for any settings not set.
    foreach (self::$default_settings as $key => $value) {
      if (!isset($settings[$key])) {
        $settings[$key] = $value;
      }
    }

    $element['private_msg'] = [
      '#type' => 'textfield',
      '#title' => 'Message for Private Content',
      '#description' => 'This message is shown to users to inform them that the current page is private.',
      '#default_value' => $settings['private_msg'],
    ];

    $element['use_dsm'] = [
      '#type' => 'checkbox',
      '#title' => 'Use Drupal Set Message',
      '#description' => 'Checking this box will display the message to users in the typical warning message box.',
      '#default_value' => $settings['use_dsm'],
    ];

    return $element;
  }

  /**
   * @see ChadoFieldFormatter::View()
   *
   */
  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {

    // Get the settings
    $settings = $display['settings'];
    // Fill in defaults for any settings not set.
    foreach (self::$default_settings as $key => $value) {
      if (!isset($settings[$key])) {
        $settings[$key] = $value;
      }
    }

    // If this page is private then...
    if ((isset($items[0]['is_private'])) && ($items[0]['is_private'] == TRUE)) {
      // Otherwise, tell the user so they can change it if it shouldn't be.
      if ($settings['use_dsm']) {
        $element[0] = [
          '#prefix' => '<div class="messages warning">',
          '#markup' => $settings['private_msg'],
          '#suffix' => '</div>',
        ];
      }
      else {
        $element[0] = [
          '#markup' => $settings['private_msg'],
        ];
      }
    }
  }

  /**
   * @see ChadoFieldFormatter::settingsSummary()
   *
   */
  public function settingsSummary($view_mode) {
    $settings = $this->instance['display'][$view_mode]['settings'];

    // Ensure settings have defaults set.
    foreach (self::$default_settings as $k => $default) {
      if (!isset($settings[$k])) { $settings[$k] = $default; }
    }

    return '<strong>Message:</strong> '.$settings['private_msg'];
  }

}
