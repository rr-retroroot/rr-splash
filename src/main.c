//
// Created by cpasjuste on 03/06/22.
//

#include <stdio.h>
#include <signal.h>
#include "fbgraphics.h"

#ifdef __RETROROOT__

#include "fbg_fbdev.h"

#else

#include "glfw/fbg_glfw.h"

#endif

int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, int_handler);

    if (argc != 2) {
        fprintf(stderr, "rr-splash: no png image provided...\n");
        return 0;
    }

#ifdef __RETROROOT__
    struct _fbg *fbg = fbg_fbdevInit();
#else
    struct _fbg *fbg = fbg_glfwSetup(640, 480, 4, "rr-splash", 0, 0, 1);
#endif
    if (!fbg) {
        return 0;
    }

    struct _fbg_img *texture = fbg_loadPNG(fbg, argv[1]);
    if (!texture) {
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

        fbg_image(fbg, texture,
                  (int) (fbg->width - texture->width) / 2,
                  (int) (fbg->height - texture->height) / 2);

        fbg_draw(fbg);
        fbg_flip(fbg);
    }

    fbg_freeImage(texture);
    fbg_close(fbg);
}
