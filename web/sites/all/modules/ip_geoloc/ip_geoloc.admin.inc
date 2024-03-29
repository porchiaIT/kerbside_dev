<?php

/**
 * @file
 * Admin configuration settings for IP Geolocation.
 */

/**
 * Menu callback for admin settings.
 */
function ip_geoloc_admin_configure() {

  if (ip_geoloc_diagnose() > 0) {
    // Form for sync-ing the geolocation table with the system accesslog
    $form['ip_geoloc_db_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update IP geolocation database using past visitor IP addresses from the system access log'),
      '#description'  => t('You can update the IP geolocation database in batches by pressing the button below. As a rough guide, count on a 1 minute wait for every 100 IP addresses, when executing a web service like IPInfoDB, as employed by Smart IP. Add another 2 minutes per 100 IP addresses if you ticked the option to employ the Google Maps API to reverse-geocode to street addresses. If your server interrupts the process you can continue from where it stopped by refreshing this page and pressing the button again. You will not lose any data.')
    );
    $form['ip_geoloc_db_options']['ip_geoloc_sync_with_accesslog'] = array(
      '#type' => 'submit',
      '#value' => t('Update now'),
      '#submit' => array('ip_geoloc_sync_with_accesslog'),
    );
    $form['ip_geoloc_db_options']['ip_geoloc_sync_batch_size'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Batch size'),
      '#default_value'  => variable_get('ip_geoloc_sync_batch_size', 500),
      '#description' => t('To change the default batch size, press "Save configuration".')
    );
  }

  $form['ip_geoloc_data_collection_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data collection options'),
  );
  $form['ip_geoloc_data_collection_options']['ip_geoloc_google_to_reverse_geocode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Employ the Google Maps API to reverse-geocode HTML5 visitor locations to street addresses.'),
    '#default_value' => variable_get('ip_geoloc_google_to_reverse_geocode', TRUE),
    '#description' => t("For present and future visitors this is done via the Javascript version of the Maps API and the HTML5 way of obtaining a visitor's location. This involves them being prompted to accept sharing of their location. For the upload of historic visitor location data the server-side version of the Google Maps API is used. The latter is subject to a Google-imposed daily limit on the number of calls coming from the same server. <br/>If you are using IP Geolocation only for the Views format <strong>Map (Google, via IP Geolocation)</strong>, then you can safely untick this box.")
  );
  $form['ip_geoloc_data_collection_options']['ip_geoloc_roles_to_reverse_geocode'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles for which HTML5 visitor locations are to be requested and reverse-geocoded to street addresses'),
    '#default_value' => variable_get('ip_geoloc_roles_to_reverse_geocode', array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID)),
    '#options' => user_roles(),
    '#description' => t('Selected roles are effective only when the check box on the data collection option above is also ticked.'),
  );

  $form['ip_geoloc_data_collection_options']['ip_geoloc_smart_ip_as_backup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Employ Smart IP as a backup to the Google Maps Javascript API as well as declined or failed HTML5 location retrievals in Views.'),
    '#default_value' => variable_get('ip_geoloc_smart_ip_as_backup', TRUE),
    '#description' => t('This refers to situations where the lat/long coords could not be established (e.g. because the browser/device is not supported or the user declined to share their location) or the Google Maps API reverse-geocode function failed or was not employed through the tick box above. Smart IP lookups tend to be less detailed than the Google Maps reverse-geocoded results.<br/>If this box is <strong>not</strong> ticked, but the <a href="@geoip">GeoIP API module</a> is enabled, then GeoIP will be used as the Google Maps API fallback and to load historic lat/long coordinates.', array(
      '@geoip' => url('http://drupal.org/project/geoip')
    ))
  );
  $form['ip_geoloc_data_collection_options']['ip_geoloc_location_check_interval'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Mininum elapsed time in seconds before geolocation data for the same user will be collected again.'),
    '#default_value' => variable_get('ip_geoloc_location_check_interval', IP_GEOLOC_LOCATION_CHECK_INTERVAL),
    '#description' => t('Geolocation information associated with an IP address may change over time, for instance when the visitor is using a mobile device and is moving. Use zero to stop repeat location collection.')
  );

  $form['ip_geoloc_advanced'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Advanced options'),
  );
  $form['ip_geoloc_advanced']['ip_geoloc_page_refresh'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-refresh the page as soon as an HTML5 location update has come in.'),
    '#default_value' => variable_get('ip_geoloc_page_refresh', FALSE),
    '#description' => t('The above tick box does not apply to administration pages.')
  );

  $form['ip_geoloc_advanced']['markers'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Alternative markers'),
    '#description' => t('')
  );
  $form['ip_geoloc_advanced']['markers']['ip_geoloc_marker_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to marker images (Leaflet, Google Maps)'),
    '#default_value' => variable_get('ip_geoloc_marker_directory', drupal_get_path('module', 'ip_geoloc') . '/markers'),
    '#description' => t('Should normally NOT start with a slash. All marker images must be .png files.')
  );
  if (module_exists('openlayers')) {
    $form['ip_geoloc_advanced']['markers']['ip_geoloc_num_location_marker_layers'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of OpenLayers location marker layers you may need.'),
      '#default_value' => variable_get('ip_geoloc_num_location_marker_layers', IP_GEOLOC_DEF_NUM_MARKER_LAYERS),
      '#description' => t('Only relevant when you have selected "differentiator" fields in your views.')
    );
  }
  $form['ip_geoloc_advanced']['ip_geoloc_debug'] = array(
    '#type' => 'textfield',
    '#title' => t('Detail execution progress with status messages.'),
    '#default_value' => variable_get('ip_geoloc_debug'),
    '#description' => t('Enter a comma-separated list of names of users that should see status messages coming from this module, e.g. for debugging purposes.')
  );
  $form['ip_geoloc_advanced']['ip_geoloc_erase_session'] = array(
    '#type' => 'submit',
    '#value' => t('Erase geolocation data from session now'),
    '#submit' => array('ip_geoloc_erase_session'),
  );
  $form['ip_geoloc_advanced']['ip_geoloc_erase_db'] = array(
    '#type' => 'submit',
    '#value' => t('Erase entire IP geolocation database now'),
    '#submit' => array('ip_geoloc_erase_db'),
  );

  return system_settings_form($form);
}

