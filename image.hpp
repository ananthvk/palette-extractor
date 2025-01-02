#pragma once
#include "stb_image.h"
#include <stdexcept>
#include <string>

class image_error : public std::runtime_error
{
  public:
    image_error(const std::string &message) : std::runtime_error(message) {}
};

class Image
{
  private:
    std::string file_path_;
    int width_;
    int height_;
    int num_components_;
    unsigned char *buffer_;
    // Was the buffer created by the library or did we create it
    bool is_buffer_created_by_stbi;
    Image();

  public:
    static Image from_file(const std::string &path);
    auto width() const -> int;
    auto height() const -> int;
    auto file_path() const -> std::string;
    auto channels() const -> int;
    // Returns the size of pixel data in bytes
    auto size() const -> int;

    // Returns pointer to underlying data buffer
    auto buffer() -> unsigned char *;

    ~Image();
    Image(const Image &other);
    Image &operator=(Image other);
    Image(Image &&other) noexcept;
    Image &operator=(Image &&other) noexcept;

    friend void swap(Image &first, Image &second)
    {
        using std::swap;
        swap(first.file_path_, second.file_path_);
        swap(first.width_, second.width_);
        swap(first.height_, second.height_);
        swap(first.num_components_, second.num_components_);
        swap(first.buffer_, second.buffer_);
        swap(first.is_buffer_created_by_stbi, second.is_buffer_created_by_stbi);
    }
};
