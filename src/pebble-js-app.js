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
  var options = JSON.parse(decodeURIComponent(e.response));
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
});