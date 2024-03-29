<?php

/**
 * @file
 * views/ip_geoloc_plugin_argument_default_ip_geoloc.in
 */

/**
 * Default argument plugin to extract a user via menu_get_object
 */
class ip_geoloc_plugin_argument_default_ip_geoloc extends views_plugin_argument_default {

  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'postal');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Location attribute to use'),
      '#options' => array(
        'postal_code' => t('Postal code'),
        'city_state' => t('City and State/Province'),
        'country' => t('Country'),
        'lat_long' => t('Latitude, longitude'),
        'formatted_address' => t('Formatted address'),
      ),
      '#default_value' => $this->options['type']
    );
  }

  function get_argument() {
    if (!empty($_SESSION['ip_geoloc']['location'])) {
      $location = $_SESSION['ip_geoloc']['location'];
      switch ($this->options['type']) {
        case 'postal_code':
          return $location['postal_code'];
        case 'city_state':
          return $location['city'] . ', ' . $location['region'];
        case 'country':
          return $location['country'];
        case 'lat_long':
          return $location['latitude'] . ',' . $location['longitude'];
        case 'formatted_address':
          return $location['formatted_address'];
      }
    }
    return NULL;
  }

}
