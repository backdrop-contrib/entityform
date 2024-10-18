<?php
/**
 * @file
 * Entityform editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring
 * instead to use views. That offers more flexibility to change a UI that will,
 * more often than not, be end-user facing.
 */

/**
 * UI controller.
 */
class EntityformUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Entityforms',
      'description' => 'Add edit and update entityforms.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => backdrop_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of entityforms.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items['eform/submit/%entityform_type'] = array(
      'title callback' => 'entityform_page_title',
      'title arguments' => array(2, 1),
      'page callback' => 'entityform_type_submit_page',
      'page arguments' => array(2),
      'access callback' => 'entityform_access',
      'access arguments' => array('submit', 2),
      'file' => 'entityform.admin.inc',
      'file path' => backdrop_get_path('module', $this->entityInfo['module']),
      'type' => MENU_CALLBACK,
    );
    $items['eform/%entityform_type/confirm'] = array(
      'title callback' => 'entityform_type_page_title',
      'title arguments' => array(1, 'confirm'),
      'description' => '',
      'page callback' => 'entityform_confirm_page',
      'page arguments' => array(1, 3),
      'access callback' => 'entityform_access',
      'access arguments' => array('confirm', 1),
    );

    $items['eform/%entityform_type/draft'] = array(
      'title' => 'Form Saved',
      'description' => '',
      'page callback' => 'entityform_draft_page',
      'page arguments' => array(1),
      'access callback' => TRUE,
      'access arguments' => array('draft_save', 1),
    );
    $items['eform/%entityform_type/submissions'] = array(
      'title callback' => 'entityform_type_page_title',
      'title arguments' => array(1, 'view submissions'),
      'description' => '',
      'page callback' => 'entityform_submission_page',
      'page arguments' => array(1, 3, 'user'),
      'access arguments' => array('view own entityform'),
    );
    $types = array_keys(entityform_get_types());
    $controller = entity_ui_controller('entityform');

    // Loading and editing entityform entities.
    $items['entityform/' . $wildcard] = array(
      'page callback' => 'entityform_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'entityform_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'entityform.admin.inc',
      'file path' => backdrop_get_path('module', $this->entityInfo['module']),
    );
    $items['entityform/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'entityform_form_wrapper',
      'page arguments' => array(1, 'edit'),
      'access callback' => 'entityform_access',
      'access arguments' => array('edit', 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'entityform.admin.inc',
      'file path' => backdrop_get_path('module', $this->entityInfo['module']),
    );

    $items['entityform/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'entityform_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'entityform_access',
      'access arguments' => array('delete', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'entityform.admin.inc',
      'file path' => backdrop_get_path('module', $this->entityInfo['module']),
    );

    // Menu item for viewing entityforms.
    $items['entityform/' . $wildcard] = array(
      'title callback' => 'entityform_page_title',
      'title arguments' => array(1),
      'page callback' => 'entityform_page_view',
      'page arguments' => array(1),
      'access callback' => 'entityform_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    $items['entityform/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    return $items;
  }

  /**
   * Create the markup for the add Entityform Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      backdrop_goto($item['href']);
    }

    return theme('entityform_add_list', array('content' => $content));
  }

  /**
   *
   */
  public function confirm_path($bundle, $entityform_id = NULL) {
    module_load_include('inc', 'entityform', 'entityform_type.admin');
    $entityform_type = entityform_type_load($bundle);
    if (!empty($entityform_id)) {
      $redirect_path = $entityform_type->get_path_property('redirect_path', entityform_load($entityform_id));
    }
    else {
      $redirect_path = $entityform_type->get_path_property('redirect_path');
    }
    if (!empty($redirect_path)) {
      return $redirect_path;
    }
    $path = _entityform_type_get_confirm_url($bundle);
    $path = backdrop_get_path_alias($path);
    if ($entityform_id) {
      return array(
        $path,
        array(
          'query' => array(
            'entityform_id' => $entityform_id,
          ),
        ),
      );
    }
    return array($path);
  }

  /**
   *
   */
  public function draft_path($bundle) {
    $bundle = str_replace('_', '-', $bundle);
    return "eform/$bundle/draft";
  }

  /**
   *
   */
  public function submit_path($bundle) {
    module_load_include('inc', 'entityform', 'entityform_type.admin');
    $path = _entityform_type_get_submit_url($bundle);
    $path = backdrop_get_path_alias($path);
    return $path;
  }

}

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
    $items[$this->path]['type'] = MENU_NORMAL_ITEM;
    return $items;
  }

  /**
   *
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);
    $row[] = l(t('Submissions'), $this->path . '/manage/' . $id . '/submissions');
    $row[] = l(t('Submit Link'), entity_ui_controller('entityform')->submit_path($id));
    return $row;
  }

  /**
   *
   */
  public function overviewTable($conditions = array()) {
    $render = parent::overviewTable($conditions);
    $render['#header'][] = t('Submissions');
    $render['#header'][] = t('Submit Link');
    return $render;
  }

}