/**
 * Report on the configuration status.
 *
 * Reports in particular to the system access log, which is required for
 * visitor views and maps.
 *
 * @return
 *   -1, if there's a problem, otherwise a count of IP addresses not stored
 */
function ip_geoloc_diagnose() {
  $geoloc_count = db_query('SELECT COUNT(DISTINCT ip_address) FROM {ip_geoloc}')->fetchField();
  drupal_set_message(t("The IP geolocation database currently contains information for %geoloc_count visited IP addresses.", array('%geoloc_count' => $geoloc_count)), 'status', FALSE);

  if (!db_table_exists('accesslog')) {
    drupal_set_message(t('The <strong>accesslog</strong> database table does not exist, probably because core\'s <strong>Statistics</strong> module is not enabled. Views and maps of visitors will not be available until you enable the <strong>Statistics</strong> module and its <strong>access log</strong>. The block "Map centered on current visitor\'s location" is not affected and should still display.'), 'warning');
  }
  elseif (!module_exists('statistics')) {
    drupal_set_message(t('The <strong>Statistics</strong> module is not enabled. Views and maps of visitors will not be available or display errors until you enable the <strong>Statistics</strong> module and its <strong>access log</strong>. The block "Map centered on current visitor\'s location" is not affected and should still display.'), 'warning');
  }
  else {
    $ip_address_count = db_query('SELECT COUNT(DISTINCT hostname) FROM {accesslog}')->fetchField();
    drupal_set_message(t("The system access log currently contains entries from %ip_address_count IP addresses.", array('%ip_address_count' => $ip_address_count)), 'status', FALSE);
    if (!variable_get('statistics_enable_access_log', FALSE)) {
      drupal_set_message(t('The <strong>Statistics</strong> module is enabled, but its system <strong>access log</strong> is not. Therefore all visitor Views are frozen in time and will not grow. The block "Map centered on current visitor\'s location" is not affected and should still display. You can enable the <strong>access log</strong> at <a href="/admin/config/system/statistics">Configuration >> Statistics</a>.'), 'warning');
    }
    else {
      $non_synched_ips = ip_geoloc_ips_to_be_synched();
      $count = count($non_synched_ips);
      if ($count > 0) {
        drupal_set_message(t("%count IP addresses in the system access log currently have no associated lat/long or address information on the IP geolocation database. These are the most recent ones: %ips",
          array('%count' => $count, '%ips' => implode(', ', array_slice($non_synched_ips, 0, 10, TRUE)))), 'status', FALSE);
      }
      else {
        drupal_set_message(t("The IP geolocation database is up to date and in sync with the system access log."), 'status', FALSE);
      }
      return $count;
    }
  }
  return -1;
}

/**
 * Bring the visitor location database up to date with the system accesslog.
 *
 * Go through all the IP addresses in the {accesslog} table (Statistics module).
 * For each IP address not yet recorded in the {ip_geoloc} table, retrieve its
 * geolocation data and store in {ip_geoloc}.
 * This is a one-off process. Once synchronised the {ip_geoloc} table will lock
 * step with the {accesslog} table.
 * Returns the number of {accesslog} records processed or FALSE if no IP
 * geolocation retrieval function was found.
 *
 * Note: modules supported for the backfill currently are:
 *   Smart IP module
 *   GeoIP API module
 *   custom modules that implement hook_get_ip_geolocation_alter().
 */
