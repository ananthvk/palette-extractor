#include "cluster.hpp"
#include "image.hpp"
#include <iostream>
using std::swap;

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