<?php

/**
 * @file
 * Providing extra functionality for the Entityform UI via views.
 */


/**
 * Implements hook_views_data().
 */
function entityform_views_data_alter(&$data) {
  $data['entityform']['link_entityform'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the entityform.'),
      'handler' => 'entityform_handler_link_field',
    ),
  );
  $data['entityform']['edit_entityform'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the entityform.'),
      'handler' => 'entityform_handler_edit_link_field',
    ),
  );
  $data['entityform']['delete_entityform'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the entityform.'),
      'handler' => 'entityform_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows entityforms/entityform/%entityform_id/op
  $data['entityform']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this entityform.'),
      'handler' => 'entityform_handler_entityform_operations_field',
    ),
  );
  $data['entityform_type']['table']['join'] = array(
    'entityform' => array(
      'left_field' => 'type',
      'field' => 'type',
      //'type' => 'INNER', // all nodes have an author.
    ),
  );
  $data['entityform']['uid']['relationship'] = array(
    'base' => 'users',
    'field' => 'uid',
    'handler' => 'views_handler_relationship',
    'label' => t('User Submitting'),
  );
  $data['entityform']['table']['join']['user'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );
}

/**
 * Implements hook_views_default_views().
 */
function entityform_views_default_views() {
  $view = new view;
$view->name = 'entityforms';
$view->description = 'A list of all entityforms submissions';
$view->tag = 'entityforms';
$view->base_table = 'entityform';
$view->human_name = 'Entityforms';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Form Submissions';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any entityform';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '40';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'entityform_id' => 'entityform_id',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entityform_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'Empty ';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'No entityforms have been created yet';
/* Relationship: Entityform: Uid */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'entityform';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['required'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'Submitter';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['external'] = 0;
$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Entityform: Link */
$handler->display->display_options['fields']['link_entityform']['id'] = 'link_entityform';
$handler->display->display_options['fields']['link_entityform']['table'] = 'entityform';
$handler->display->display_options['fields']['link_entityform']['field'] = 'link_entityform';
$handler->display->display_options['fields']['link_entityform']['label'] = 'View';
$handler->display->display_options['fields']['link_entityform']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['external'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['link_entityform']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['link_entityform']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['trim'] = 0;
$handler->display->display_options['fields']['link_entityform']['alter']['html'] = 0;
$handler->display->display_options['fields']['link_entityform']['element_label_colon'] = 1;
$handler->display->display_options['fields']['link_entityform']['element_default_classes'] = 1;
$handler->display->display_options['fields']['link_entityform']['hide_empty'] = 0;
$handler->display->display_options['fields']['link_entityform']['empty_zero'] = 0;
/* Field: Entityform: Edit Link */
$handler->display->display_options['fields']['edit_entityform']['id'] = 'edit_entityform';
$handler->display->display_options['fields']['edit_entityform']['table'] = 'entityform';
$handler->display->display_options['fields']['edit_entityform']['field'] = 'edit_entityform';
$handler->display->display_options['fields']['edit_entityform']['label'] = 'Edit';
$handler->display->display_options['fields']['edit_entityform']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['external'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_entityform']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_entityform']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_entityform']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_entityform']['element_label_colon'] = 1;
$handler->display->display_options['fields']['edit_entityform']['element_default_classes'] = 1;
$handler->display->display_options['fields']['edit_entityform']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_entityform']['empty_zero'] = 0;
$handler->display->display_options['fields']['edit_entityform']['hide_alter_empty'] = 1;
/* Field: Entityform: Delete Link */
$handler->display->display_options['fields']['delete_entityform']['id'] = 'delete_entityform';
$handler->display->display_options['fields']['delete_entityform']['table'] = 'entityform';
$handler->display->display_options['fields']['delete_entityform']['field'] = 'delete_entityform';
$handler->display->display_options['fields']['delete_entityform']['label'] = 'Delete';
$handler->display->display_options['fields']['delete_entityform']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['external'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['delete_entityform']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['delete_entityform']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['trim'] = 0;
$handler->display->display_options['fields']['delete_entityform']['alter']['html'] = 0;
$handler->display->display_options['fields']['delete_entityform']['element_label_colon'] = 1;
$handler->display->display_options['fields']['delete_entityform']['element_default_classes'] = 1;
$handler->display->display_options['fields']['delete_entityform']['hide_empty'] = 0;
$handler->display->display_options['fields']['delete_entityform']['empty_zero'] = 0;
$handler->display->display_options['fields']['delete_entityform']['hide_alter_empty'] = 0;
/* Field: Entityform Type: Label */
$handler->display->display_options['fields']['label_1']['id'] = 'label_1';
$handler->display->display_options['fields']['label_1']['table'] = 'entityform_type';
$handler->display->display_options['fields']['label_1']['field'] = 'label';
$handler->display->display_options['fields']['label_1']['label'] = 'Form Type';
$handler->display->display_options['fields']['label_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['label_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['label_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['label_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['label_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['label_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['label_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['label_1']['empty_zero'] = 0;
/* Field: Entityform: Date submitted */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'entityform';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['external'] = 0;
$handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['element_label_colon'] = 1;
$handler->display->display_options['fields']['created']['element_default_classes'] = 1;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['created']['date_format'] = 'medium';
/* Field: Entityform: Date changed */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'entityform';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['changed']['alter']['external'] = 0;
$handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['changed']['alter']['html'] = 0;
$handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
$handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
$handler->display->display_options['fields']['changed']['hide_empty'] = 0;
$handler->display->display_options['fields']['changed']['empty_zero'] = 0;
$handler->display->display_options['fields']['changed']['hide_alter_empty'] = 0;
/* Contextual filter: Entityform: Type */
$handler->display->display_options['arguments']['type']['id'] = 'type';
$handler->display->display_options['arguments']['type']['table'] = 'entityform';
$handler->display->display_options['arguments']['type']['field'] = 'type';
$handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['type']['glossary'] = 0;
$handler->display->display_options['arguments']['type']['limit'] = '0';
$handler->display->display_options['arguments']['type']['transform_dash'] = 0;
$handler->display->display_options['arguments']['type']['break_phrase'] = 0;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'entityforms_admin_page');
$handler->display->display_options['path'] = 'admin/content/entityforms/list/%';

if(module_exists('views_data_export')) {
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = 'eform_submission_%1-value.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['encoding'] = 'ASCII';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Submitter';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Entityform Type: Label */
  $handler->display->display_options['fields']['label_1']['id'] = 'label_1';
  $handler->display->display_options['fields']['label_1']['table'] = 'entityform_type';
  $handler->display->display_options['fields']['label_1']['field'] = 'label';
  $handler->display->display_options['fields']['label_1']['label'] = 'Form Type';
  $handler->display->display_options['fields']['label_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['label_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['label_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['label_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['label_1']['empty_zero'] = 0;
  /* Field: Entityform: Date submitted */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'entityform';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  /* Field: Entityform: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'entityform';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 0;
  $handler->display->display_options['path'] = 'admin/content/entityforms/csv/%';
  $handler->display->display_options['displays'] = array(
    'entityforms_admin_page' => 'entityforms_admin_page',
    'default' => 0,
  );
}
  $views[$view->name] = $view;
  return $views;
}