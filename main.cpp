#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include "../include/image.h"

namespace lab_bmp {
int32_t count_size(int32_t w, int32_t h) {
    return (3 * w + (4 - 3 * w % 4) % 4) * h;
}

std::size_t index(int32_t width, int32_t x, int32_t y, int32_t z) {
    return count_size(width, y) + 3 * x + z;
}

void image::rotate_clockwise() {
    std::vector<char> rotated_pixels(
        count_size(bitmapinfoheader.height, bitmapinfoheader.width));
    for (int32_t i = 0; i < bitmapinfoheader.height; i++) {
        for (int32_t j = 0; j < bitmapinfoheader.width; j++) {
            for (int32_t z = 0; z < 3; z++) {
                rotated_pixels[index(bitmapinfoheader.height, i,
                                     bitmapinfoheader.width - j - 1, z)] =
                    pixel_storage[index(bitmapinfoheader.width, j, i, z)];
            }
        }
    }
    pixel_storage = rotated_pixels;
    int32_t temp = bitmapinfoheader.width;
    bitmapinfoheader.width = bitmapinfoheader.height;
    bitmapinfoheader.height = temp;
}

void image::crop(int32_t x, int32_t y, int32_t w, int32_t h) {
    int32_t cropped_pixel_storage_size = count_size(w, h);
    std::vector<char> croped_pixel_storage(cropped_pixel_storage_size);
    y = bitmapinfoheader.height - y - h;
    for (int32_t j = y; j < y + h; j++) {
        for (int32_t i = x; i < x + w; i++) {
            for (int32_t k = 0; k < 3; k++) {
                croped_pixel_storage[index(w, i - x, j - y, k)] =
                    pixel_storage[index(bitmapinfoheader.width, i, j, k)];
            }
        }
    }
    pixel_storage.clear();
    bitmapinfoheader.width = w;
    bitmapinfoheader.height = h;
    bitmapfileheader.size = cropped_pixel_storage_size +
                            static_cast<int32_t>(sizeof(bitmapfileheader)) +
                            static_cast<int32_t>(sizeof(bitmapinfoheader));
    bitmapinfoheader.size_image = cropped_pixel_storage_size;
    pixel_storage.resize(cropped_pixel_storage_size);
    pixel_storage = croped_pixel_storage;
}

void image::read(const std::string &path) {
    std::ifstream file(path, std::ifstream::binary);
    file.read(reinterpret_cast<char *>(&bitmapfileheader),
              sizeof(bitmapfileheader));
    file.read(reinterpret_cast<char *>(&bitmapinfoheader),
              sizeof(bitmapinfoheader));
    pixel_storage.resize(bitmapinfoheader.size_image);
    file.read(reinterpret_cast<char *>(pixel_storage.data()),
              bitmapinfoheader.size_image);
}

void image::print(const std::string &path) {
    std::ofstream file(path, std::ofstream::binary);
    file.write(reinterpret_cast<char *>(&bitmapfileheader),
               sizeof(bitmapfileheader));
    file.write(reinterpret_cast<char *>(&bitmapinfoheader),
               sizeof(bitmapinfoheader));
    file.write(reinterpret_cast<char *>(pixel_storage.data()),
               bitmapinfoheader.size_image);
}
}  // namespace lab_bmp

int main(int count, char *args[]) {
    if (count != 8) {
        std::cerr << "Missing arguments"
                  << "\n";
        return 1;
    }
    lab_bmp::image image;
    if (std::strcmp(args[1], "crop-rotate") == 0) {
        image.read(args[2]);
    }
    image.crop(atoi(args[4]), atoi(args[5]), atoi(args[6]), atoi(args[7]));
    image.rotate_clockwise();
    image.print(args[3]);

    return 0;
}