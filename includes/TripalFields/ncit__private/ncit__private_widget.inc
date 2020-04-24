<?php
/**
 * @class
 * Purpose: Allow users to set whether a particular page should be private.
 *
 * Allowing edit? Yes
 * Data: Saved in public.private_biodata
 *
 * Note: Chado was not chosen as the storage backend because we didn't want this
 *   to show up as a chado property.
 */
class ncit__private_widget extends TripalFieldWidget {

  // The default label for this field.
  public static $default_label = 'Keep Private';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = ['ncit__private'];


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
    $lock_img = theme('image', ['path' => $icon_path]);

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
    $private = $form_state['values'][$field_name][$langcode][0]['is_private'];

    // If this is a new entity (i.e. on create)
    // then we can't access the entity_id here...
    if (isset($form_state['TripalEntity']->is_new) AND ($form_state['TripalEntity']->is_new)) {
      // We cannot set this entity to private on create so let the user know.
      if ($private) {
        drupal_set_message('Cannot set the page to private before it exists. Please edit the page to set it to private.', 'warning');
      }
      return TRUE;
    }
    $entity_id = $form_state['TripalEntity']->id;

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
