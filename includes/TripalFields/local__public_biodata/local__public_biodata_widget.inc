<?php
/**
 * @class
 * Purpose:
 *
 * Allowing edit?
 * Data:
 * Assumptions:
 */
class local__public_biodata_widget extends ChadoFieldWidget {

  // The default label for this field.
  public static $default_label = 'Public Tripal Content';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('local__public_biodata');


  /**
   * @see ChadoFieldWidget::form()
   *
   */
  public function form(&$widget, &$form, &$form_state, $langcode, $items, $delta, $element) {
    parent::form($widget, $form, $form_state, $langcode, $items, $delta, $element);

    dpm($this->instance, 'instance');

    $title = ($this->instance['label']) ? $this->instance['label'] : 'Release Publicly';
    $description = ($this->instance['description']) ? $this->instance['description'] : 'Check this box to release the current data page for access by anonymous users.';
    $default_value = (isset($this->instance['default_value'])) ? $this->instance['default_value'][0]['is_public'] : FALSE;

    $widget['is_public'] = [
      '#type' => 'checkbox',
      '#title' => $title,
      '#description' => $description,
      '#default_value' => $default_value,
    ];
  }

  /**
   * @see ChadoFieldWidget::validate()
   *
   */
  public function validate($element, $form, &$form_state, $langcode, $delta) {
  }

  /**
   * @see ChadoFieldWidget::submit()
   *
   */
  public function submit($form, &$form_state, $entity_type, $entity, $langcode, $delta) {
  }

}
