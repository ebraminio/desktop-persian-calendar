// g++ source.cc `pkg-config --libs --cflags icu-i18n cairo appindicator3-0.1` && ./a.out
#include <stdio.h>
#include <string.h>

#include <assert.h>

#include <unicode/smpdtfmt.h>
#include <unicode/calendar.h>
#include <unicode/uversion.h>

#include <cairo/cairo.h>

#define TRAY_APPINDICATOR 1

#include "tray.h"

using namespace icu;

static void quit_cb(struct tray_menu *item) {
    tray_exit();
}

static struct tray tray_config = {
    .menu =
        (struct tray_menu[]){
            {.text = (char *) "", .disabled = 1},
            {.text = (char *) "-"},
            {.text = (char *) "خروج", .cb = quit_cb},
            NULL
        }
};

int main(int argc, char **argv) {
    std::string day_of_month;
    std::string date;
    {
        UErrorCode error_code = U_ZERO_ERROR;

        Locale locale("fa_IR@calendar=persian");

        Calendar *calendar = Calendar::createInstance(locale, error_code);
        assert(!error_code);

        {
            UnicodeString pattern("d");
            SimpleDateFormat df(pattern, locale, error_code);

            UnicodeString str;
            df.format(calendar->getTime(error_code), str);
            str.toUTF8String(day_of_month);
        }

        {
            UnicodeString pattern("EEEE d LLLL y");
            SimpleDateFormat df(pattern, locale, error_code);

            UnicodeString str;
            df.format(calendar->getTime(error_code), str);

            str.toUTF8String(date);
        }
    }
    tray_config.menu[0].text = (char *) date.c_str();

    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 128, 128);
    {
        cairo_t *cr = cairo_create (surface);

        cairo_select_font_face(cr, "Noto Naskh Arabic", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 128);
        double x, y;
        cairo_text_extents_t extents;
        cairo_text_extents (cr, day_of_month.c_str(), &extents);
        x = 64 -(extents.width/2 + extents.x_bearing);
        y = 64 -(extents.height/2 + extents.y_bearing);

        cairo_move_to(cr, x, y);
        cairo_set_source_rgba(cr, 1, 1, 1, 1);
        cairo_show_text(cr, day_of_month.c_str());
    }

    char file_name[] = "/tmp/todayicon.png";
    cairo_surface_write_to_png(surface, file_name);
    tray_config.icon = file_name;

    tray_init(&tray_config);
    while (tray_loop(1) == 0);
    tray_exit();

    printf("This is project %s.\n", "a");
    return 0;
}
