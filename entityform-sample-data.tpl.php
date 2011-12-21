<?php

/**
 * @file
 * Example tpl file for theming a single entityform-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $entityform: The Entityform object this status is derived from
 */
return;
?>

<div class="entityform-status">
  <?php print '<strong>Entityform Sample Data:</strong> ' . $entityform_sample_data = ($entityform_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>