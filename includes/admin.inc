<?php
// $Id$

/**
 * Page callback for quicktabs admin landing page.
 */
function quicktabs_list() {
  $result = db_query('SELECT qtid, title FROM {quicktabs} ORDER BY title');
  $header = array(
    array('data' => t('Quick Tabs Block Name')),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $tablerow = array(
      array('data' => $row->title),
      array('data' => l('Edit', 'admin/build/quicktabs/edit/'. $row->qtid)),
      array('data' => l('Delete', 'admin/build/quicktabs/delete/'. $row->qtid)),
    );
    $rows[] = $tablerow;
  }
  $output = theme('table', $header, $rows, array('id' => 'quicktabs'));
  return $output;
}


function quicktabs_new($ajax = FALSE) {
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_form.js');
  drupal_add_css(drupal_get_path('module', 'quicktabs') .'/quicktabs.css');
  if (!$ajax) {
    return drupal_get_form('quicktabs_form');
  } else {
    return drupal_get_form('quicktabs_aj_form');
  }
}

/**
 * build the Quick Tabs (normal) creation and edit form
 */
function quicktabs_form($form_state = NULL, $quicktabs = NULL) {
  // Add our JS file, which has some Drupal core JS overrides, and ensures ahah behaviours get re-attached
  drupal_add_js(drupal_get_path('module', 'quicktabs') . '/js/quicktabs_ahah.js', 'footer');

  $form = _qt_admin_main_form($form_state, $quicktabs, FALSE);

  $formtype = $quicktabs['formtype'];
  if ($formtype == 'edit') {

    $form['qtid'] = array(
    '#type' => 'hidden',
    '#value' => $quicktabs['qtid']
    );
  }

  $tabcontent = $quicktabs['tabs'];

  if (isset($form_state['qt_count'])) {
    $qt_count = $form_state['qt_count'];
  }
  else {
    $qt_count = max(2, empty($tabcontent) ? 2 : count($tabcontent));
  }

  // Add the current tabs to the form.
  for ($delta = 0; $delta < $qt_count; $delta++) {

    $weight = isset($tabcontent[$delta]['tabweight']) ? $tabcontent[$delta]['tabweight'] : $delta-10;
    $title = isset($tabcontent[$delta]['tabtext']) ? $tabcontent[$delta]['tabtext'] : '';
    $type = isset($tabcontent[$delta]['tabtype']) ? $tabcontent[$delta]['tabtype'] : 'block';
    $bid = isset($tabcontent[$delta]['bid']) ? $tabcontent[$delta]['bid'] : NULL;
    $hide_title = isset($tabcontent[$delta]['hide_title']) ? $tabcontent[$delta]['hide_title'] : 1;
    $vid = isset($tabcontent[$delta]['vid']) ? $tabcontent[$delta]['vid'] : 'default';
    $args = isset($tabcontent[$delta]['args']) ? $tabcontent[$delta]['args'] : NULL;
    $display = isset($tabcontent[$delta]['display']) ? $tabcontent[$delta]['display'] : 'default';
    $details = array(
      'delta' => $delta,
      'weight' => $weight,
      'title' => $title,
      'type' => $type,
      'bid' => $bid,
      'hide_title' => $hide_title,
      'vid' => $vid,
      'args' => $args,
      'display' => $display,
    );
    $form['qt_wrapper']['tabs'][$delta] = _quicktabs_form($details, FALSE);
  }

  return $form;
}

/**
 * build the Quick Tabs (ajax) creation and edit form
 */