function ip_geoloc_sync_with_accesslog() {
  drupal_get_messages(); // wipe previous message to avoid confusion

  $use_smart_ip = variable_get('ip_geoloc_smart_ip_as_backup', TRUE) && module_exists('smart_ip');
  $use_google_to_reverse_geocode = variable_get('ip_geoloc_google_to_reverse_geocode', TRUE);

  $batch_size = check_plain($_POST['ip_geoloc_sync_batch_size']); // from Configuration >> IP Geolocation form
  if (empty($batch_size)) {
    $batch_size = variable_get('ip_geoloc_sync_batch_size', 500);
  }
  $ips_to_be_processed = ip_geoloc_ips_to_be_synched();
  if (count($ips_to_be_processed) > $batch_size) {
    $ips_to_be_processed = array_slice($ips_to_be_processed, 0, $batch_size, TRUE);
  }
  $count = count($ips_to_be_processed);

  $batch = array(
    'file' => drupal_get_path('module', 'ip_geoloc') . '/ip_geoloc.admin.inc',
    'operations' => array(array('_ip_geoloc_process_access_log', array($ips_to_be_processed, $use_smart_ip, $use_google_to_reverse_geocode))),
    'title' => t('Processing next %count IP addresses from access log', array('%count' => $count)),
    'progress_message' => t('Time elapsed: @elapsed. Time remaining: @estimate.'),
    'error_message' => t('An error occurred while processing the access log.'),
    'finished' => '_ip_geoloc_process_access_log_finished'
  );
  batch_set($batch);
  return $count;
}

/*
 *  Return an array of all IP addresses in the current access log that have not
 *  yet had their geolocation data added to the ip_geoloc table.
 */
function ip_geoloc_ips_to_be_synched() {
  if (!db_table_exists('accesslog')) {
    return array();
  }
  $hostnames = db_query('SELECT DISTINCT hostname FROM {accesslog} ORDER BY aid DESC')->fetchCol('hostname');
  $ip_geolocations = db_query('SELECT DISTINCT ip_address FROM {ip_geoloc}')->fetchCol('ip_address');
  return array_diff($hostnames, $ip_geolocations);
}

function ip_geoloc_erase_session() {
  drupal_get_messages();
  unset($_SESSION['ip_geoloc']);
  drupal_set_message(t('Geolocation data erased from session.'));
}

function ip_geoloc_erase_db() {
  drupal_get_messages(); // wipe previous messages to avoid confusion
  $result = db_delete('ip_geoloc')->execute();
}

function _ip_geoloc_process_access_log($ips_to_be_processed, $use_smart_ip, $use_google_to_reverse_geocode, &$context) {
  $sandbox = &$context['sandbox'];
  if (!isset($sandbox['progress'])) {
    $sandbox['progress'] = 0;
    $sandbox['max'] = count($ips_to_be_processed);
    $sandbox['ips'] = $ips_to_be_processed;
  }
  $ip_address = array_shift($sandbox['ips']);
  $location = array('ip_address' => $ip_address);

  if ($use_smart_ip) {
    // Depending on the value of the variable 'smart_ip_use_ipinfodb_service'
    // this will either use the IPInfoDB web service or Smart IP's database, as
    // created by importing the MaxMind CSV archive.
    $location = smart_ip_get_location($location['ip_address']);
  }
  else {
    ip_geoloc_use_geoip_api_if_enabled($location);
  }
  // Now that we have lat/long we can reverse-geocode to the street address.
  // Note, this call is subject to a limit of 2500/day.
  if ($use_google_to_reverse_geocode && $google_address = ip_geoloc_reverse_geocode($location['latitude'], $location['longitude'])) {
    // To avoid fields contradicting eachother, should we clear out, rather
    // than merge with whatever Smart IP or GeoIP put in the $location?
    // For example Google normally returns 'locality', whereas and Smart IP and
    // GeoIP return 'city' instead. Similarly 'administrative_area_level_1' vs
    // 'region'.
    // $location = array('ip_address' => $ip_address);
    ip_geoloc_flatten_google_address($google_address, $location);
  }

  if (empty($location['formatted_address'])) {
    // Just so that a record is created and the IP is taken off the list
    $location['formatted_address'] = '-';
  }
  if (ip_geoloc_store_location($location) === FALSE) {
    return;
  }

  // Update our progress information.
  $sandbox['progress']++;
  // Store result for post-processing in the _finished callback.
  $context['results'][] = $location['ip_address'] . ': ' . (empty($location['formatted_address']) ? '?' : $location['formatted_address']);
  // Provide to the batch engine an estimate of the level of completion so far.
  if ($sandbox['progress'] < $sandbox['max']) {
    // Note the addition of 100 in the formula below. This is to make sure that
    // batch sizes of 200 or greater do not terminate prematurely.
    // E.g 199/200 = 0.995 ends up being rounded to 100% causing abort.
    $context['finished'] = floor(100 * $sandbox['progress'] / $sandbox['max']) / 100;
  }
}

function _ip_geoloc_process_access_log_finished($success, $results, $operations, $elapsed) {
  if ($success) {
    drupal_set_message(t("%count new IP geolocation records compiled and stored in %elapsed-time.",
      array('%count' => count($results), '%elapsed-time' => $elapsed)));
  }
  else {
    drupal_set_message(t('An error occurred. Processing of the access log did not complete.'), 'error');
    $message = format_plural(count($results), 'One IP address successfully processed:', '@count IP addresses successfully processed:');
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
}
