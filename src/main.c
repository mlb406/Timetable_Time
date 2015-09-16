#include <pebble.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define KEY_SUN 0
#define KEY_MON 1
#define KEY_TUE 2
#define KEY_WED 3
#define KEY_THU 4
#define KEY_FRI 5
#define KEY_SAT 6
//#define KEY_SHAKE_TOMORROW 7
  
static Window *window;
static TextLayer *text_layer, *date_layer, *timetable_layer, *battery_layer;
static BitmapLayer *background_layer;
static GBitmap *background_bitmap;
static int battery_level;

//static bool shake_tomorrow = true;
static char sun[64], mon[64], tue[64], wed[64], thu[64], fri[64], sat[64];

static void battery_callback(BatteryChargeState state) {
  battery_level = state.charge_percent;
  static char battery_text[16];
  if (state.is_charging == true) {
    snprintf(battery_text, sizeof(battery_text), "%d%%+", state.charge_percent);
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%%", state.charge_percent);
  }
  
  text_layer_set_text(battery_layer, battery_text);
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%d/%m/%y", tick_time);
  text_layer_set_text(date_layer, date_buffer);
  
  static char buffer[] = "00:00:00";
  
  if(clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    strftime(buffer, sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }
  
  text_layer_set_text(text_layer, buffer);
  
  static char day[] = "1";
  
  strftime(day, sizeof("1"), "%w", tick_time);
  
  static char sun_buffer[64], mon_buffer[64], tue_buffer[64], wed_buffer[64], thu_buffer[64], fri_buffer[64], sat_buffer[64];
  
  
  if (persist_read_string(KEY_SUN, sun_buffer, sizeof(sun_buffer))) {
    int d = atoi(day);
    switch(d) {
      case 0 :
        persist_read_string(KEY_SUN, sun_buffer, sizeof(sun_buffer));
        text_layer_set_text(timetable_layer, sun_buffer);
        break;
      case 1 :
        persist_read_string(KEY_MON, mon_buffer, sizeof(mon_buffer));
        text_layer_set_text(timetable_layer, mon_buffer);
        break;
      case 2 :
        persist_read_string(KEY_TUE, tue_buffer, sizeof(tue_buffer));
        text_layer_set_text(timetable_layer, tue_buffer);
        break;
      case 3 :
        persist_read_string(KEY_WED, wed_buffer, sizeof(wed_buffer));
        text_layer_set_text(timetable_layer, wed_buffer);
        break;
      case 4 :
        persist_read_string(KEY_THU, thu_buffer, sizeof(thu_buffer));
        text_layer_set_text(timetable_layer, thu_buffer);
        break;
      case 5 :
        persist_read_string(KEY_FRI, fri_buffer, sizeof(fri_buffer));
        text_layer_set_text(timetable_layer, fri_buffer);
        break;
      case 6 :
        persist_read_string(KEY_SAT, sat_buffer, sizeof(sat_buffer));
        text_layer_set_text(timetable_layer, sat_buffer);
        break;
      default :
        text_layer_set_text(timetable_layer, "Something went wrong,\nplease try again later");
    }
  } else {
    text_layer_set_text(timetable_layer, "Please add a timetable\n\n:-(");
  }
  
  
  
}

/*static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char day_buffer[] = "1";
  
  strftime(day_buffer, sizeof("1"), "%w", tick_time); 
  
  static char sun_buffer[64], mon_buffer[64], tue_buffer[64], wed_buffer[64], thu_buffer[64], fri_buffer[64], sat_buffer[64];
  
  if (persist_read_string(KEY_SUN, sun_buffer, sizeof(sun_buffer))) {
    int d = atoi(day_buffer);
    switch(d) {
      case 6 :
        persist_read_string(KEY_SUN, sun_buffer, sizeof(sun_buffer));
        text_layer_set_text(timetable_layer, sun_buffer);
        break;
      case 0 :
        persist_read_string(KEY_MON, mon_buffer, sizeof(mon_buffer));
        text_layer_set_text(timetable_layer, mon_buffer);
        break;
      case 1 :
        persist_read_string(KEY_TUE, tue_buffer, sizeof(tue_buffer));
        text_layer_set_text(timetable_layer, tue_buffer);
        break;
      case 2 :
        persist_read_string(KEY_WED, wed_buffer, sizeof(wed_buffer));
        text_layer_set_text(timetable_layer, wed_buffer);
        break;
      case 3 :
        persist_read_string(KEY_THU, thu_buffer, sizeof(thu_buffer));
        text_layer_set_text(timetable_layer, thu_buffer);
        break;
      case 4 :
        persist_read_string(KEY_FRI, fri_buffer, sizeof(fri_buffer));
        text_layer_set_text(timetable_layer, fri_buffer);
        break;
      case 5 :
        persist_read_string(KEY_SAT, sat_buffer, sizeof(sat_buffer));
        text_layer_set_text(timetable_layer, sat_buffer);
        break;
      default :
        text_layer_set_text(timetable_layer, "Something went wrong,\nplease try again later");
    }
    tick_timer_service_unsubscribe();
    Delay(3000);
  }
}
*/


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(background_layer, background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(background_layer));
  
  date_layer = text_layer_create(GRect(0, 90, 144, 30));
  text_layer_set_text(date_layer, "00/00/00");
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorClear);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  text_layer = text_layer_create(GRect(0, 123, 144, 46));
  text_layer_set_text(text_layer,  "00:00:00");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  
  timetable_layer = text_layer_create(GRect(0, 0, 144, 70));
  text_layer_set_text(timetable_layer, "Da1\nLe01Le02Le03Le04Le05\nDa2\nLe01Le02Le03Le04Le05");
  text_layer_set_background_color(timetable_layer, GColorClear);
  text_layer_set_text_color(timetable_layer, GColorWhite);
  text_layer_set_font(timetable_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(timetable_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(timetable_layer));
  
  battery_layer = text_layer_create(GRect(0, 75, 144, 20));
  text_layer_set_text(battery_layer, "---");
  text_layer_set_background_color(battery_layer, GColorClear);
  text_layer_set_text_color(battery_layer, GColorWhite);
  text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(battery_layer));
  
  battery_callback(battery_state_service_peek());
}