function quicktabs_aj_form($form_state = NULL, $quicktabs = NULL) {
  // Add our JS file, which has some Drupal core JS overrides, and ensures ahah behaviours get re-attached
  drupal_add_js(drupal_get_path('module', 'quicktabs') . '/js/quicktabs_ahah.js', 'footer');

  $form = _qt_admin_main_form($form_state, $quicktabs, TRUE);

  $formtype = $quicktabs['formtype'];
  if ($formtype == 'edit') {

    $form['qtid'] = array(
      '#type' => 'hidden',
      '#value' => $quicktabs['qtid']
    );
  }

  $tabcontent = $quicktabs['tabs'];

  if (isset($form_state['qt_count'])) {
    $qt_count = $form_state['qt_count'];
  }
  else {
    $qt_count = max(2, empty($tabcontent) ? 2 : count($tabcontent));
  }

  // Add the current tabs to the form.
  for ($delta = 0; $delta < $qt_count; $delta++) {

    $weight = isset($tabcontent[$delta]['tabweight']) ? $tabcontent[$delta]['tabweight'] : $delta-10;
    $title = isset($tabcontent[$delta]['tabtext']) ? $tabcontent[$delta]['tabtext'] : '';
    $type = isset($tabcontent[$delta]['tabtype']) ? $tabcontent[$delta]['tabtype'] : 'node';
    $nid = isset($tabcontent[$delta]['nid']) ? $tabcontent[$delta]['nid'] : NULL;
    $vid = isset($tabcontent[$delta]['vid']) ? $tabcontent[$delta]['vid'] : 'default';
    $args = isset($tabcontent[$delta]['args']) ? $tabcontent[$delta]['args'] : NULL;
    $display = isset($tabcontent[$delta]['display']) ? $tabcontent[$delta]['display'] : 'default';
    $details = array(
      'delta' => $delta,
      'weight' => $weight,
      'title' => $title,
      'type' => $type,
      'nid' => $nid,
      'vid' => $vid,
      'args' => $args,
      'display' => $display,
    );
    $form['qt_wrapper']['tabs'][$delta] = _quicktabs_form($details, TRUE);
  }
  return $form;
}

/*
 * The main section of admin page.
 */
