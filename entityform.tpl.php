<?php
/**
 * @file
 * A basic template for entityform entities.
 *
 * Available variables:
 * - $content: An array of field items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The name of the entityform
 * - $url: The standard URL for viewing a entityform entity
 * - $page: TRUE if this is the main view page $url points too.
 * - $classes: Array of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-profile
 *   - entityform-{TYPE}
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
?>
<div class="<?php print implode(' ', $classes); ?> clearfix"<?php (empty($attributes)) ? '' : print backdrop_attributes($attributes); ?>>
  <?php if (!$page) : ?>
    <h2<?php print $title_attributes; ?>>
      <?php if (!empty($url)) : ?>
        <a href="<?php print $url; ?>"><?php print $title; ?></a>
      <?php else : ?>
        <?php print $title; ?>
      <?php endif; ?>
    </h2>
  <?php endif; ?>

  <div class="content"<?php (empty($content_attributes)) ? '' : print backdrop_attributes($content_attributes); ?>>
    <?php print render($content); ?>
  </div>
</div>
