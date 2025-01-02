#include "cluster.hpp"
#include "image.hpp"
#include <iostream>
using std::swap;

auto main(int argc, char *argv[]) -> int
{
    if (argc != 3)
    {
        std::cerr << "Usage: palette extract <image_file>" << std::endl;
        exit(1);
    }
    try
    {
        Image img = Image::from_file(argv[2]);
        std::cout << "Resolution: " << img.width() << "x" << img.height() << std::endl;
        std::cout << "Number of channels: " << img.channels() << std::endl;
        KMeans kmeans(5);
        kmeans.fit(img);
        std::cout << "Dominant colors: " << std::endl;
        for (const auto &label : kmeans.labels())
        {
            std::cout << label.format_hex() << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        exit(2);
    }
}