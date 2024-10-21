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
 * The class used for entityform entities.
 */
class Entityform extends Entity {

  /**
   * The user id who submitted this form.
   *
   * @var int
   */
  public $uid;

  /**
   * {@inheritdoc}
   */
  public function __construct($values = array()) {
    parent::__construct($values);
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultLabel() {
    $entityform_type = entityform_type_load($this->type);
    $label = $entityform_type->label;
    $submit_user = NULL;
    if (!empty($this->uid)) {
      $submit_user = user_load($this->uid);
    }
    $label .= ' - ' . user_format_name($submit_user);
    $label .= ' - ' . format_date($this->created, 'short');
    return $label;
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'entityform/' . $this->entityform_id);
  }

  /**
   * Get human readable entityform_type label.
   *
   * @return string
   */
  public function getTypeName() {
    $entity_info = entity_get_info('entityform');
    if (!empty($entity_info['bundles'][$this->type])) {
      $entityform_type = entityform_type_load($this->type);
      return $entityform_type->getTranslation('label');
    }
    else {
      return t('Broken or missing form type');
    }
  }

  /**
   * Returns the user who submitted this form.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user who submitted this form.
   *
   * @param User|AnonymousUser|int $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   * {@inheritdoc}
   */
  public function uri() {
    return array(
      'path' => 'entityform/' . $this->entityform_id,
      'options' => array(),
    );
  }

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return $this->entityform_id;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'entityform';
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return t('Form Submission: @label', array(
      '@label' => $this->getTypeName(),
    ));
  }

}


/**
 * The class used for entityform type entities.
 */
class EntityformType extends Entity {

  /**
   *
   */
  protected function defaultUri() {
    return array(
      'path' => 'eform/submit/' . str_replace('_', '-', $this->type),
    );
  }

  public $type;
  public $label;
  public $data;

  /**
   *
   */
  public function __construct($values = array()) {
    parent::__construct($values);
  }

  /**
   * Get A path property for this EntityformType with tokens replaced.
   */
  public function get_path_property($property, $entityform = NULL) {
    $path = $this->getTranslation($property);
    if (empty($path)) {
      return '';
    }
    if ($path == '<front>') {
      return $path;
    }
    $path = _entityform_format_text($path, array(
      'entityform_type' => $this,
      'entityform' => $entityform,
    ));
    $options = backdrop_parse_url(decode_entities($path));
    return array($options['path'], array('query' => $options['query']));
  }

  /**
   *
   */
  public function get_prop($key, $entityform = NULL) {
    if (isset($this->data[$key])) {
      if (is_array($this->data[$key]) && !empty($this->data[$key]['value'])) {
        $value = array(
          'value' => $this->getTranslation($key),
        );
        if (!empty($this->data[$key]['format'])) {
          $value['format'] = $this->data[$key]['format'];
        }
      }
      else {
        $value = $this->getTranslation($key);
      }
    }
    else {
      $value = '';
    }
    $text_value = $value;
    if (is_array($text_value)) {
      // We are dealing for a filtered text value.
      $text_value = $text_value['value'];
    }
    if ($text_value == '<none>') {
      return '';
    }
    $token_types['entityform_type'] = $this;
    if (!empty($entityform)) {
      $token_types['entityform'] = $entityform;
    }
    if (empty($text_value)) {
      $default_entityform_type = entity_get_controller('entityform_type')->create(array());
      if (isset($default_entityform_type->data[$key])) {
        $value = $default_entityform_type->data[$key];
      }
    }
    return _entityform_format_text($value, $token_types);
  }

  /**
   * {@inheritdoc}
   */
  public function getTranslation($property, $langcode = NULL) {
    $all_info = entity_plus_get_all_property_info($this->entityType());
    // Assign by reference to avoid triggering notices if metadata is missing.
    $property_info = &$all_info[$property];
    // @todo make this code readable:
    $property_value = isset($this->$property) ? $this->$property : (isset($this->data[$property]) ? $this->data[$property] : NULL);

    if (is_array($property_value) && isset($property_value['value'])) {
      $property_value = $property_value['value'];
    }
    if (!empty($property_info['translatable'])) {
      if (!empty($property_info['field'])) {
        return field_get_items($this->entityType, $this, $property, $langcode);
      }
      elseif (module_exists('i18n_string') && !empty($property_info['i18n string'])) {
        $name = 'entityform:entityform_type:' . $this->id() . ':' . $property;

        $options = array(
          'langcode' => $langcode,
          'sanitize' => FALSE,
        );
        return html_entity_decode(i18n_string($name, $property_value, $options), ENT_QUOTES);
      }
    }
    return $property_value;
  }