static void main_window_unload(Window *window) {
  text_layer_destroy(text_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(timetable_layer);
  text_layer_destroy(battery_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Inbox recieved!");
  Tuple *t = NULL;
  Tuple *sunday_t = dict_find(iterator, KEY_SUN);
  Tuple *monday_t = dict_find(iterator, KEY_MON);
  Tuple *tuesday_t = dict_find(iterator, KEY_TUE);
  Tuple *wednesday_t = dict_find(iterator, KEY_WED);
  Tuple *thursday_t = dict_find(iterator, KEY_THU);
  Tuple *friday_t = dict_find(iterator, KEY_FRI);
  Tuple *saturday_t = dict_find(iterator, KEY_SAT);
  
  t = dict_read_first(iterator);
  while(t != NULL)
  {
    switch(t->key)
    {
      case KEY_SUN:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_SUN");
        snprintf(sun, sizeof(sun), sunday_t->value->cstring, sunday_t->value->cstring);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Written %s", sun);
        persist_write_string(KEY_SUN, sun);
        break;
      case KEY_MON:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_MON");  
        snprintf(mon, sizeof(mon), monday_t->value->cstring, monday_t->value->cstring);
        persist_write_string(KEY_MON, mon);
        break;
      case KEY_TUE:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_TUE");
        snprintf(tue, sizeof(tue), tuesday_t->value->cstring, tuesday_t->value->cstring);
        persist_write_string(KEY_TUE, tue);
        break;      
      case KEY_WED:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_WED");
        snprintf(wed, sizeof(wed), wednesday_t->value->cstring, wednesday_t->value->cstring);
        persist_write_string(KEY_WED, wed);
        break;  
      case KEY_THU:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_THU");
        snprintf(thu, sizeof(thu), thursday_t->value->cstring, thursday_t->value->cstring);
        persist_write_string(KEY_THU, thu);
        break;
      case KEY_FRI:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_FRI");  
        snprintf(fri, sizeof(fri), friday_t->value->cstring, friday_t->value->cstring);
        persist_write_string(KEY_FRI, fri);
        break;
      case KEY_SAT:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "got KEY_SAT");
        snprintf(sat, sizeof(sat), saturday_t->value->cstring, saturday_t->value->cstring);
        persist_write_string(KEY_SAT, sat);
      break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown key! :-(");
        break;
    }
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void init() {
  window = window_create();
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  battery_state_service_subscribe(battery_callback);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  

  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  
  window_stack_push(window, true);
  
  update_time();
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Initialised window!");
}

static void deinit() {
  window_destroy(window);
}

int main(void) {
  init();
  
  app_event_loop();
  
  deinit();
  
  return 0;
}