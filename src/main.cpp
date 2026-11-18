#include <Arduino.h>
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include "lvgl_v8_port.h"
#include <stdio.h>

using namespace esp_panel::board;

namespace {
constexpr uint32_t kRed = 0xF20A18;
constexpr uint32_t kDimRed = 0x8F0710;
constexpr uint32_t kCard = 0x080808;
constexpr uint32_t kWhite = 0xF2EDEE;

lv_obj_t *clock_label;
uint32_t clock_started_ms;

lv_obj_t *make_label(const char *text, const lv_font_t *font, lv_color_t color)
{
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, color, 0);
    return label;
}

void make_tile(lv_align_t align, lv_coord_t x, const char *caption, const char *value)
{
    lv_obj_t *tile = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(tile);
    lv_obj_set_size(tile, 64, 54);
    lv_obj_set_style_bg_color(tile, lv_color_hex(kCard), 0);
    lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(tile, lv_color_hex(kDimRed), 0);
    lv_obj_set_style_border_width(tile, 1, 0);
    lv_obj_set_style_radius(tile, 14, 0);
    lv_obj_align(tile, align, x, 42);

    lv_obj_t *head = lv_label_create(tile);
    lv_label_set_text(head, caption);
    lv_obj_set_style_text_font(head, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(head, lv_color_hex(kWhite), 0);
    lv_obj_align(head, LV_ALIGN_TOP_MID, 0, 5);

    lv_obj_t *data = lv_label_create(tile);
    lv_label_set_text(data, value);
    lv_obj_set_style_text_font(data, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(data, lv_color_hex(kRed), 0);
    lv_obj_align(data, LV_ALIGN_BOTTOM_MID, 0, -5);
}

void update_clock(lv_timer_t *timer)
{
    lv_obj_t *label = static_cast<lv_obj_t *>(timer->user_data);
    const uint32_t elapsed_minutes = (millis() - clock_started_ms) / 60000U;
    const uint32_t total_minutes = (12U * 60U + 45U + elapsed_minutes) % (24U * 60U);
    char text[6];
    snprintf(text, sizeof(text), "%02lu:%02lu",
             static_cast<unsigned long>(total_minutes / 60U),
             static_cast<unsigned long>(total_minutes % 60U));
    lv_label_set_text(label, text);
}

void create_nightwatch_ui()
{
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t *title = make_label("NIGHTWATCH", &lv_font_montserrat_14, lv_color_hex(kRed));
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // These are illustrative demo readings; this board has no built-in battery,
    // step counter, or compass sensor.
    make_tile(LV_ALIGN_TOP_LEFT, 12, "BAT DEMO", "83%");
    make_tile(LV_ALIGN_TOP_MID, 0, "STEPS", "1234");
    make_tile(LV_ALIGN_TOP_RIGHT, -12, "VOLT DEMO", "3.87V");

    clock_label = make_label("12:45", &lv_font_montserrat_40, lv_color_hex(kRed));
    lv_obj_align(clock_label, LV_ALIGN_TOP_MID, 0, 108);
    clock_started_ms = millis();
    lv_timer_create(update_clock, 1000, clock_label);

    lv_obj_t *date = make_label("ESP32-S3  /  DEMO", &lv_font_montserrat_12, lv_color_hex(kWhite));
    lv_obj_align(date, LV_ALIGN_TOP_MID, 0, 160);

    lv_obj_t *compass = lv_obj_create(screen);
    lv_obj_remove_style_all(compass);
    lv_obj_set_size(compass, 76, 76);
    lv_obj_set_style_radius(compass, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(compass, lv_color_hex(kCard), 0);
    lv_obj_set_style_bg_opa(compass, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(compass, lv_color_hex(kDimRed), 0);
    lv_obj_set_style_border_width(compass, 2, 0);
    lv_obj_align(compass, LV_ALIGN_CENTER, 0, 34);

    lv_obj_t *north = lv_label_create(compass);
    lv_label_set_text(north, "N");
    lv_obj_set_style_text_font(north, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(north, lv_color_hex(kWhite), 0);
    lv_obj_align(north, LV_ALIGN_TOP_MID, 0, 7);

    lv_obj_t *bearing = lv_label_create(compass);
    lv_label_set_text(bearing, "NW");
    lv_obj_set_style_text_font(bearing, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(bearing, lv_color_hex(kRed), 0);
    lv_obj_align(bearing, LV_ALIGN_CENTER, 0, 2);

    lv_obj_t *compass_note = make_label("COMPASS DEMO", &lv_font_montserrat_10, lv_color_hex(kRed));
    lv_obj_align(compass_note, LV_ALIGN_CENTER, 0, 83);

    lv_obj_t *footer = make_label("N       E       S       W", &lv_font_montserrat_12, lv_color_hex(kRed));
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -25);
    lv_obj_t *board = make_label("VIEWe  •  240 x 320", &lv_font_montserrat_10, lv_color_hex(kWhite));
    lv_obj_align(board, LV_ALIGN_BOTTOM_MID, 0, -7);
}
} // namespace

void setup()
{
    Serial.begin(115200);
    Board *board = new Board();
    board->init();
    assert(board->begin());

    lvgl_port_init(board->getLCD(), board->getTouch());
    lvgl_port_lock(-1);
    create_nightwatch_ui();
    lvgl_port_unlock();
}

void loop()
{
    delay(1000);
}
