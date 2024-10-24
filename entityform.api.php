<?php
/**
 * @file
 * Hooks provided by the Entityform module.
 *
 * Entityform and Entityform Type are standard entities using the Entity API
 * module. The standard Entity hooks are available for them.
 * Including:
 *  - hook_entityform_load()
 *  - hook_entityform_insert()
 *  - hook_entityform_update()
 *  - hook_entityform_delete()
 *  - hook_entityform_presave()
 *  - hook_entityform_view()
 *  - hook_entityform_view_alter()
 *  - hook_entityform_type_load()
 *  - hook_entityform_type_insert()
 *  - hook_entityform_type_update()
 *  - hook_entityform_type_delete()
 *  - hook_entityform_type_presave()
 *  - hook_entityform_type_view()
 *  - hook_entityform_type_view_alter()
 */

/**
 * Alter the render array that will make the confirm page all entityform types.
 *
 * This is the called the "Submission Page Settings" on the Entityform Type edit
 * page.
 *
 * @param array $render_array
 * @param string $entityform_type
 * @param string|null $entityform_id
 */
function hook_entityform_confirm_page_alter(array &$render_array, $entityform_type, $entityform_id) {
  $render_array['new_markup'] = array(
    '#markup' => t('Hello World!'),
    '#type' => 'markup',
  );
}

/**
 * Alter the render array for the confirm page for a single Entityform Type.
 *
 * @param array $render_array
 * @param string $entityform_type
 * @param string|null $entityform_id
 */
function hook_entityform_ENTITYFORM_TYPE_confirm_page_alter(array &$render_array, $entityform_type, $entityform_id) {
  $render_array['new_markup'] = array(
    '#markup' => t('Hello 1 Form World!'),
    '#type' => 'markup',
  );
}

/**
 * Alter whether a user should have access to an operation on an entityform.
 *
 * @param bool $access
 * @param string $op
 * @param array $context
 *   An associative array containing the following keys:
 *   - entityform: The entityform submission
 *    If $op == 'submit' this will be a new Entityform Submission for the type.
 *   - account: The Drupal user account that is doing the operation.
 */
function hook_entityform_access_alter(&$access, $op, array $context) {
  $entityform = $context['entityform'];
  // Only allow editing of draft forms.
  if ($op == 'edit' && empty($entityform->draft)) {
    $access = FALSE;
  }
}

/**
 * Alter render array for Draft page.
 *
 * @param array $render_array
 * @param string $entityform_type
 * @param array $args
 *   Extra arguments to the function entityform_draft_page() function.
 */
function hook_entityform_draft_page_alter(array &$render_array, $entityform_type, $args) {
  $render_array['new_markup'] = array(
    '#markup' => t('Hello World - draft!'),
    '#type' => 'markup',
  );
}

/**
 * Alter render array for Draft page for a specific Entityform Type.
 *
 * @param array $render_array
 * @param EntityformType $entityform_type
 * @param array $args
 *   Extra arguments to the function entityform_draft_page() function.
 */
function hook_entityform_ENTITYFORM_TYPE_draft_page_alter(array &$render_array, $entityform_type, array $args) {
  $render_array['new_markup'] = array(
    '#markup' => t('Hello World - draft!'),
    '#type' => 'markup',
  );
}

/**
 * Alter the previous Entityform Submission for a user when submitting a form.
 *
 * @param object $entityform_submission
 *   The current previous submission if any.
 * @param string $entityform_type
 * @param array $context
 *   An associative array containing the following keys:
 *    - draft: whether draft submissions should be included.
 *    - uid: uid of the user to find previous submissions.
 *
 * @see entityform_anonymous_entityform_previous_submission_alter()
 */
function hook_entityform_previous_submission_alter(&$entityform_submission, $entityform_type, $context) {
}

/**
 * Allow altering fields automatically added to Entityform Views.
 *
 * Entityform_settings will already have been added to the current view display.
 *
 * @see _entityform_view_add_all_fields()
 * @param array $autofields
 *   Array of fields that will be added.
 * @param view $view
 * @param string $display_id
 */
function hook_entityform_views_autofields_alter(array &$autofields, $view, $display_id) {
  $view_entityform_settings = $view->display[$display_id]->entityform_settings;
  $instances = field_info_instances('entityform', $view_entityform_settings['entityform_type']);
  $view_mode = $view_entityform_settings['view_mode'];
  foreach ($autofields as &$autofield) {
    // Check to see this field was added by entityform.
    if ($instances[$autofield['field_name']]) {
      $field = $instances[$autofield['field_name']];
      if ($label = _entityform_view_label_get_label($field, $view_mode)) {
        $autofield['options']['label'] = $label;
      }
    }
  }
}
