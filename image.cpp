#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>

Image::Image()
    : width_(0), height_(0), num_components_(0), buffer_(nullptr), is_buffer_created_by_stbi(false)
{
}

Image Image::from_file(const std::string &path)
{
    Image image;
    image.file_path_ = path;
    image.buffer_ =
        stbi_load(path.c_str(), &image.width_, &image.height_, &image.num_components_, 0);
    image.is_buffer_created_by_stbi = true;
    if (!image.buffer_)
    {
        std::ostringstream oss;
        oss << "Error while loading image \"" << path << "\": ";
        oss << stbi_failure_reason();
        throw image_error(oss.str());
    }
    return image;
}

auto Image::width() const -> int { return width_; }

auto Image::height() const -> int { return height_; }

auto Image::file_path() const -> std::string { return file_path_; }

auto Image::channels() const -> int { return num_components_; }

auto Image::buffer() -> unsigned char * { return buffer_; }

auto Image::size() const -> int { return width_ * height_ * num_components_; }

Image::~Image()
{
    if (buffer_)
    {
        if (is_buffer_created_by_stbi)
        {
            stbi_image_free(buffer_);
        }
        else
        {
            free(buffer_);
        }
        buffer_ = nullptr;
    }
}

Image::Image(const Image &other)
{
    file_path_ = other.file_path_;
    width_ = other.width_;
    height_ = other.height_;
    num_components_ = other.num_components_;
    is_buffer_created_by_stbi = false;
    buffer_ = nullptr;
    if (other.size() != 0)
    {
        buffer_ = static_cast<unsigned char *>(malloc(size()));
        if (!buffer_)
        {
            throw image_error("Could not allocate memory to copy the image");
        }
    }
}

Image &Image::operator=(Image other)
{
    swap(*this, other);
    return *this;
}

Image::Image(Image &&other) noexcept : Image() { swap(*this, other); }

Image &Image::operator=(Image &&other) noexcept
{
    swap(*this, other);
    return *this;
}