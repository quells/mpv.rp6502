#include <rp6502.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define CANVAS_DATA 0x0000
#define PALETTE 0xFD00 // 256 u16
#define CANVAS_STRUCT 0xFF00

#define CANVAS_WIDTH 160
#define CANVAS_HEIGHT 90

static void erase_canvas(void) {
    unsigned i;
    RIA.addr0 = CANVAS_DATA;
    RIA.step0 = 1;
    for (i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT / 16; i++) {
        // unrolled for speed
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
        RIA.rw0 = 0;
    }
}

static void init_palette(void) {
    uint16_t p = 0;
    unsigned r = 0, g, b;
    RIA.addr0 = PALETTE;
    RIA.step0 = 1;
    for (; r < 4; r++) {
        for (g = 0; g < 16; g++) {
            for (b = 0; b < 4; b++) {
                p = (b << 14) | (g << 7) | (r << 3);
                RIA.rw0 = p & 0x00FF;
                RIA.rw0 = (p & 0xFF00) >> 8;
            }
        }
    }
}

void main() {
    int f = open("bbbh.mov", 0x01);
    if (f < 0) {
        perror("open image");
        return;
    }

    xreg_vga_canvas(2); // 320x180
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, x_wrap, false);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, y_wrap, false);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, x_pos_px, 80);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, y_pos_px, 45);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, width_px, CANVAS_WIDTH);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, height_px, CANVAS_HEIGHT);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, xram_data_ptr, CANVAS_DATA);
    xram0_struct_set(CANVAS_STRUCT, vga_mode3_config_t, xram_palette_ptr, PALETTE);

    erase_canvas();
    init_palette();

    xreg_vga_mode(3, 3, CANVAS_STRUCT, 0);

    for (;;) {
        read_xram(CANVAS_DATA, CANVAS_WIDTH*CANVAS_HEIGHT, f);
    }
}
