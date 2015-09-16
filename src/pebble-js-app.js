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
    0: configuration.sun,
    1: configuration.mon,
    2: configuration.tue,
    3: configuration.wed,
    4: configuration.thu,
    5: configuration.fri,
    6: configuration.sat
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
  
});