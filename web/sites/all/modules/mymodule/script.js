// Using the closure to map jQuery to $. 
(function ($) {

// Store our function as a property of Drupal.behaviors.
Drupal.behaviors.myModuleSecureLink = {
  attach: function (context, settings) {
    $('#full-button').click(function () {
        $('#dashboard').html(
          $('#dashboard').html()+
          "Latitude: "+document.getElementById('currentLat').innerHTML+","+
          "Longitude: "+document.getElementById('currentLon').innerHTML+"<br>"
        );
      });
  
  }
};

// You could add additional behaviors here.
Drupal.behaviors.myModuleMagic = {
  attach: function (context, settings) { },
  detach: function (context, settings) { }
};

Drupal.behaviors.startAddress = {
  attach: function (context, settings) {
    
    setInterval(function(){ 
      $.ajax({
              type: "GET",
              url: "http://linux.porchia.it/kerbside/web/my_module/reverse_geocode",
              data: "latitude=" + document.getElementById('startLat').innerHTML + '&longitude=' + document.getElementById('startLon').innerHTML + "&sensor=true&start=1",
              success: function(data){
                 $('#startAddr').html('Starting Address ' + data);
              },
              error: function (jqXHR, textStatus, errorThrown) {
                 $('#startAddr').html('Starting Address: Error');
              }
       
      });
    }, 5000);
  }
};

Drupal.behaviors.currentAddress = {
  attach: function (context, settings) {
    
    setInterval(function(){ 
      $.ajax({
              type: "GET",
              url: "http://linux.porchia.it/kerbside/web/my_module/reverse_geocode",
              data: "latitude=" + document.getElementById('currentLat').innerHTML + '&longitude=' + document.getElementById('currentLon').innerHTML + "&sensor=true",
              success: function(data){
                 $('#currAddr').html('Current Address ' + data);
              },
              error: function (jqXHR, textStatus, errorThrown) {
                 $('#currAddr').html('Current Address: Error');
              }
       
      });
    }, 5000);
  }
};

}(jQuery));