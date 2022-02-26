#ifndef IMAGE_H_
#define IMAGE_H_
#include <string>
#include <vector>

namespace lab_bmp {
#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    int16_t type;
    int32_t size;
    int16_t reserved1;
    int16_t reserved2;
    int32_t off_bits;
};
struct BITMAPINFOHEADER {
    int32_t size;
    int32_t width;
    int32_t height;
    int16_t planes;
    int16_t bit_count;
    int32_t compression;
    int32_t size_image;
    int32_t pels_per_meter_x;
    int32_t pels_per_meter_y;
    int32_t clr_used;
    int32_t clr_important;
};
#pragma pack(pop)
struct image {  // NOLINT(cppcoreguidelines-pro-type-member-init)
private:
    BITMAPFILEHEADER bitmapfileheader;
    BITMAPINFOHEADER bitmapinfoheader;
    std::vector<char> pixel_storage;

public:
    image() = default;
    void print(const std::string &path);
    void crop(int32_t x, int32_t y, int32_t w, int32_t h);
    void rotate_clockwise();
    void read(const std::string &path);
};
}  // namespace lab_bmp

#endif  // IMAGE_H_