<?php
/**
 * @class
 * Purpose:
 *
 * Allowing edit?
 * Data:
 * Assumptions:
 */
class ncit__private_widget extends ChadoFieldWidget {

  // The default label for this field.
  public static $default_label = 'Keep Private';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('ncit__private');


  /**
   * @see ChadoFieldWidget::form()
   *
   */
  public function form(&$widget, &$form, &$form_state, $langcode, $items, $delta, $element) {
    parent::form($widget, $form, $form_state, $langcode, $items, $delta, $element);

    $title = ($this->instance['label']) ? $this->instance['label'] : 'Kepp Private';
    $description = ($this->instance['description']) ? $this->instance['description'] : 'Check this box to protect the current page to a specific set of priviledged users.';
    $default_value = (isset($items[0]['is_private'])) ? $items[0]['is_private'] : FALSE;

    $icon_path = drupal_get_path('module', 'private_biodata') . '/includes/TripalFields/ncit__private/iconfinder_102_111044.png';
    $lock_img = theme('image', array('path' => $icon_path));

    $widget['is_private'] = [
      '#type' => 'checkbox',
      '#title' => $title,
      '#description' => $description,
      '#default_value' => $default_value,
      '#prefix' => $lock_img,
    ];

    $widget['custom_css'] = [
      '#type' => 'markup',
      '#markup' => '<style>
        .field-name-ncit--private {
          border: 1px solid #ccc;
          background-color: #F2F2F2;
          padding: 25px;
        }
        .field-name-ncit--private label.option {
          font-size: 1.2em;
          font-weight: bold;
        }
        .field-name-ncit--private img {
          height: 45px;
          float: left;
          padding-right: 20px;
        }
      </style>',
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
    $field_name = $this->instance['field_name'];
    $entity_id = $form_state['TripalEntity']->id;
    $private = $form_state['values'][$field_name][$langcode][0]['is_private'];

    // If the user set this page to be public, remove our record for it.
    if (!$private) {
      db_query('DELETE FROM {private_biodata} WHERE entity_id=:id',
        [':id' => $entity_id]);
    }
    // Otherwise it should be made private!!!
    else {
      // Check if it already exists...
      $exists = db_query('SELECT true FROM {private_biodata} WHERE entity_id=:id',
        [':id' => $entity_id])->fetchField();
      if (!$exists) {
        db_query('INSERT INTO {private_biodata} (entity_id, private) VALUES (:id, :private)',
          [':id' => $entity_id, ':private' => $private]);
        drupal_set_message('This page has been made private.');
      }
      else {
        db_query('UPDATE {private_biodata} SET private=:private WHERE entity_id=:id',
          [':id' => $entity_id, ':private' => $private]);
        drupal_set_message('This page will be kept private.');
      }
    }
  }

}
