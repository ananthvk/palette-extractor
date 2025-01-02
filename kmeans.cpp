#include "cluster.hpp"
#include <iostream>
#include <random>

KMeans::KMeans(int k) : k(k), centroids(k, Color{}) {}

auto KMeans::fit(const Image &img) -> void
{
    // Steps:
    // 1) Initialize the centroids to random points
    // 2) Assign each point to it's closest centroid
    // 3) Shift the centroid to the average of items in its cluster

    // TODO: Apply kmeans++
    // For now, just set it to random points
    std::random_device device;
    auto seed = device();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    for (int i = 0; i < k; ++i)
    {
        centroids[i].r = dist(rng);
        centroids[i].g = dist(rng);
        centroids[i].b = dist(rng);
        centroids[i].a = dist(rng);
    }

    std::vector<int> clusters(img.width() * img.height(), 0);
    int max_number_iterations = 1000;
    for (int iter = 0; iter < max_number_iterations; ++iter)
    {

        // TODO: Also try looping over the buffer and check if the performance is better
        for (int i = 0; i < img.height(); ++i)
        {
            for (int j = 0; j < img.width(); ++j)
            {
                auto color = img.at_rgba(i, j);
                auto minimum_distance = std::numeric_limits<int>::max();
                int best_cluster = 0;

                for (int m = 0; m < k; ++m)
                {
                    auto new_distance = color.distance_squared(centroids[m]);
                    if (new_distance < minimum_distance)
                    {
                        minimum_distance = new_distance;
                        best_cluster = m;
                    }
                }
                // Assign this pixel to it's closest cluster
                clusters[i * img.width() + j] = best_cluster;
            }
        }

        // Shift each cluster
        // TODO: Check for overflow
        std::vector<std::vector<long long int>> sum_of_colors(k, std::vector<long long int>(4, 0));
        std::vector<int> number_of_points_in_cluster(k, 0);

        for (size_t i = 0; i < clusters.size(); ++i)
        {
            auto row = i / img.width();
            auto col = i % img.width();
            auto color = img.at_rgba(row, col);
            sum_of_colors[clusters[i]][0] += color.r;
            sum_of_colors[clusters[i]][1] += color.g;
            sum_of_colors[clusters[i]][2] += color.b;
            sum_of_colors[clusters[i]][3] += color.a;
            number_of_points_in_cluster[clusters[i]]++;
        }
        int number_of_centroids_changed = 0;
        for (int i = 0; i < k; ++i)
        {
            if (number_of_points_in_cluster[i])
            {
                auto previous_centroid = centroids[i];
                centroids[i].r = sum_of_colors[i][0] / number_of_points_in_cluster[i];
                centroids[i].g = sum_of_colors[i][1] / number_of_points_in_cluster[i];
                centroids[i].b = sum_of_colors[i][2] / number_of_points_in_cluster[i];
                centroids[i].a = sum_of_colors[i][3] / number_of_points_in_cluster[i];

                if (!(centroids[i].r == previous_centroid.r &&
                      centroids[i].g == previous_centroid.g &&
                      centroids[i].b == previous_centroid.b &&
                      centroids[i].a == previous_centroid.a))
                {
                    ++number_of_centroids_changed;
                }
            }
        }
        // If no centroids have shifted, we have converged to a solution
        if (number_of_centroids_changed == 0)
        {
            std::cout << "Converged in " << (iter + 1) << " iterations" << std::endl;
            break;
        }
    }
}

auto KMeans::labels() const -> const std::vector<Color> & { return centroids; }