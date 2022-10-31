#ifndef HINAPE_HDR_IMAGE_H
#define HINAPE_HDR_IMAGE_H

#include <vector>

#include "../lib/spectrum.h"
#include "../platform/gl.h"

class HDR_Image
{
public:
    HDR_Image();
    HDR_Image(size_t w, size_t h);
    HDR_Image(const HDR_Image &src) = delete;
    HDR_Image(HDR_Image &&src) = default;
    ~HDR_Image() = default;

    auto copy() const -> HDR_Image;

    auto operator=(const HDR_Image &src) -> HDR_Image & = delete;
    auto operator=(HDR_Image &&src) -> HDR_Image & = default;

    auto at(size_t x, size_t y) -> Spectrum &;
    auto at(size_t x, size_t y) const -> Spectrum;
    auto at(size_t i) -> Spectrum &;
    auto at(size_t i) const -> Spectrum;

    void clear(Spectrum color);
    void resize(size_t w, size_t h);
    auto dimension() const -> std::pair<size_t, size_t>;

    auto load_from(std::string file) -> std::string;
    auto loaded_from() const -> std::string;

    void tonemap_to(std::vector<unsigned char> &data, float exposure = 0.0f) const;
    auto get_texture(float exposure = 0.0f) const -> const GL::Tex2D &;

private:
    void tonemap(float exposure = 0.0f) const;

    size_t w, h;
    std::string last_path;
    std::vector<Spectrum> pixels;

    mutable GL::Tex2D render_tex;
    mutable float exposure = 1.0f;
    mutable bool dirty = true;
};

#endif
