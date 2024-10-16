<?php
/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */

/**
 * @todo
 */
class entityform_handler_edit_link_field extends entityform_handler_link_field {

  /**
   *
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   *
   */
  public function render($values) {
    if ($entity = $this->get_value($values)) {
      $uri = array(
        'path' => 'entityform/' . $entity->entityform_id . '/edit',
        'options' => NULL,
      );
      return $this->render_link($entity, 'edit', $uri, t('edit'));
    }
  }

}
