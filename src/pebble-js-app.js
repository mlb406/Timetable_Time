Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKitJS ready!');
    
    
  }
);

//Begin config code
Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL("http://mlb406.github.io/Timetable_Time");
  console.log("Opened settings!");
});
    
Pebble.addEventListener('webviewclosed', function(e) {
  //Get JSON dictionary
  var configuration = JSON.parse(decodeURIComponent(e.response));
  console.log("Configuration window returned: " + JSON.stringify(configuration));

  // Create a dict to send to pebble
  // new one to avoid possibilty of sending larger dict from web page
  // also allows sanity checks/conversions if needed
  var config_for_pebble = {
    "KEY_SUN": configuration.sun,
    "KEY_MON": configuration.mon,
    "KEY_TUE": configuration.tue,
    "KEY_WED": configuration.wed,
    "KEY_THU": configuration.thu,
    "KEY_FRI": configuration.fri,
    "KEY_SAT": configuration.sat
  };
  console.log("Configuration for pebble: " + JSON.stringify(config_for_pebble));

  //Send to Pebble, persist there     
  Pebble.sendAppMessage(
    config_for_pebble,
    function(e) {
      console.log("Sending settings data...");
    },
    function(e) {
      console.log("Settings feedback failed!");
    }
  );
  /*var options = JSON.parse(decodeURIComponent(e.response));
  console.log(JSON.stringify(options));
  console.log("Recieved settings!");
  var sun = options.sun;
  var mon = options.mon;
  var tue = options.tue;
  var wed = options.wed;
  var thu = options.thu;
  var fri = options.fri;
  var sat = options.sat;
  
  var dict = {
    "KEY_SUN": sun,
    "KEY_MON": mon,
    "KEY_TUE": tue,
    "KEY_WED": wed,
    "KEY_THU": thu,
    "KEY_FRI": fri,
    "KEY_SAT": sat
  };
  
  Pebble.sendAppMessage(dict, function(e) {
    console.log("Sent config settings!");
  }, function(e) {
    console.log("Failed to send options!");
  });
  */
});