<?php

/**
 * @file
 * Entityform type editing UI.
 */

/**
 * UI controller.
 */
class EntityformTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage entityform entity types, including adding and removing fields and the display of fields.';
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }

  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('Submissions'), "admin/content/entityforms/list/" . $id);
    $row[] = l(t('Submit Link'), entity_ui_controller('entityform')->submit_path($id));
    return $row;
  }

  public function overviewTable($conditions = array()) {
    $render = parent::overviewTable($conditions);
    $render['#header'][] = t('Submissions');
    $render['#header'][] = t('Submit Link');
    return $render;
  }

}


/**
 * Generates the entityform type editing form.
 */
function entityform_type_form($form, &$form_state, $entityform_type, $op = 'edit') {

  if ($op == 'clone') {
    $entityform_type->label .= ' (cloned)';
    $entityform_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $entityform_type->label,
    '#description' => t('The human-readable name of this entityform type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entityform_type->type) ? $entityform_type->type : '',
    '#maxlength' => 32,
    //    '#disabled' => $entityform_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'entityform_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this entityform type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['instruction_pre'] = array(
    '#type' => 'textarea',
    '#title' => t('Intro Form instructions'),
    '#default_value' => empty($entityform_type->data['instruction_pre']) ? '' : $entityform_type->data['instruction_pre'],
  );
  $form['data']['draftable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Draftable'),
    '#default_value' => !empty($entityform_type->data['draftable']),
    '#description' => t('Should users be able to save a draft of this form?'),
  );
  $form['data']['draft_save_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Draft save text'),
    '#default_value' => empty($entityform_type->data['draft_save_text']) ? '' : $entityform_type->data['draft_save_text'],
    '#description' => t('Text to dispaly to the user when the form is saved as a draft'),
  );
  $form['data']['redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect Path'),
    '#default_value' => empty($entityform_type->data['redirect_path']) ? '' : $entityform_type->data['redirect_path'],
    '#description' => t('What path should the user be redirected to on correct submission?  Leave blank for default action'),
  );
  $form['data']['submission_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Submission Reply'),
    '#default_value' => empty($entityform_type->data['submission_text']) ? '' : $entityform_type->data['submission_text'],
    '#description' => t('Text for correct submission'),
  );
  $form['data']['submission_show_submitted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Submission information'),
    '#default_value' => !empty($entityform_type->data['submission_show_submitted']),
    '#description' => t('Show submitted data on submission page?'),
  );
  $form['data']['email_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#weight' => 20,
  );
  $form['data']['email_set']['confirm_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send Confirmation Email'),
    '#default_value' => !empty($entityform_type->data['confirm_email']),
    '#description' => t('Should users recieve a conformation email when submitting this form?'),
  );
  $form['data']['email_set']['email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Text'),
    '#default_value' => empty($entityform_type->data['email_body']) ? '' : $entityform_type->data['email_body'],
    '#description' => t('Text for Confirmation Email. DO NOT put HTML in this field'),
  );
  $form['data']['email_set']['confirm_email_show_submitted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Submission information in confirm email'),
    '#default_value' => !empty($entityform_type->data['confirm_email_show_submitted']),
    '#description' => t('Show submitted data in conformation email?'),
  );
  $form['data']['email_set']['notify_emails'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification Emails'),
    '#default_value' => empty($entityform_type->data['notify_emails']) ? '' : $entityform_type->data['notify_emails'],
    '#description' => t('Enter emails that should be notified for each form submissions.'),
    '#element_validate' => array('_entityform_form_email_list_validate'),
  );
  $form['data']['access_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access settings'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
    '#weight' => 40,
  );
  $form['data']['access_set']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#options' => user_roles(),
    '#default_value' => empty($entityform_type->data['roles']) ? array() : $entityform_type->data['roles'],
    '#description' => t('Roles who can submit this form.'),
    '#multiple' => TRUE,
  );
  $form['data']['access_set']['resubmit_action'] = array(
    '#type' => 'select',
    '#title' => t('Resubmit Action'),
    '#options' => array(
      'new' => t('Allow new submission'),
      'old' => t('Edit old submission'),
      'disallow' => t("Don't allow"),
      'confirm' => t('Goto Confirm page'),
    ),
    '#default_value' => empty($entityform_type->data['resubmit_action']) ? 'new' : $entityform_type->data['resubmit_action'],
    '#description' => t('Action to take if logged in user has already submitted form.'),
  );
  //set #parents to skip sets in form values
  foreach (element_children($form['data']) as $key) {
    if (strrpos($key, '_set') === drupal_strlen($key) - drupal_strlen('_set')) {
      foreach (element_children($form['data'][$key]) as $sub_key ) {
        $form['data'][$key][$sub_key]['#parents'] = array('data', $sub_key);
      }
    }
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save entityform type'),
    '#weight' => 40,
  );
  if (module_exists('path')) {
    $form['paths'] = array(
      '#type' => 'fieldset',
      '#title' => t('URL path settings'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($path['alias']),
      '#group' => 'additional_settings',
      '#attributes' => array(
        'class' => array('path-form'),
      ),
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'path') . '/path.js'),
      ),
      '#access' => user_access('create url aliases') || user_access('administer url aliases'),
      '#weight' => 30,
      '#tree' => TRUE,
      '#element_validate' => array('_entityform_paths_form_element_validate'),
    );
    $existing_paths = array(
      'submit' => array(
        'default_path' => _entityform_type_get_submit_url( $entityform_type->type),
        'title' => 'Submit URL alias',
      ),
      'confirm' => array(
        'default_path' => _entityform_type_get_confirm_url( $entityform_type->type),
        'title' => 'Confirm URL alias',
      ),
    );
    foreach ($existing_paths as $key => $existing_path) {
      if ($op == 'edit') {
        //check for existing alias
        $conditions = array('source' => $existing_path['default_path']);
        $path = path_load($conditions);
        if ($path === FALSE) {
          $path = array();
        }
      }
      else {
        $path = array();
      }
      $path += array(
        'pid' => NULL,
        'source' => isset($entityform_type) ? $existing_path['default_path'] : NULL,
        'alias' => '',
        'language' => LANGUAGE_NONE,
      );
      $form['paths'][$key]['alias'] = array(
        '#type' => 'textfield',
        '#title' => t($existing_path['title']),
        '#default_value' => $path['alias'],
        '#maxlength' => 255,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('Optionally specify an alternative URL by which this content can be accessed. For example, type "about" when writing an about page. Use a relative path and don\'t add a trailing slash or the URL alias won\'t work.'),
      );
      $form['paths'][$key]['pid'] = array(
        '#type' => 'value',
        '#value' => $path['pid'],
      );
      $form['paths'][$key]['source'] = array(
        '#type' => 'value',
        '#value' => $path['source'],
      );
      $form['paths'][$key]['language'] = array(
        '#type' => 'value',
        '#value' => $path['language'],
      );
    }
    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#weight' => 99,
    );
  }
  return $form;
}
/**
 * Form element validation handler for URL alias form element.
 */
