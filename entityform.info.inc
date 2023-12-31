<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class EntityformMetadataController extends EntityDefaultMetadataController {

  /**
   * {@inheritdoc}
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The user who submitted the form."),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'administer entityform types',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    $properties['created'] = array(
      'label' => t("Date submitted"),
      'type' => 'date',
      'description' => t("The date form was submitted."),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer entityform types',
      'required' => TRUE,
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the form was most recently updated."),
    );
    // The type property is created in parent::entityPropertyInfo().
    $properties['type']['label'] = t("Entityform Type");
    $properties['type']['type'] = 'entityform_type';
    $properties['type']['schema field'] = 'type';
    $properties['type']['description'] = t("The Entityform Type for the Entityform Submission.");
    // @todo This line could be removed depending on this https://drupal.org/node/1931376
    $properties['type']['required'] = TRUE;

    $properties['draft']['setter callback'] = 'entity_property_verbatim_set';

    return $info;
  }

}
/**
 * Extend the defaults.
 */
class EntityformTypeMetadataController extends EntityDefaultMetadataController {

  /**
   * {@inheritdoc}
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['type']['type'] = 'text';
    // Add properties for translatable fields.
    $labels = entity_get_controller('entityform_type')->get_text_labels();
    foreach ($labels as $text_prop => $label) {
      if (!isset($properties[$text_prop])) {
        $properties[$text_prop] = array();
      }
      $properties[$text_prop] += array(
        'label' => $label,
        'getter callback' => 'entityformtype_metadata_get_properties',
        'setter callback' => 'entityformtype_metadata_set_properties',
        'type' => 'text',
        'translatable' => TRUE,
        'i18n string' => TRUE,
      );
      // Testing if this will allow property to be translated.
      // @todo remove flag all formatted properties.
      if (in_array($text_prop, array('instruction_pre', 'submission_text'))) {
        $properties[$text_prop]['format'] = 'format';
      }
    }
    return $info;
  }

}