function _qt_admin_main_form($form_state = NULL, &$quicktabs = NULL, $ajax = FALSE) {
  $form = array(
    '#cache' => TRUE,
  );
  // the contents of $quicktabs will either come from the db or from $form_state
  if (isset($form_state['quicktabs'])) {
    $quicktabs = $form_state['quicktabs'] + (array)$quicktabs;
  }

  $form['title'] = array(
  '#title' => t('Block Title'),
  '#type' => 'textfield',
  '#description' => t('The title of the whole block'),
  '#default_value' => ($quicktabs['title'] ? $quicktabs['title'] : ''),
  '#weight' => -5
  );

  // Add a wrapper for the tabs and Add Another Tab button.
  $form['qt_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -4,
    '#prefix' => '<div class="clear-block" id="quicktabs-tabs-wrapper">',
    '#suffix' => '</div>',
  );

  $form['qt_wrapper']['tabs'] = array(
    '#prefix' => '<div id="quicktabs-tabs">',
    '#suffix' => '</div>',
    '#theme' => 'qt_tabs',
  );

  $form['qt_wrapper']['tabs_more'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="add-more-tabs-button">',
    '#suffix' => '<label for="edit-tabs-more">' . t('Add tab') . '</label></div>',
    '#value' => t('More tabs'),
    '#description' => t("Click here to add more tabs."),
    '#attributes' => array('class' => 'add-tab'),
    '#weight' => 1,
    '#submit' => array('qt_more_tabs_submit'), // If no javascript action.
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['qt_wrapper']['remove'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="delete-tabs-button">',
    '#suffix' => '<label for="edit-remove">' . t('Remove last tab') . '</label></div>',
    '#value' => t('remove'),
    '#attributes' => array('class' => 'delete-tab'),
    '#submit' => array('qt_remove_tab_submit'),
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['submit_form'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => t('Save'),
  );

  return $form;
}

/*
 * Build one row (one tabpage) on the QT admin form.
 */
function _quicktabs_form($details, $ajax = FALSE) {
  $form['#tree'] = TRUE;
  $delta = $details['delta'];

  // We'll manually set the #parents property of these fields so that
  // their values appear in the $form_state['values']['tabs'] array.
  $form['tabweight'] = array(
    '#type' => 'weight',
    '#default_value' => $details['weight'],
    '#parents' => array('tabs', $delta, 'tabweight'),
  );

  $form['tabtext'] = array(
    '#type' => 'textfield',
    '#size' => '10',
    '#title' => t('Tab @n', array('@n' => ($delta + 1))),
    '#default_value' => $details['title'],
    '#parents' => array('tabs', $delta, 'tabtext'),
  );

  if (module_exists('views')) {
    $views = quicktabs_get_views();
    if (!$ajax) {
      $blocks = quicktabs_get_blocks();

      $form['type_options'] = array(
      '#type' => 'value',
      '#value' => array(
      'block' => 'block',
      'view' => 'view',
      )
      );
    }
    else {
      $nodes = quicktabs_get_nodes();

      $form['type_options'] = array(
      '#type' => 'value',
      '#value' => array(
      'node' => 'node',
      'view' => 'view',
      )
      );
    }
    $form['tabtype'] = array(
    '#type' => 'radios',
    '#options' => $form['type_options']['#value'],
    '#title' => t('Tab Content'),
    '#default_value' => $details['type'],
    '#parents' => array('tabs', $delta, 'tabtype'),
    );
    $views_keys = array_keys($views);
    $selected_view = ($details['vid'] ? ($details['vid'] == 'default' ? $views_keys[0] : ($details['type'] != 'view' ? $views_keys[0] : $details['vid'])) : $views_keys[0]);
    $form['vid'] = array(
    '#type' => 'select',
    '#options' => $views,
    '#default_value' => $selected_view,
    '#title' => t('Select a view'),
    '#parents' => array('tabs', $delta, 'vid'),
    '#ahah' => array(
      'path' => 'quicktabs/ahah/',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'event' => 'change',
      ),
    );
    $form['args'] = array(
    '#type' => 'textfield',
    '#title' => 'arguments',
    '#size' => '10',
    '#required' => false,
    '#default_value' => $details['args'],
    '#description' => t('Provide a comma separated list of arguments to pass to the view.'),
    '#parents' => array('tabs', $delta, 'args')
    );

    $form['display'] = array(
    '#type' => 'select',
    '#title' => 'display',
    '#options' => _quicktabs_get_views_displays($selected_view),
    '#default_value' => $details['display'],
    '#description' => t('Choose a display for your view.'),
    '#parents' => array('tabs', $delta, 'display')
    );
    $form['get_displays'] = array(
    '#type' => 'submit',
    '#value' => 'vdisp_'. $delta,
    '#parents' => array('tabs', $delta, 'get_displays'),
    '#submit' => array('qt_get_displays_submit'),
    );
  } else {
    $form['tabtype'] = array(
      '#type' => 'hidden',
      '#title' => t('Type for tab @n', array('@n' => ($delta + 1))),
      '#value' => $details['type'],
      '#parents' => array('tabs', $delta, 'tabtype'),
    );
  }
  if (!$ajax) {
    $form['bid'] = array(
      '#type' => 'select',
      '#options' => $blocks,
      '#default_value' => $details['bid'],
      '#title' => t('Select a block'),
      '#parents' => array('tabs', $delta, 'bid'),
    );
    $form['hide_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the title of this block'),
      '#default_value' => $details['hide_title'],
      '#parents' => array('tabs', $delta, 'hide_title'),
    );
  }
  else {
    $form['nid'] = array(
      '#type' => 'select',
      '#options' => $nodes,
      '#default_value' => $details['nid'],
      '#title' => t('Select a node'),
      '#parents' => array('tabs', $delta, 'nid'),
    );
  }
  return $form;
}

/**
 * Theme function for quicktabs admin page.
 * Theme the form elements for the tabs as draggable table rows.
 *
 * @ingroup themeable
 */
function theme_qt_tabs($form) {
  drupal_add_tabledrag('qt-tablist-table', 'order', 'sibling', 'qt-tabs-weight');
  $rows = array();
  $headers = array(
    t('Tab Title'),
    t('Weight'),
    module_exists('views') ? t('Tab type') : '',
    t('Content'),
  );

  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
    unset($form[$key]['tabtext']['#title'], $form[$key]['tabtype']['#title']);
    $form[$key]['tabweight']['#attributes']['class'] = 'qt-tabs-weight';
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['tabtext']), 'class' => 'qt-tab-title'),
        array('data' => drupal_render($form[$key]['tabweight']), 'class' => 'qt-tab-weight'),
        module_exists('views') ? array('data' => drupal_render($form[$key]['tabtype']), 'class' => 'qt-tab-type') : array('data' => '', 'class' => 'qt-tabtype-hidden'),
        array('data' => ($form[$key]['bid'] ? drupal_render($form[$key]['bid']) . drupal_render($form[$key]['hide_title']) : drupal_render($form[$key]['nid'])) . (module_exists('views') ? drupal_render($form[$key]['vid']) . drupal_render($form[$key]['args']) . drupal_render($form[$key]['limit']) . drupal_render($form[$key]['display']) . drupal_render($form[$key]['get_displays']) : ''), 'class' => 'qt-tab-bvid'),
      ),
      'class' => 'draggable',
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', $headers, $rows, array('id' => 'qt-tablist-table'));
  $output .= drupal_render($form);
  return $output;
}



