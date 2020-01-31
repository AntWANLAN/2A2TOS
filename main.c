#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

Ppm_image_t ppm_new(const uint8_t * pathname) {
    FILE * picture = fopen((char *)pathname, "r+b");
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

        imageInfos.pixelArray = malloc(sizeof(Pixel_t) * (fileSize / 3));
    } else {
        fprintf(stderr, "Can't malloc line in 'ppm_new'");
        exit(EXIT_FAILURE);
    }
    
    
    return imageInfos;
}

int main(void) {
    Ppm_image_t image;
    image = ppm_new((uint8_t *)"Aerial.512.ppm");

    printf("%ld x %ld\n", image.width, image.height);

    // for(int i = 0; i < 6; i++) {
    //     printf("%c %c %c\n",
    //         image.pixelArray[i].red,
    //         image.pixelArray[i].green,
    //         image.pixelArray[i].blue
    //     );
    // }
    
    /* question 11
    imageInfos.pixelArray = malloc(sizeof(Pixel_t) * (fileSize / 3));
    fgets((char *)line, fileSize, picture); //maximum value for each color
    fgets((char *)line, fileSize, picture);
    while(line[0] == '#') {
        fgets((char *)line, fileSize, picture);
        printf("%s", (char *)line);
    }
    for(i = 0; i < sizeof(Pixel_t) * (fileSize / 3); i++) {
            sscanf((char *)line, "%u %u %u", &red, &green, &blue);
            pixel.red = red;
            pixel.green = green;
            pixel.blue = blue;
            imageInfos.pixelArray[i] = pixel;
            fgets((char *)line, fileSize, picture);
    }
    */

    return EXIT_SUCCESS;
}