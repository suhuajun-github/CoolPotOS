#include "../include/graphics.h"
#include "../include/memory.h"
#include "../include/io.h"
#include "../include/common.h"
#include "../include/multiboot2.h"
#include "../include/timer.h"

uint32_t width, height;
uint32_t c_width, c_height; // 字符绘制总宽高
int32_t x, y;
int32_t cx, cy; // 字符坐标
uint32_t color, back_color;
uint32_t *screen;
uint32_t *char_buffer;
extern uint8_t ascfont[];
extern uint8_t plfont[];

bool vbe_status;

void vbe_scroll() {
    if (cx > c_width) {
        cx = 0;
        cy++;
    } else cx++;

    if (cy >= c_height) {
        cy = c_height - 1;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                screen[j + i * width] = screen[j + (i + 16) * width];
            }
        }
    }
    if (y == height - height % 16) {
        cy = c_height - 1;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                (screen)[j + i * width] = (screen)[j + (i + 16 - height % 16) * width];
            }
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < 28; j++) {
            screen[height * (width + j) + i] = back_color;
        }
    }
}

void vbe_draw_char(char c, int32_t x, int32_t y) {
    if (c == ' ') {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 9; j++) {
                screen[(y + i) * width + x + j] = back_color;
            }
        }
        return;
    }

    unsigned char *font;
    font = plfont;
    font += c * 16;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 9; j++) {
            if (font[i] & (0x80 >> j)) {
                screen[(y + i) * width + x + j] = color;
            } else screen[(y + i) * width + x + j] = back_color;
        }
    }
}

int cur_task() {
    return 0;
    while (1) {
        vbe_draw_char('_', x, y);
        clock_sleep(5);
        vbe_draw_char(' ', x, y);
        clock_sleep(5);
    }
}

void draw_rect(int x0, int y0, int x1, int y1, int c) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            (screen)[y * width + x] = c;
        }
    }
}

void vbe_putchar(char ch) {

    if (ch == '\n') {
        cx = 0;
        cy++;
        return;
    } else if (ch == '\r') {
        cx = 0;
    } else if (ch == '\b' && cx > 0) {
        cx -= 1;
        if (cx == 0) {
            cx = c_width - 1;
            if (cy != 0) cy -= 1;
            if (cy == 0) cx = 0, cy = 0;
        }
        draw_rect(x, y, x + 9, y + 16, 0);
        return;
    }

    vbe_scroll();

    vbe_draw_char(ch, cx * 9 - 7, cy * 16);

}

void vbe_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        vbe_putchar(data[i]);
}

void vbe_writestring(const char *data) {
    vbe_write(data, strlen(data));
}

void vbe_clear() {
    for (uint32_t i = 0; i < (width * (height)); i++) {
        screen[i] = back_color;
    }
    x = 2;
    y = 0;
    cx = cy = 0;
}

void initVBE(multiboot_t *info) {
    printf("Loading VBE...\n");
    vbe_status = true;

    x = 2;
    y = cx = cy = 0;
    screen = (uint32_t) info->framebuffer_addr;
    width = info->framebuffer_width;
    height = info->framebuffer_height;
    color = 0xFFFFFF;
    back_color = 0x310924;
    c_width = width / 9;
    c_height = height / 16;

    vbe_clear();

    /*
    for (int i = 0; i < c_height; i++){
        vbe_putchar('A');
        vbe_putchar('\n');
    }

    while (1) io_hlt();
     */
}
