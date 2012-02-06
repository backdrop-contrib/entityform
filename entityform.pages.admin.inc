<?php
/*
 * @file
 * Global(not form specific) admin pages
 */
/**
 * Entityform global setting form
 * @param array $form
 * @param array $form_state
 * @return array
 */
function entityform_settings($form, &$form_state) {
  //choose the default View for viewing for submissions
  $form['entityform_default_submissions_view'] = array(
    '#type' => 'select',
    '#title' => t('Default View for Submissions'),
    '#default_value' => variable_get('entityform_default_submissions_view', 'entityforms:entityforms_admin_page'),
  );
  $form['entityform_default_submissions_view']['#options'] = _entityform_get_entityform_views_options();
  return system_settings_form($form);
}
