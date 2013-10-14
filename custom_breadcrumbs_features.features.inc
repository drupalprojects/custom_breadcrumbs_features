<?php

// TODO implement hooks in a for loop

/**
 * Generic function for hook_features_export_options().
 */
function _custom_breadcrumbs_features_features_export_options($table) {
  $options = array();
  $result = db_select($table, 'cb')
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
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbsapi_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbsapi');
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbs_views_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbs_views');
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbs_taxonomy_vocabulary_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbs_taxonomy_vocabulary');
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbs_taxonomy_term_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbs_taxonomy_term');
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbs_paths_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbs_paths');
}

/**
 * Implements hook_features_export_options(). [component hook]
 */
function custom_breadcrumbs_panels_features_export_options() {
  return _custom_breadcrumbs_features_features_export_options('custom_breadcrumbs_panels');
}


/**
 * Generic function for hook_features_export().
 */
function _custom_breadcrumbs_features_features_export($data, &$export, $module_name, $table) {
  $types = _custom_breadcrumbs_features_get_types();
  $module = $types[$table]['module']; // TODO use $module_name ?
  $export['dependencies']['features'] = 'features';
  $export['dependencies'][$module] = $module; // TODO [$table] = $module ? cf terms & vocabularies
  foreach ($data as $component) {
    $export['features'][$table][$component] = $component;
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
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbsapi_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbsapi');
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbs_views_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbs_views');
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbs_taxonomy_vocabulary_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbs_taxonomy_vocabulary');
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbs_taxonomy_term_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbs_taxonomy_term');
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbs_paths_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbs_paths');
}

/**
 * Implements hook_features_export(). [component hook]
 */
function custom_breadcrumbs_panels_features_export($data, &$export, $module_name) {
  return _custom_breadcrumbs_features_features_export($data, $export, $module_name, 'custom_breadcrumbs_panels');
}


/**
 * Generic function for hook_features_export_render().
 */
function _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, $table) {
  $code = array();
  $code[] = "  $table = array();";
  $code[] = '';
  foreach ($data as $option) {
    $item = _custom_breadcrumbs_features_export_item($option, $table);
    if (isset($item[$option])) {
      $export = features_var_export($option);
      $identifier = features_var_export($option);
      $code[] = "  // Exported custom breadcrumb: $option.";
      $code[] = "  \$custom_breadcrumbs[$identifier] = $export;";
      $code[] = "";
    }
  }
  $code[] = "  return $table;";
  $code = import("\n", $code);
  return array($table . '_features_default_settings' => $code);
}

/**
 * Generic function to retrieve breadcrumb data.
 */
function _custom_breadcrumbs_features_export_item($option, $table) {
  $result = db_select($table, 'cb')
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
function custom_breadcrumb_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumb');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbsapi_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbsapi');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbs_views_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbs_views');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbs_taxonomy_vocabulary_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbs_taxonomy_vocabulary');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbs_taxonomy_term_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbs_taxonomy_term');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbs_paths_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbs_paths');
}

/**
 * Implements hook_features_export_render(). [component hook]
 */
function custom_breadcrumbs_panels_features_export_render($module_name, $data, $export = NULL) {
  return _custom_breadcrumbs_features_features_export_render($module_name, $data, $export, 'custom_breadcrumbs_panels');
}


/**
 * Generic function for hook_features_rebuild().
 */
function _custom_breadcrumbs_features_features_rebuild($module, $table) {
  // TODO why do we invoke this hook ?
  $items = module_invoke($module, $table . '_features_default_settings');
  // Loop over breadcrumbs we need to recreate.
  foreach ($items as $option => $item) {
    _custom_breadcrumbs_features_rebuild_item($item, $table);
  }
}

/**
 * Generic function to store a breadcrumb in database.
 */
function _custom_breadcrumbs_features_rebuild_item($item, $table) {
  db_delete($table)
    ->condition('machine_name', $item['machine_name'])
    ->execute();
  db_insert($table)
    ->fields(array(
      'name'           => $item['name'],
      'machine_name'   => $item['machine_name'],
      'titles'         => $item['titles'],
      'paths'          => $item['paths'],
      'visibility_php' => $item['visibility_php'],
      'language'       => $item['language'],
      'node_type'      => $item['node_type'], // TODO term defines tid, path defines specific_path, etc -> maybe could use ->fields($item)
    ))->execute();
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumb_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumb');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbsapi_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbsapi');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbs_views_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbs_views');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbs_taxonomy_vocabulary_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbs_taxonomy_vocabulary');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbs_taxonomy_term_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbs_taxonomy_term');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbs_paths_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbs_paths');
}

/**
 * Implements hook_features_rebuild(). [component hook]
 */
function custom_breadcrumbs_panels_features_rebuild($module) {
  _custom_breadcrumbs_features_features_rebuild($module, 'custom_breadcrumbs_panels');
}


/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumb_features_revert($module) {
  custom_breadcrumb_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbsapi_features_revert($module) {
  custom_breadcrumbsapi_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbs_views_features_revert($module) {
  custom_breadcrumbs_views_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbs_taxonomy_vocabulary_features_revert($module) {
  custom_breadcrumbs_taxonomy_vocabulary_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbs_taxonomy_term_features_revert($module) {
  custom_breadcrumbs_taxonomy_term_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbs_paths_features_revert($module) {
  custom_breadcrumbs_paths_features_rebuild($module);
}

/**
 * Implements hook_features_revert(). [component hook]
 */
function custom_breadcrumbs_panelsfeatures_revert($module) {
  custom_breadcrumbs_panels_features_rebuild($module);
}
