#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

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

int ppm_get_height(const Ppm_image_t * image){
    return image->height;
}

size_t ppm_get_width(const Ppm_image_t * image){
    return image->width;
}

size_t ppm_get_lenght(const Ppm_image_t * image){
    return image->length;
}

Pixel_t ppm_pixel(const Ppm_image_t * img, const size_t x, const size_t y) {
    return img->pixelArray[y + x * img->width];
}

Ppm_image_t ppm_new(const uint8_t * pathname) {
    FILE * picture = fopen((char *)pathname, "rb");
    Ppm_image_t imageInfos = {0, 0, 0, NULL};
    uint64_t fileSize;
    uint8_t * line;
    uint16_t i;

    fileSize = get_file_size(pathname);
    line = malloc(sizeof(uint8_t) * fileSize);
    if(line != NULL) {
        imageInfos.length = fileSize;

        fgets((char *)line, fileSize, picture); //image type
        fgets((char *)line, fileSize, picture); //image width and height
        while(isspace(line[i]))i++;
        imageInfos.width = atoi((char *)&line[i++]);

        while(isdigit(line[i]))i++;
        while(isspace(line[i]))i++;
        imageInfos.height = atoi((char *)&line[i++]);

        imageInfos.pixelArray = malloc(
                sizeof(Pixel_t) * imageInfos.width * imageInfos.height
            );
    } else {
        fprintf(stderr, "Can't malloc line in 'ppm_new'");
        exit(EXIT_FAILURE);
    }
    
    fclose(picture);
    
    return imageInfos;
}

void printPixel(const Pixel_t * pixel){
    printf("R: %d\tG: %d\tB: %d\n", pixel->red, pixel->green, pixel->blue);
}

void ppm_negatif(Ppm_image_t * img) {
    uint32_t x;
    uint32_t y;
    uint32_t i = 0;
    uint8_t fileName[256];
    FILE * file = NULL;
    Pixel_t pixel;

    for(x = 0; x < ppm_get_height(img); x++) {
        for(y = 0; y < ppm_get_width(img); y++) {
            pixel = ppm_pixel(img, x, y);
            img->pixelArray[i] = pixel_invert(&pixel);
            i++;
        }
    }
}

void loadImage(const uint8_t * pathname, Ppm_image_t * img) {
    FILE * picture;
    Pixel_t pixel;
    uint16_t i, j = 0;
    uint64_t size;
    uint8_t * buffer;

    picture = fopen((char *)pathname, "rb");

    size = img->width * 3;
    buffer = malloc(sizeof(uint8_t) * (size + 1));
    buffer[size] = '\0';

    fgets((char *)buffer, size, picture); //image type
    fgets((char *)buffer, size, picture); //image width and height
    fgets((char *)buffer, size, picture); //maximum value for each color

    for(i = 0; fread(buffer, sizeof(uint8_t), 3, picture), i < img->height * img->width && !feof(picture); i += 3) {
        pixel = pixel_new(
            (uint8_t)buffer[0],
            (uint8_t)buffer[1],
            (uint8_t)buffer[2]
        );
        img->pixelArray[j++] = pixel;
        printPixel(&pixel);
    }
    free(buffer);
    fclose(picture);
}

int main(void) {
    Ppm_image_t img;
    img = ppm_new((uint8_t *)"Aerial.512.ppm");

    printf("Image size %ld x %ld\n", img.width, img.height);
    loadImage((uint8_t *)"Aerial.512.ppm", &img);

    return EXIT_SUCCESS;
}