/**
 * Submit handler for the "Add Tab" button.
 */
function qt_more_tabs_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $quicktabs = $form_state['values'];
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
  if ($form_state['values']['tabs_more']) {
    if (isset($form_state['values']['tabs'])) {
      $form_state['qt_count'] = count($form_state['values']['tabs']) + 1;
    }
    else {
      $form_state['qt_count'] = 1;
    }
  }
  return $quicktabs;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function qt_remove_tab_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $quicktabs = $form_state['values'];
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
  unset($form['qt_wrapper']['tabs'][0]);
  if (isset($form_state['values']['tabs'])) {
    $form_state['values']['tabs'] = array_values($form_state['values']['tabs']);
    $form_state['qt_count'] = count($form_state['values']['tabs']) - 1;
  }
  else {
    $form_state['qt_count'] = 0;
  }
  return $quicktabs;
}

/**
 * Submit handler for the Views drop down.
 */
function qt_get_displays_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $quicktabs = $form_state['values'];
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
  return $quicktabs;
}

/**
 * ahah callback.
 */
function quicktabs_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $qt_form = $form['qt_wrapper']['tabs'];
  unset($qt_form['#prefix'], $qt_form['#suffix']); // Prevent duplicate wrappers.
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status'   => TRUE,
    'data'     => theme('status_messages') . drupal_render($qt_form),
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * Validation handler for quicktabs admin page.
 */
function quicktabs_form_validate($form, &$form_state) {
  // We don't want it to validate when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    if (empty($form_state['values']['title'])) {
      form_set_error('title', t('You must give your Quick Tabs block a title.'));
    }
    else {
      if (!isset($form_state['values']['tabs'])) {
        form_set_error('', t('At least one tab should be created.'));
      }
      foreach ($form_state['values']['tabs'] as $j => $tab) {
        if (empty($tab['tabtext'])) {
          form_set_error('tabs]['. $j .'][tabtext', t('You must give each tab a title.'));
        }
      }
    }
  }
}

/**
 * Submit handler for quicktabs admin page.
 */
function quicktabs_form_submit($form, &$form_state) {
  // We don't want it to submit when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    $formvalues_tabs = array();
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      $formvalues_tabs[] = array(
      'title' => $form_state['values']['tabs'][$j]['tabtext'],
      'type' => $form_state['values']['tabs'][$j]['tabtype'],
      'bvid' => ($form_state['values']['tabs'][$j]['tabtype'] == 'block' ? $form_state['values']['tabs'][$j]['bid'] : $form_state['values']['tabs'][$j]['vid']),
      'hide_title' => ($form_state['values']['tabs'][$j]['tabtype'] == 'block' ? $form_state['values']['tabs'][$j]['hide_title'] : 0),
      'weight' => $form_state['values']['tabs'][$j]['tabweight'],
      'args' => ($form_state['values']['tabs'][$j]['tabtype'] == 'block' ? '' : $form_state['values']['tabs'][$j]['args']),
      'display' => ($form_state['values']['tabs'][$j]['tabtype'] == 'block' ? '' : $form_state['values']['tabs'][$j]['display']),
      );
    }
    $fullcontent = array(
      'blocktitle' => $form_state['values']['title'],
      'blockcontent' => $formvalues_tabs,
    );

    if ($form_state['values']['qtid']) {
      $qtid = $form_state['values']['qtid'];
      quicktabs_updateblock($qtid, $fullcontent);
      $msg = t('Your Quick Tabs block has been updated.');
    }
    else {
      quicktabs_createblock($fullcontent);
      $msg = t('Your Quick Tabs block has been created and can now be enabled.');
    }
    drupal_set_message($msg);
    drupal_goto('admin/build/quicktabs');
  }
}

