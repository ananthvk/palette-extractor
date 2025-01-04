#include "cluster.hpp"
#include "image.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
using std::swap;

auto main(int argc, char *argv[]) -> int
{
    if (argc != 5)
    {
        std::cerr << "Usage: palette reduce <number_of_colors> <image_file_to_extract_palette> "
                     "<image_file>"
                  << std::endl;
        exit(1);
    }
    try
    {
        Image palette_img = Image::from_file(argv[3]);
        std::cout << "Resolution: " << palette_img.width() << "x" << palette_img.height() << std::endl;
        if (palette_img.channels() < 3)
        {
            std::cout << "Error: Number of channels: " << palette_img.channels()
                      << ", image should have 3/4 channels" << std::endl;
            exit(1);
        }

        int k = 0;
        std::istringstream iss(argv[2]);
        iss >> k;
        if (!iss || k < 0)
        {
            std::cerr << "Invalid number of colors" << std::endl;
            exit(1);
        }

        auto colors = palette_img.get_colors();
        KMeans kmeans(k);
        kmeans.fit(colors);

        Image img2 = Image::from_file(argv[4]);
        auto colors_clustered = kmeans.predict(img2.get_colors());

        auto labels = kmeans.labels();

        auto converted_image = Image::create(img2.width(), img2.height(), img2.channels());
        unsigned char *buffer = converted_image.buffer();
        for (size_t i = 0; i < (img2.width() * img2.height()); ++i)
        {
            unsigned char *ptr = buffer + (img2.channels() * i);
            auto color = labels[colors_clustered[i]];

            ptr[0] = color.r;
            ptr[1] = color.g;
            ptr[2] = color.b;
            if (img2.channels() == 4)
            {
                ptr[3] = color.a;
            }
        }
        stbi_write_png("output.png", img2.width(), img2.height(), img2.channels(), buffer,
                       img2.width() * img2.channels());
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(2);
    }
}

/*
auto main(int argc, char *argv[]) -> int
{
    if (argc != 4)
    {
        std::cerr << "Usage: palette reduce <number_of_colors> <image_file>" << std::endl;
        exit(1);
    }
    try
    {
        Image img = Image::from_file(argv[3]);
        std::cout << "Resolution: " << img.width() << "x" << img.height() << std::endl;
        if (img.channels() < 3)
        {
            std::cout << "Error: Number of channels: " << img.channels()
                      << ", image should have 3/4 channels" << std::endl;
            exit(1);
        }

        int k = 0;
        std::istringstream iss(argv[2]);
        iss >> k;
        if (!iss || k < 0)
        {
            std::cerr << "Invalid number of colors" << std::endl;
            exit(1);
        }

        auto colors = img.get_colors();
        KMeans kmeans(k);
        auto colors_clustered = kmeans.fit_predict(colors);
        auto labels = kmeans.labels();

        auto converted_image = Image::create(img.width(), img.height(), img.channels());
        unsigned char *buffer = converted_image.buffer();
        for (size_t i = 0; i < (img.width() * img.height()); ++i)
        {
            unsigned char *ptr = buffer + (img.channels() * i);
            auto color = labels[colors_clustered[i]];

            ptr[0] = color.r;
            ptr[1] = color.g;
            ptr[2] = color.b;
            if (img.channels() == 4)
            {
                ptr[3] = color.a;
            }
        }
        stbi_write_png("output.png", img.width(), img.height(), img.channels(), buffer,
                       img.width() * img.channels());
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(2);
    }
}
*/
/*
auto main(int argc, char *argv[]) -> int
{
    if (argc != 4)
    {
        std::cerr << "Usage: palette extract <number_of_colors> <image_file>" << std::endl;
        exit(1);
    }
    try
    {
        Image img = Image::from_file(argv[3]);
        std::cout << "Resolution: " << img.width() << "x" << img.height() << std::endl;
        if (img.channels() < 3)
        {
            std::cout << "Error: Number of channels: " << img.channels()
                      << ", image should have 3/4 channels" << std::endl;
            exit(1);
        }

        int k = 0;
        std::istringstream iss(argv[2]);
        iss >> k;
        if (!iss || k < 0)
        {
            std::cerr << "Invalid number of colors" << std::endl;
            exit(1);
        }

        auto colors = img.get_colors();
        KMeans kmeans(k);
        auto iterations = kmeans.fit(colors);
        std::cout << "Converged in " << iterations << " iterations" << std::endl;
        std::cout << "Dominant colors: " << std::endl;

        auto &labels = kmeans.labels();

        for (int i = 0; i < k; ++i)
        {
            std::cout << labels[i].format_hex() << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(2);
    }
}
*/