function _entityform_paths_form_element_validate($element, &$form_state, $complete_form) {
  $previous_pathes = array();
  foreach ($form_state['values']['paths'] as $path_type => $path) {
    // Trim the submitted value.
    $alias = trim($path['alias']);
    if (!empty($alias)) {
      form_set_value($element[$path_type]['alias'], $alias, $form_state);

      $path = $form_state['values']['paths'][$path_type];
      if (in_array($path['alias'], $previous_pathes)) {
        form_error($element[$path_type]['alias'], "Aliases must be unique.");
        return;
      }
      $previous_pathes[] = $path['alias'];

      // Ensure that the submitted alias does not exist yet.
      $query = db_select('url_alias')
      ->condition('alias', $path['alias'])
      ->condition('language', $path['language']);
      if (!empty($path['source'])) {
        $query->condition('source', $path['source'], '<>');
      }
      $query->addExpression('1');
      $query->range(0, 1);
      if ($query->execute()->fetchField()) {
        form_error($element[$path_type]['alias'], t('The alias is already in use.'));
        return;
      }
    }
  }

}
function _entityform_form_email_list_validate($element, &$form_state, $form) {
  $emails = explode(', ', $element['#value']);
  foreach ($emails as $email) {
    $email = trim($email);
    if (!empty($email) && !valid_email_address($email)) {
      form_error($element, t('Please enter valid email addresses.'));
      return;
    }
  }
}


/**
 * Form API submit callback for the type form.
 */
function entityform_type_form_submit(&$form, &$form_state) {
  $entityform_type = entity_ui_form_submit_build_entity($form, $form_state);
  $entityform_type->save();
  $form_state['redirect'] = 'admin/content/entityform_types';
}


/**
 * Form API submit callback for the delete button.
 */
function entityform_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/entityform_types/manage/' . $form_state['entityform_type']->type . '/delete';
}
function _entityform_type_get_submit_url($type) {
  return 'eform/submit/' . str_replace('_', '-', $type);
}
function _entityform_type_get_confirm_url($type) {
  return 'eform/' . str_replace('_', '-', $type) . '/confirm';
}
