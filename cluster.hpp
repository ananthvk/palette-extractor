#pragma once

#include "image.hpp"
#include <vector>

class Cluster
{
  public:
    virtual ~Cluster() {}
    virtual auto fit(const Image &img) -> void = 0;
    virtual auto labels() const -> const std::vector<Color>& = 0;
};

class KMeans : Cluster
{
    int k;
    std::vector<Color> centroids;

  public:
    KMeans(int k);
    auto fit(const Image &img) -> void;
    auto labels() const -> const std::vector<Color>&;
};