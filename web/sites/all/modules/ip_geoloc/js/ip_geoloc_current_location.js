
(function ($) {

  Drupal.behaviors.addCurrentLocation = {
    attach: function (context, settings) {

      var callback_url = Drupal.settings.basePath + settings.ip_geoloc_menu_callback;
      var refresh_page = settings.ip_geoloc_refresh_page;

      /* Use the geo.js unified API. This covers the W3C Geolocation API as well
       * as some specific devices like Palm and Blackberry.
       */
      if (geo_position_js.init()) {
        geo_position_js.getCurrentPosition(getLocation, handleLocationError, {enableHighAccuracy: true, timeout: 20000});
      }
      else {
        var data = new Object;
        data['error'] = Drupal.t('Cannot accurately determine visitor location. Browser does not support getCurrentPosition(): @browser', { '@browser': navigator.userAgent });
        callback_php(callback_url, data, false);
      }

      function getLocation(position) {
        //alert(Drupal.t('Received (@lat, @lng)', { '@lat': position.coords.latitude, '@lng': position.coords.longitude }));

        var location = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);

        new google.maps.Geocoder().geocode({'latLng': location }, function(response, status) {

          var ip_geoloc_address = new Object;
          ip_geoloc_address['latitude']  = position.coords.latitude;
          ip_geoloc_address['longitude'] = position.coords.longitude;
          ip_geoloc_address['accuracy']  = position.coords.accuracy;

          if (status == google.maps.GeocoderStatus.OK) {
            var google_address = response[0];
            ip_geoloc_address['formatted_address'] = google_address.formatted_address;
            for (var i = 0; i < google_address.address_components.length; i++) {
              var component = google_address.address_components[i];
              if (component.long_name != null) {
                var type = component.types[0];
                ip_geoloc_address[type] = component.long_name;
                if (type == 'country' && component.short_name != null) {
                  ip_geoloc_address['country_code'] = component.short_name;
                }
              }
            }
            //alert(Drupal.t('Reverse-geocoded your location to address: @address', { '@address': ip_geoloc_address['formatted_address'] }));
          }
          else {
            ip_geoloc_address['error'] = Drupal.t('getLocation(): Google address lookup failed with status code !code.', { '!code': status });
            //alert(ip_geoloc_address['error']);
            refresh_page = false;
          }
          // Pass lat/long, accuracy and address back to Drupal
          callback_php(callback_url, ip_geoloc_address, refresh_page);
        });
      }

      function handleLocationError(error) {
        switch (error.code) {
          case 1:
            text = Drupal.t('user declined to share location');
            break;
          case 2:
            text = Drupal.t('position unavailable (connection lost?)');
            break;
          case 3:
            text = Drupal.t('timeout');
            break;
          default:
            text = Drupal.t('unknown error');
        }
        var data = new Object;
        data['error'] = Drupal.t('getCurrentPosition() returned error !code: !text. @browser', {'!code': error.code, '!text': text, '@browser': navigator.userAgent});
        // Pass error back to PHP rather than alert();
        callback_php(callback_url, data, false);
      }

      function callback_php(callback_url, data, refresh_page) {
        $.ajax({
          url: callback_url,
          type: 'POST',
          dataType: 'json',
          data: data,
          success: function () {
          },
          error: function (http) {
            if (http.status > 0 && http.status != 200) {
              alert(Drupal.t('IP Geolocation: an HTTP error @status occurred.', { '@status': http.status }));
            }
          },
          complete: function() {
            if (refresh_page) {
              /* Requires: http://malsup.github.com/jquery.blockUI.js
              $.blockUI({
                message: Drupal.t('Your location was updated...'),
                css: {
                  padding: '10px',
                  color: 'white',
                  backgroundColor: 'black',
                  opacity: .5
                }
              });
              setTimeout($.unblockUI, 2000);
              */
              window.location.reload();
            }
          }
        });
      }

    }
  }
})(jQuery);
