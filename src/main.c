//
// Created by cpasjuste on 03/06/22.
//

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "fbgraphics.h"

#ifdef __RETROROOT__

#include "fbg_fbdev.h"

#else

#include "glfw/fbg_glfw.h"

#endif

#define SCREEN_WIDTH 640

#define min(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

static int keep_running = 1;
static float screen_scale = 1.f;

static void int_handler() {
    keep_running = 0;
}

static int scale(int w) {
    return (int) ((float) w * screen_scale);
}

int main(int argc, char *argv[]) {
    char *logo_path;
    struct _fbg_img *logo_img;
    struct _fbg *fbg;
    int timer = 0;

    signal(SIGINT, int_handler);

#ifdef __RETROROOT__
    if (argc < 3) {
        fprintf(stderr, "usage: rr-splash [device] [png]\n");
        return 0;
    }

    fbg = fbg_fbdevSetup(argv[1], 0);
    logo_path = argv[2];
#else
    fbg = fbg_glfwSetup(640, 480, 4, "rr-splash", 0, 0, 1);
    logo_path = "../data/logo.png";
#endif
    if (!fbg) {
        return 0;
    }

    // set scaling
    screen_scale = (float) fbg->width / (float) SCREEN_WIDTH;

    // load logo
    logo_img = fbg_loadPNG(fbg, logo_path);
    if (!logo_img) {
        fbg_close(fbg);
        return 0;
    }

#ifdef __RETROROOT__
    while (keep_running) {
#else
    while (keep_running && !fbg_glfwShouldClose(fbg)) {
        fbg_glfwClear();
#endif
        fbg_clear(fbg, 0);

        if (fbg->width < logo_img->width || fbg->height < logo_img->height) {
            float img_scale = min(
                    (float) fbg->width / (float) logo_img->width,
                    (float) fbg->height / (float) logo_img->height);
            int img_w = (int) ((float) logo_img->width * img_scale);
            int img_h = (int) ((float) logo_img->height * img_scale);
            fbg_imageScale(fbg, logo_img,
                           (fbg->width - img_w) / 2,
                           (fbg->height - img_h) / 2,
                           img_scale, img_scale);
        } else {
            fbg_image(fbg, logo_img,
                      (int) (fbg->width - logo_img->width) / 2,
                      (int) (fbg->height - logo_img->height) / 2);
        }

        // ~ 1 second updates for "waiting points"
        fbg_rect(fbg, fbg->width - scale(48), fbg->height - scale(16),
                 scale(8), scale(8), 86, 30, 255);
        if (timer > 2) {
            fbg_rect(fbg, fbg->width - scale(32), fbg->height - scale(16),
                     scale(8), scale(8), 86, 30, 255);
        }
        if (timer > 4) {
            fbg_rect(fbg, fbg->width - scale(16), fbg->height - scale(16),
                     scale(8), scale(8), 86, 30, 255);
        }
        if (timer > 6) {
            timer = 0;
        }

        fbg_draw(fbg);
        fbg_flip(fbg);

        if (argc > 3) {
            break;
        }

        timer++;
        usleep(500000);
    }

    fbg_freeImage(logo_img);
    fbg_close(fbg);
}