  /**
   * Clears values default text values so global text values can be used.
   */
  public function clear_text_props() {
    $clear_keys = array_keys(entity_get_controller('entityform_type')->get_default_text_values());
    foreach ($clear_keys as $key) {
      $this->data[$key] = '';
    }
  }

  /**
   * Return a list of property labels used for translation forms.
   */
  public function getLabelsForTranslation() {
    return entity_get_controller('entityform_type')->get_text_labels();
  }

  /**
   *
   */
  public function uri() {
    return array(
      'path' => 'eform/submit/' . $this->type,
      'options' => array(),
    );
  }

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return $this->type;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'entityform_type';
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return $this->getTranslation('label');
  }

}


/**
 * The Controller for Entityform entities.
 */
class EntityformController extends EntityPlusController {

  /**
   *
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a entityform - we first set up the values that are specific
   * to our entityform schema but then also go through the EntityAPIController
   * function.
   *
   * @param array $values
   *
   * @return Entityform|null
   *   A entityform object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // We need a type for the bundle but it may not be valid.
    if (isset($values['type'])) {
      // Rules supplies a EntityformType object instead of string.
      if (is_object($values['type'])) {
        if (get_class($values['type']) == 'EntityformType') {
          $values['type'] = $values['type']->type;
        }
        else {
          // This should never happen.
          throw new EntityMalformedException("Object sent for EntityformType not of class EntityformType.");
        }
      }
      $type = entityform_get_types($values['type']);
    }
    if (empty($type)) {
      return NULL;
    }
    // Add values that are specific to our Entityform.
    $values += array(
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
      'data' => '',
    );

    $entityform = parent::create($values);
    return $entityform;
  }

  /**
   * Overriding the buildContent function to add entity specific fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    $content['info']['user'] = array(
      '#markup' => _entityform_get_submit_info($entity),
      '#weight' => -100,
      '#prefix' => '<div class="submitted">',
      '#suffix' => '</div>',
    );

    $content['info']['#weight'] = -99;
    return $content;
  }

}


/**
 * The Controller for Entityform entities.
 */
class EntityformTypeController extends EntityPlusController {

  /**
   *
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a entityform type.
   *
   * We first set up the values that are specific to our entityform type schema
   * but then also go through the EntityAPIController function.
   *
   * @param array $values
   * @param bool $load_defaults
   *
   * @return EntityformType
   *   A entityform type object with all default fields initialized.
   */
  public function create(array $values = array(), $load_defaults = FALSE) {
    // Add values that are specific to our Entityform.
    $default_values = config_get('entityform.settings', 'entityform_type_defaults');
    if (empty($default_values)) {
      $default_values = array();
    }
    $values += $default_values;
    $values += array(
      'id' => '',
      'is_new' => TRUE,
    );
    if (!isset($values['data'])) {
      $values['data'] = array();
    }
    if ($load_defaults) {
      $values['data'] += array(
        'submissions_view' => 'entityforms',
        'user_submissions_view' => 'user_entityforms',
        'preview_page' => 0,
      );
      $values['data'] += $this->get_default_text_values();
    }
    else {
      if ($values['is_new']) {
        // Don't override values even if is_new. Features will send values in
        // for checking status.
        $values['data'] += array(
          'submissions_view' => 'default',
          'user_submissions_view' => 'default',
        );
      }
    }
    $entityform_type = parent::create($values);
    return $entityform_type;
  }