function quicktabs_aj_form_validate($form, &$form_state) {
  // we don't want it to validate when we're just adding or removing tabs
  if ($form_state['values']['op'] == t('Save')) {
    if (empty($form_state['values']['title'])) {
      form_set_error('title', t('You must give your Quick Tabs block a title.'));
    }
    else {
      foreach ($form_state['values']['tabs'] as $j => $tab) {
        if (empty($tab['tabtext'])) {
          form_set_error('tabs]['. $j .'][tabtext', t('You must give each tab a title.'));
        }
      }
    }
  }
}

function quicktabs_aj_form_submit($form, $form_state) {
  // we don't want it to submit when we're just adding or removing tabs
  if ($form_state['values']['op'] == t('Save')) {
    $formvalues_tabs = array();
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      $formvalues_tabs[] = array(
      'title' => $form_state['values']['tabs'][$j]['tabtext'],
      'type' => $form_state['values']['tabs'][$j]['tabtype'],
      'bnid' => ($form_state['values']['tabs'][$j]['tabtype'] == 'node' ? $form_state['values']['tabs'][$j]['nid'] : $form_state['values']['tabs'][$j]['vid']),
      'weight' => $form_state['values']['tabs'][$j]['tabweight'],
      'args' => ($form_state['values']['tabs'][$j]['type'] == 'node' ? '' : $form_state['values']['tabs'][$j]['args']),
      'display' => ($form_state['values']['tabs'][$j]['type'] == 'node' ? '' : $form_state['values']['tabs'][$j]['display']),
      );
    }
    $fullcontent = array(
      'blocktitle' => $form_state['values']['title'],
      'blockcontent' => $formvalues_tabs,
    );

    if ($form_state['values']['qtid']) {
      $qtid = $form_state['values']['qtid'];
      quicktabs_updateblock($qtid, $fullcontent);
      $msg = t('Your Quick Tabs block has been updated.');
    }
    else {
      quicktabs_createblock($fullcontent, TRUE);
      $msg = t('Your Quick Tabs block has been created and can now be enabled.');
    }
    drupal_set_message($msg);
    drupal_goto('admin/build/quicktabs');
  }
}

/**
 * Insert db record.
 */
function quicktabs_createblock($fullcontent, $ajax = FALSE) {
  $qtid = db_last_insert_id('quicktabs', 'qtid');
  $title = $fullcontent['blocktitle'];
  $tabs = serialize($fullcontent['blockcontent']);
  db_query("INSERT INTO {quicktabs} (qtid, ajax, title, tabs) VALUES(%d, %d, '%s', '%s')", $qtid, $ajax, $title, $tabs);
}

/**
 * Update db record.
 */
function quicktabs_updateblock($qtid, $fullcontent) {
  $title = $fullcontent['blocktitle'];
  $tabs = serialize($fullcontent['blockcontent']);
  db_query("UPDATE {quicktabs} SET title='%s', tabs='%s' WHERE qtid=%d", $title, $tabs, $qtid);
}

/**
 * Edit Quick Tabs block.
 */
