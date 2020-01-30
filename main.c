#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Pixel_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel_t;

typedef struct Ppm_image_t {
    size_t height;
    size_t width;
    size_t length;
    Pixel_t * pixelArray;
} Ppm_image_t;

Pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue) {
    Pixel_t newPixel;

    newPixel.red = red;
    newPixel.green = green;
    newPixel.blue = blue;

    return newPixel;
}

uint8_t pixel_red(const Pixel_t * p) {
    return p->red;
}
uint8_t pixel_green(const Pixel_t * p) {
    return p->green;
}
uint8_t pixel_blue(const Pixel_t * p) {
    return p->blue;
}

bool pixel_equals(const Pixel_t * self, const Pixel_t * other) {
    return (
        self->red == other->red
        && self->green == other->green
        && self->blue == other->blue
    );
}

Pixel_t pixel_invert(const Pixel_t * p) {
    Pixel_t invertedPixel;

    invertedPixel.blue = ~p->blue;
    invertedPixel.red = ~p->red;
    invertedPixel.green = ~p->green;

    return invertedPixel;
}

Ppm_image_t ppm_new(const char * pathname) {
    FILE * picture = fopen(pathname, "r+b");

    
}

int main(void) {

}