  /**
   * Returns default text values.
   */
  public function get_default_text_values() {
    return array(
      'label' => '',
      'instruction_pre' => '',
      'submit_confirm_msg' => t('Your submission has been saved.'),
      'submission_page_title' => t('Thank You.'),
      'draft_button_text' => t('Save Draft'),
      'submit_button_text' => t('Submit'),
      'your_submissions' => t('Your Submissions: @label', array('@label' => '[entityform_type:label]')),
      'disallow_resubmit_msg' => t('You already submitted this form'),
      'delete_confirm_msg' => t('Are you sure you want to delete this Submission for @label', array('@label' => '[entityform_type:label]?')),
      'draft_save_text' => '',
      'page_title_view' => t('Form Submission: @label', array('@label' => '[entityform_type:label]')),
    );
  }

  /**
   * Returns text labels.
   */
  public function get_text_labels() {
    return array(
      'label' => t('Label'),
      'draft_redirect_path' => t('Draft Redirect path'),
      'draft_button_text' => t('Override draft button text'),
      'draft_save_text' => t('Draft save text'),
      'submit_button_text' => t('Submit Button Text'),
      'submit_confirm_msg' => t('Submission Confirmation Text'),
      'your_submissions' => t('Your Submissions Text'),
      'disallow_resubmit_msg' => t('Form Disallow Resubmit Text'),
      'delete_confirm_msg' => t('Submission Delete Text'),
      'submission_page_title' => t('Thank You.'),
      'page_title_view' => t('Submission View Title'),
      'submission_text' => t('Submission reply'),
      'redirect_path' => t('Redirect path'),
      'instruction_pre' => t('Intro form instructions'),
    );
  }

  /**
   * Overridden to Load file.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    $view = parent::view($entities, $view_mode, $langcode, $page);
    foreach ($entities as $entity_id => $entity) {
      module_load_include('inc', 'entityform', 'entityform.admin');
      $view[$this->entityType][$entity->type]['form'] = entityform_form_wrapper(entityform_empty_load($entity->type), 'submit', ($page ? 'page' : 'embedded'));
    }
    return $view;
  }

  /**
   * Overridden to load paths also.
   */
  public function load($ids = array(), $conditions = array()) {
    $entityform_types = parent::load($ids, $conditions);
    foreach ($entityform_types as $entityform_type) {
      if (module_exists('path')) {
        $entityform_type->paths = array();
        $path_types = _entityform_type_get_path_types($entityform_type->type);
        foreach ($path_types as $key => $path_type) {
          // Check for existing alias.
          $conditions = array('source' => $path_type['default_path']);
          $path = path_load($conditions);
          if ($path !== FALSE) {
            $entityform_type->paths[$key] = $path;
          }
        }
      }
    }
    return $entityform_types;
  }

  /**
   * Overridden to clear cache.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $return = parent::save($entity, $transaction);

    // Reset the entityform type cache. We need to do this first so
    // menu changes pick up our new type.
    entityform_type_cache_reset();
    // Clear field info caches such that any changes to extra fields get
    // reflected.
    field_info_cache_clear();

    return $return;
  }

  /**
   * Overridden to delete aliases and clear cache.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if ($entities) {
      if (module_exists('path')) {
        foreach ($entities as $id => $entity) {
          try {
            $path_types = _entityform_type_get_path_types($entity->type);
            foreach ($path_types as $path_type) {
              path_delete(array('source' => $path_type['default_path']));
            }
          }
          catch (Exception $e) {
          }
        }
      }
      // Delete all menu module links that point to this entityform type.
      $submit_paths = array();
      foreach ($entities as $id => $entity) {
        $submit_paths[] = _entityform_type_get_submit_url($entity->type);
      }
      $result = db_query("SELECT mlid FROM {menu_links} WHERE link_path IN (:path) AND module = 'entityform'", array(':path' => $submit_paths), array('fetch' => PDO::FETCH_ASSOC));
      foreach ($result as $m) {
        menu_link_delete($m['mlid']);
      }
      parent::delete($ids, $transaction);

      // Clear field info caches such that any changes to extra fields get
      // reflected.
      field_info_cache_clear();
      // Reset the entityform type cache.
      entityform_type_cache_reset();
    }
  }

  /**
   * Overridden to exclude pid in alias Export.
   */
  public function export($entity, $prefix = '') {
    if (module_exists('path')) {
      foreach ($entity->paths as &$path) {
        unset($path['pid']);
      }
    }
    return parent::export($entity, $prefix);
  }

}

