#pragma once

#include "image.hpp"
#include <vector>

class Cluster
{
  public:
    virtual ~Cluster() {}

    virtual auto fit(const std::vector<Color> &colors, int n_iters) -> int = 0;
    virtual auto labels() const -> const std::vector<Color> & = 0;
    virtual auto predict(const std::vector<Color> &colors) -> std::vector<int> = 0;
    virtual auto fit_predict(const std::vector<Color> &colors, int n_iters) -> std::vector<int> = 0;
};

class KMeans : Cluster
{
    int k;
    std::vector<Color> centroids;
    auto initialize(const std::vector<Color> &colors) -> void;
    auto assign_points_to_clusters(const std::vector<Color> &colors,
                                   std::vector<int> &clusters) -> void;

    // @return `true` if any of the centroids shifted during the iteration
    auto update_centroids(const std::vector<Color> &colors,
                          const std::vector<int> &clusters) -> bool;

  public:
    KMeans(int k);
    auto fit(const std::vector<Color> &colors, int n_iters = 100) -> int;
    auto labels() const -> const std::vector<Color> &;
    auto predict(const std::vector<Color> &colors) -> std::vector<int>;
    auto fit_predict(const std::vector<Color> &colors, int n_iters = 100) -> std::vector<int>;
};