<?php

// TODO implement hooks in a for loop

/**
 * Generic function for hook_features_export_options().
 */
function _custom_breadcrumbs_features_features_export_options($cb_type) {
  $options = array();
  $result = db_select($cb_type, 'cb')
    ->fields('cb', array('machine_name', 'name'))
    ->execute();
  foreach ($result as $crumb) {
    $options[$crumb->machine_name] = $crumb->name;
  }
  return $options;
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumb_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumb');
}


/**
 * Generic function for hook_features_export().
 */
function _custom_breadcrumbs_features_features_export($data, &$export, $module_name, $cb_type) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies'][$cb_type] = $cb_type; // TODO pb for voc / terms. use $module_name ?
  foreach ($data as $component) {
    $export['features'][$cb_type][$component] = $component;
  }
  return array(); // TODO weird
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumb_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumb');
}


/**
 * Generic function for hook_features_export_render().
 */
function _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, $cb_type) {
  $code = array();
  $code[] = "  $cb_type = array();";
  $code[] = '';
  foreach ($data as $option) {
    $item = _custom_breadcrumbs_features_export_item($option, $cb_type);
    if (isset($item[$option])) {
      $export = features_var_export($option);
      $identifier = features_var_export($option);
      $code[] = "  // Exported custom breadcrumbs: $option.";
      $code[] = "  \$custom_breadcrumbs[$identifier] = $export;";
      $code[] = "";
    }
  }
  $code[] = "  return $cb_type;";
  $code = import("\n", $code);
  return array($cb_type . '_features_default_settings' => $code);
}

/**
 * Generic function to retrieve breadcrumb data.
 */
function _custom_breadcrumbs_features_export_item($option, $cb_type) {
  $result = db_select($cb_type, 'cb')
    ->fields('cb')
    ->condition('machine_name', $option)
    ->execute();
  $crumb = array();
  if ($result->rowCount() > 0) {
    $crumb = $result->fetchAssoc();
    // We don't need to export the auto-increment value.
    unset($crumb['bid']);
  }
  return array($option => $crumb);
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumb_features_export_render($module_name, $data, $export) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumb');
}


/**
 * Generic function for hook_features_rebuild().
 */
function _custom_breadcrumbs_features_features_rebuild($module, $cb_type) {
  // TODO why do we invoke this hook ?
  $items = module_invoke($module, $cb_type . '_features_default_settings');
  // Loop over breadcrumbs we need to recreate.
  foreach ($items as $option => $item) {
    _custom_breadcrumbs_features_rebuild_item($item, $cb_type);
  }
}

/**
 * Generic function to store a breadcrumb in database.
 */
function _custom_breadcrumbs_features_rebuild_item($item, $cb_type) {
  db_delete($cb_type)
    ->condition('machine_name', $item['machine_name'])
    ->execute();
  db_insert($cb_type) // TODO maybe could use ->fields($item)
    ->fields(array(
      'name'           => $item['name'],
      'machine_name'   => $item['machine_name'],
      'titles'         => $item['titles'],
      'paths'          => $item['paths'],
      'visibility_php' => $item['visibility_php'],
      'language'       => $item['language'],
      'node_type'      => $item['node_type'],
    ))->execute();
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumb_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumb');
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumb_features_revert($module) {
  custom_breadcrumb_features_rebuild($module);
}