function quicktabs_block_edit(array $quicktabs) {
    foreach ($quicktabs['tabs'] as $key => $tab) {
      $formtabs[] = array(
        'tabweight' => $tab['weight'],
        'tabtext' => $tab['title'],
        'tabtype' => $tab['type'],
        'bid' => ($quicktabs['ajax'] == 0 ? ($tab['type'] == 'block' ? $tab['bvid'] : NULL) : NULL),
        'hide_title' => ($quicktabs['ajax'] == 0 ? ($tab['type'] == 'block' ? $tab['hide_title'] : NULL) : NULL),
        'nid' => ($quicktabs['ajax'] == 1 ? ($tab['type'] == 'node' ? $tab['bnid'] : NULL) : NULL),
        'vid' => $tab['type'] == 'view' ? ($quicktabs['ajax'] == 0 ? $tab['bvid'] : $tab['bnid']) : NULL,
        'args' => $tab['type'] == 'view' ? $tab['args'] : NULL,
        'display' => $tab['type'] == 'view' ? $tab['display'] : NULL,
      );
    }

    $quicktabs['tabs'] = $formtabs;
    $quicktabs['formtype'] = 'edit';

    drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_form.js');
    drupal_add_css(drupal_get_path('module', 'quicktabs') .'/quicktabs.css');
    if ($quicktabs['ajax'] == 0) {
      $output = drupal_get_form('quicktabs_form', $quicktabs);
    } else {
      $output = drupal_get_form('quicktabs_aj_form', $quicktabs);
    }
    return $output;
}

/**
 * Deletion of Quick Tabs blocks.
 */
