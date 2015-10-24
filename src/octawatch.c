#include <pebble.h>
#include <stdio.h>

#define KEY_BACKGROUND_COLOR 0

static Window *window;
static Layer *s_layer;
static TextLayer *s_time_layer;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static uint8_t s_hour;
static uint8_t s_min;
static uint8_t s_sec;

static const GPathInfo TWELVE_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{67,16},{76,16},{83,23},{83,24},{76,31},{67,31},{60,24},{60,23}}
};

static GPath *twelve_path = NULL; 

static const GPathInfo ONE_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{84,25},{85,25},{106,46},{106,47},{100,53},{99,53},{78,32},{78,31}}
};

static GPath *one_path = NULL;

static const GPathInfo TWO_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{108,49},{109,49},{130,70},{130,71},{124,77},{123,77},{102,56},{102,55}}
};

static GPath *two_path = NULL;

static const GPathInfo THREE_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{131,72},{132,72},{139,79},{139,88},{132,95},{131,95},{124,88},{124,79}}
};

static GPath *three_path = NULL;

static const GPathInfo FOUR_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{123,90},{124,90},{130,96},{130,97},{109,118},{108,118},{102,112},{102,111}}
};

static GPath *four_path = NULL;

static const GPathInfo FIVE_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{99,114},{100,114},{106,120},{106,121},{85,142},{84,142},{78,136},{78,135}}
};

static GPath *five_path = NULL;

static const GPathInfo SIX_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{67,136},{76,136},{83,143},{83,144},{76,151},{67,151},{60,144},{60,143}}
};

static GPath *six_path = NULL;

static const GPathInfo SEVEN_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{43,114},{44,114},{65,135},{65,136},{59,142},{58,142},{37,121},{37,120}}
};

static GPath *seven_path = NULL;

static const GPathInfo EIGHT_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{19,90},{20,90},{41,111},{41,112},{35,118},{34,118},{13,97},{13,96}}
};

static GPath *eight_path = NULL;

static const GPathInfo NINE_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{11,72},{12,72},{19,79},{19,88},{12,95},{11,95},{4,88},{4,79}}
};

static GPath *nine_path = NULL;

static const GPathInfo TEN_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{34,49},{35,49},{41,55},{41,56},{20,77},{19,77},{13,71},{13,70}}
};

static GPath *ten_path = NULL;

static const GPathInfo ELEVEN_INFO = {
	.num_points = 8,
	.points = (GPoint []) {{58,25},{59,25},{65,31},{65,32},{44,53},{43,53},{37,47},{37,46}}
};

static GPath *eleven_path = NULL;

static void update_time(struct tm *tick_time) {
	s_hour = tick_time->tm_hour;
	s_min = tick_time->tm_min;
	s_sec = tick_time->tm_sec;
	layer_mark_dirty(s_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time(tick_time);

	static char buffer[] = "00";

	//update minutes
	strftime(buffer, sizeof("00"), "%M", tick_time);
	text_layer_set_text(s_time_layer, buffer);
}

static void draw_watchface(Layer *layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorWindsorTan, GColorWhite));
	uint8_t cur_time = s_hour % 12;


	switch (cur_time) {
		case 11:
			gpath_draw_filled(ctx, eleven_path);
		case 10:
			gpath_draw_filled(ctx, ten_path);
		case 9:
			gpath_draw_filled(ctx, nine_path);
		case 8:
			gpath_draw_filled(ctx, eight_path);
		case 7:
			gpath_draw_filled(ctx, seven_path);
		case 6:
			gpath_draw_filled(ctx, six_path);
		case 5:
			gpath_draw_filled(ctx, five_path);
		case 4:
			gpath_draw_filled(ctx, four_path);
		case 3:
			gpath_draw_filled(ctx, three_path);
		case 2:
			gpath_draw_filled(ctx, two_path);
		case 1:
			gpath_draw_filled(ctx, one_path);
		case 0:
			gpath_draw_filled(ctx, twelve_path);
			break;
		default:
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Hour %d", cur_time);
	}

	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_context_set_stroke_width(ctx, 3);
	switch (cur_time) {
		case 0:
			gpath_draw_outline(ctx, twelve_path);
			break;
		case 1:
			gpath_draw_outline(ctx, one_path);
			break;
		case 2:
			gpath_draw_outline(ctx, two_path);
			break;
		case 3:
			gpath_draw_outline(ctx, three_path);
			break;
		case 4:
			gpath_draw_outline(ctx, four_path);
			break;
		case 5:
			gpath_draw_outline(ctx, five_path);
			break;
		case 6:
			gpath_draw_outline(ctx, six_path);
			break;
		case 7:
			gpath_draw_outline(ctx, seven_path);
			break;
		case 8:
			gpath_draw_outline(ctx, eight_path);
			break;
		case 9:
			gpath_draw_outline(ctx, nine_path);
			break;
		case 10:
			gpath_draw_outline(ctx, ten_path);
			break;
		case 11:
			gpath_draw_outline(ctx, eleven_path);
			break;
		default:
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Invalid Hour %d", cur_time);
	}


	//draw seconds
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorRajah, GColorWhite));
	if (s_sec > 0 && s_sec < 15) {
		graphics_fill_rect(ctx, GRect(67,34 + (2*s_sec) - 2,10,2), 0,0);
	} else if (s_sec >= 15 || s_sec == 0) {
		graphics_fill_rect(ctx, GRect(52,62, 40, 2), 0, 0);
	}

	if (s_sec > 15 && s_sec < 30) {
		uint8_t cur_sec = s_sec - 15;
		graphics_fill_rect(ctx, GRect(120 - (2*cur_sec) + 2,79,2,10), 0,0);
	} else if (s_sec >= 30 || s_sec == 0) {
		graphics_fill_rect(ctx, GRect(92,64, 2, 40), 0, 0);
	}

	if (s_sec > 30 && s_sec < 45) {
		uint8_t cur_sec = s_sec - 30;
		graphics_fill_rect(ctx, GRect(67, 132 - (2*cur_sec) + 2, 10, 2), 0, 0);
	} else if (s_sec >= 45 || s_sec == 0) {
		graphics_fill_rect(ctx, GRect(52, 104, 40, 2), 0, 0);
	}

	if (s_sec > 45 && s_sec < 60) {
		uint8_t cur_sec = s_sec - 45;
		graphics_fill_rect(ctx, GRect(22 + (2*cur_sec) - 2, 79, 2, 10), 0, 0);
	} else if (s_sec == 0) {
		graphics_fill_rect(ctx, GRect(50, 64, 2, 40), 0, 0);
	}

	//draw minutes
	graphics_context_set_fill_color(ctx, COLOR_FALLBACK(GColorOrange, GColorWhite));
	if (s_min > 0 && s_min < 15) {
		graphics_fill_rect(ctx, GRect(67,34 + (2*s_min) - 2,10,2), 0,0);
	} else if (s_min >= 15) {
		graphics_fill_rect(ctx, GRect(52,65, 40, 10), 0, 0);
	}

	if (s_min > 15 && s_min < 30) {
		uint8_t cur_min = s_min - 15;
		graphics_fill_rect(ctx, GRect(120 - (2*cur_min) + 2,79,2,10), 0,0);
	} else if (s_min >= 30) {
		graphics_fill_rect(ctx, GRect(82,64, 10, 40), 0, 0);
	}

	if (s_min > 30 && s_min < 45) {
		uint8_t cur_min = s_min - 30;
		graphics_fill_rect(ctx, GRect(67, 132 - (2*cur_min) + 2, 10, 2), 0, 0);
	} else if (s_min >= 45) {
		graphics_fill_rect(ctx, GRect(52, 94, 40, 10), 0, 0);
	}

	if (s_min > 45 && s_min < 60) {
		uint8_t cur_min = s_min - 45;
		graphics_fill_rect(ctx, GRect(22 + (2*cur_min) - 2, 79, 2, 10), 0, 0);
	} else if (s_min == 0) {
		graphics_fill_rect(ctx, GRect(53, 64, 10, 40), 0, 0);
	}
}

