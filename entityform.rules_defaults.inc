<?php
/*
 * @file Default Rules
*/
/**
 * Implements hook_default_rules_configuration().
 */
function entityform_default_rules_configuration() {
  $exported_rules[] = '{ "rules_email_entityform_submitter" : { "LABEL" : "Email Entityform Submitter", "PLUGIN" : "rule", "TAGS" : [ "entityform submission" ], "REQUIRES" : [ "rules" ], "USES VARIABLES" : { "submitted_entityform" : { "label" : "Submitted Entityform", "type" : "entityform" }, "entityform_type" : { "label" : "Entityform Type", "type" : "entityform_type" } }, "IF" : [ { "NOT user_has_role" : { "account" : [ "submitted-entityform:user" ], "roles" : { "value" : { "1" : "1" } } } }, { "component_rules_submitted_entityform_notify" : { "submitted_entityform" : [ "submitted_entityform" ] } } ], "DO" : [ { "mail" : { "to" : [ "submitted-entityform:user:mail" ], "subject" : "Thanks for the submitting the form [entityform-type:label]\r\n\r\n", "message" : "We have received your submission of the form [entityform-type:label].\r\n\r\n[entityform-type:notification-text]", "language" : [ "" ] } } ] } }';
  $exported_rules[] = '{ "rules_email_entityform_admin" : { "LABEL" : "Email Entityform Admin", "PLUGIN" : "rule", "TAGS" : [ "entityform submission" ], "REQUIRES" : [ "rules" ], "USES VARIABLES" : { "submitted_entityform" : { "label" : "Submitted Entityform", "type" : "entityform" }, "entityform_type" : { "label" : "Entityform Type", "type" : "entityform_type" } }, "IF" : [ { "component_rules_submitted_entityform_notify" : { "submitted_entityform" : [ "submitted-entityform" ] } }, { "NOT data_is_empty" : { "data" : [ "entityform-type:notify-emails" ] } } ], "DO" : [ { "mail" : { "to" : "[entityform-type:notify-emails]", "subject" : "Form submission notification: [entityform-type:label]", "message" : "Form submission notification: [entityform-type:label]\r\nSubmitted by: [submitted-entityform:user]\r\nDate: [submitted-entityform:changed]", "language" : [ "" ] } } ] } }';
  $exported_rules[] = '{ "rules_submitted_entityform_notify" : { "LABEL" : "Submitted Entityform Notify", "PLUGIN" : "and", "TAGS" : [ "enityform" ], "REQUIRES" : [ "rules" ], "USES VARIABLES" : { "submitted_entityform" : { "label" : "Submitted Entityform", "type" : "entityform" } }, "AND" : [ { "data_is_empty" : { "data" : [ "submitted-entityform:draft" ] } }, { "data_is" : { "data" : [ "submitted-entityform:uid" ], "value" : [ "site:current-user:uid" ] } } ] } }';
  foreach ($exported_rules as $exported_rule) {
    $rule = rules_import($exported_rule);
    $configs[$rule->name] = $rule;
  }

  return $configs;
}