function quicktabs_block_delete(&$form_state, array $quicktabs) {
  $form['qtid'] = array('#type' => 'hidden', '#value' => $quicktabs['qtid']);
  $form['qt_name'] = array('#type' => 'hidden', '#value' => $quicktabs['title']);
  return confirm_form($form, t('Are you sure you want to delete the Quick Tabs block %name?', array('%name' => $quicktabs['title'])), 'admin/build/quicktabs', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for Quick Tabs blocks deletion.
 */
function quicktabs_block_delete_submit($form, $form_state) {
  db_query('DELETE FROM {quicktabs} WHERE qtid = %d', $form_state['values']['qtid']);
  drupal_set_message(t('The Quick Tabs block %name has been removed.', array('%name' => $form_state['values']['qt_name'])));
  cache_clear_all();
  drupal_goto('admin/build/quicktabs');
};

/**
 * Callback function for admin/settings/quicktabs. Display the settings form.
 */
function quicktabs_settings() {
  $form = array();
  $form['quicktabs_tabstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Tab Style'),
    '#options' => array('default' => t('Default (no style)')) + module_invoke_all('quicktabs_tabstyles'),
    '#default_value' => variable_get('quicktabs_tabstyle', 'default'),
    '#description' => t('Choose a tab style for your Quick Tabs blocks.'),
    '#attributes' => array('class' => 'quicktabs-tabstyles clear-block'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Submit handler for QuickTabs settings.
 */
function quicktabs_settings_submit($form, $form_state) {
  variable_set('quicktabs_tabstyle', $form_state['values']['quicktabs_tabstyle']);
  variable_set('quicktabs_blocktitles', $form_state['values']['quicktabs_blocktitles']);
  //variable_set('quicktabs_viewbuild', $form_state['values']['quicktabs_viewbuild']);
}

/**
 * Theme function for quicktabs settings page.
 *
 * @ingroup themeable
 */
function theme_quicktabs_settings($form) {
  quicktabs_add_css();
  drupal_set_title(t('Quick Tabs Settings'));

  // Default preview.
  $form['quicktabs_tabstyle']['default']['#description'] = 'Default '. t('Preview') .':<br /><iframe class="quicktabs-preview" scrolling="no" src="'. url('quicktabs/preview') .'"></iframe>';

  // Preview for each style.
  $tabstyle_number = 0;
  foreach (element_children($form['quicktabs_tabstyle']) as $tabstyle_key) {
    if ($tabstyle_key != 'default') {
      $form['quicktabs_tabstyle'][$tabstyle_key]['#description'] = $form['quicktabs_tabstyle'][$tabstyle_key]['#title'] .' '. t('Preview') .':<br /><iframe class="quicktabs-preview" scrolling="no" src="'. url('quicktabs/preview/'. $tabstyle_number) .'"></iframe>';
      $tabstyle_number++;
    }

  }
  return drupal_render($form);
}

/**
 * Callback function for quicktabs/preview.
 */
function quicktabs_preview_page($tabstyle_number = NULL) {
  $tabstyles = module_invoke_all('quicktabs_tabstyles');
  $css_files = array_keys($tabstyles);
  if (isset($css_files[$tabstyle_number])) {
    quicktabs_add_css($css_files[$tabstyle_number]);
  }
  else {
    quicktabs_add_css('default');
  }
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs.js');
  print theme('quicktabs_preview_page');
  exit;
}

/**
 * Theme function for quicktabs preview page.
 *
 * @ingroup themeable
 */
function theme_quicktabs_preview_page() {
  $output = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  $output .= '<html xmlns="http://www.w3.org/1999/xhtml">';
  $output .= '<head>';
  $output .= ' <title>'. t('Quick Tabs Preview') .'</title>';
  $output .= drupal_get_html_head();
  $output .= drupal_get_css();
  $output .= drupal_get_js();
  $output .= ' </head>';
  $output .= ' <body style="background: none; background-color: #fff; color: #000;" class="quicktabs_iframe">';
  $output .= '<div class="quicktabs_wrapper">';
  $output .= '<ul class="quicktabs_tabs"><li><a href="#">One</a></li><li><a href="#">Two</a></li></ul>';
  $output .= '<div class="quicktabs_main"><div class="quicktabs"></div><div class="quicktabs"></div></div></div>';

  $output .= '</body></html>';

  return $output;
}

/**
 * Helper function to get all views.
 */
function quicktabs_get_views() {
  $enabled_views = array();
  $views = views_get_all_views();

  foreach ($views as $view) {
    // Skip disabled views.
    if (!empty($views[$view->name]->disabled)) {
      continue;
    }
    $enabled_views[$view->name] = $view->name;
  }
  ksort($enabled_views);
  return $enabled_views;
}

/**
 * Helper function to get all blocks.
 */
function quicktabs_get_blocks() {
  static $blocksarray;
  if (empty($blocksarray)) {
    global $theme_key;
    init_theme();
    $result = db_query("SELECT * FROM {blocks} WHERE theme = '%s'", $theme_key);
    $old_blocks = array();
    while ($old_block = db_fetch_array($result)) {
      $old_blocks[$old_block['module']][$old_block['delta']] = $old_block;
    }
    $blocks = array();
    foreach (module_list() as $module) {
      $module_blocks = module_invoke($module, 'block', 'list');
      if ($module_blocks) {
        foreach ($module_blocks as $delta => $block) {
          if (empty($old_blocks[$module][$delta])) {
            // If it's a new block, add identifiers.
            $block['module'] = $module;
            $block['delta']  = $delta;
            $block['theme']  = $theme_key;
            $blocks[] = $block;
          }
          else {
            $old_blocks[$module][$delta]['info'] = $block['info'];
            // Add this block to the list of blocks we return.
            $blocks[] = $old_blocks[$module][$delta];
          }
        }
      }
    }
    $blocksarray = array();
    foreach ($blocks as $i => $block) {
      if ($block['module'] != 'quicktabs') {
        $key = $block['module'] .'_delta_'. $block['delta'];
        $blocksarray[$key] = $block['info'];
      }
    }
  }
  return $blocksarray;
}

/**
 * Helper function to get all view displays.
 */
function _quicktabs_get_views_displays($view_name) {
  $displays = array();
  if (empty($view_name)) {
    // No view.
    return $displays;
  }

  $views = views_get_all_views();
  $view = $views[$view_name];

  if (empty($view->display)) {
    // This view is broken.
    return $displays;
  }

  foreach ($view->display as $id => $display) {
    $displays[$id] = $id .': '. $display->display_title;
  }
  return $displays;
}

function quicktabs_get_nodes() {
  $nodes = array();
  // TODO: make the allowed node types an admin setting
  $result = db_query('SELECT nid, title FROM {node} WHERE type in ("page", "story")');
  while ($row = db_fetch_object($result)) {
    $key = $row->nid;
    $nodes[$key] = $row->title;
  }
  return $nodes;
}