static void setup_paths() {
	twelve_path = gpath_create(&TWELVE_INFO);
	one_path = gpath_create(&ONE_INFO);
	two_path = gpath_create(&TWO_INFO);
	three_path = gpath_create(&THREE_INFO);
	four_path = gpath_create(&FOUR_INFO);
	five_path = gpath_create(&FIVE_INFO);
	six_path = gpath_create(&SIX_INFO);
	seven_path = gpath_create(&SEVEN_INFO);
	eight_path = gpath_create(&EIGHT_INFO);
	nine_path = gpath_create(&NINE_INFO);
	ten_path = gpath_create(&TEN_INFO);
	eleven_path = gpath_create(&ELEVEN_INFO);
}

static void set_background_color(int color) {
	GColor background_color = GColorFromHEX(color);
	window_set_background_color(window, background_color);
	text_layer_set_text_color(s_time_layer, gcolor_legible_over(background_color));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox received handler");
	Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);

	if (background_color_t) {
		int background_color = background_color_t->value->int32;

		persist_write_int(KEY_BACKGROUND_COLOR, background_color);

		set_background_color(background_color);

		APP_LOG(APP_LOG_LEVEL_DEBUG, "background color: %d", background_color);
	}
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	s_layer = layer_create(layer_get_bounds(window_get_root_layer(window)));
	layer_add_child(window_get_root_layer(window), s_layer);
	layer_set_update_proc(s_layer, draw_watchface);

	window_set_background_color(window, GColorDarkGray);

	//setup the background image
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_OCTAWATCH);
	s_background_layer = bitmap_layer_create(GRect(0,0,144,168));
	bitmap_layer_set_background_color(s_background_layer, GColorClear);
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	bitmap_layer_set_compositing_mode(s_background_layer, GCompOpSet);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

	setup_paths();

	//text layer for displaying minutes in center
	s_time_layer = text_layer_create(GRect(63, 75, 19, 19));
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer, GColorWhite);
	text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

	if (persist_read_int(KEY_BACKGROUND_COLOR)) {
		set_background_color(persist_read_int(KEY_BACKGROUND_COLOR));
	}
}

static void window_unload(Window *window) {
	gbitmap_destroy(s_background_bitmap);
	bitmap_layer_destroy(s_background_layer);

	text_layer_destroy(s_time_layer);
}

static void init(void) {
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
			.load = window_load,
			.unload = window_unload,
			});
	const bool animated = true;
	window_stack_push(window, animated);

	//Register with TickTimerService
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	//display the time right away
	time_t start_time = time(NULL);
	update_time(localtime(&start_time));
}

static void deinit(void) {
	window_destroy(window);
	tick_timer_service_unsubscribe();
}

int main(void) {
	init();

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

	app_event_loop();
	deinit();
}
