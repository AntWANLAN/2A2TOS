#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

uint64_t get_file_size(const uint8_t * filePath) {
    struct stat fileInfos;

    fstat(open((char *)filePath, O_RDONLY), &fileInfos);

    return fileInfos.st_size;
}

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

Ppm_image_t ppm_new(const uint8_t * pathname) {
    FILE * picture = fopen((char *)pathname, "r+b");
    Ppm_image_t imageInfos;
    uint64_t fileSize;
    uint8_t * line;

    fileSize = get_file_size(pathname);
    line = malloc(sizeof(uint8_t) * fileSize);
    if(line != NULL) {
        fgets((char *)line, fileSize, picture);
        fgets((char *)line, fileSize, picture);
    } else {
        fprintf(stderr, "Can't malloc line in 'ppm_new'");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", line);
    imageInfos.length = fileSize;
    imageInfos.height = 8;
    imageInfos.width = 8;
    imageInfos.pixelArray = malloc(sizeof(Pixel_t) * fileSize);
    
    return imageInfos;
}

int main(void) {
    Ppm_image_t image;
    image = ppm_new((uint8_t *)"image.ppm");

    printf("%ld", image.height);

    return EXIT_SUCCESS;
}