#ifndef ASSIGNMENT0_BASICSHAPES_H
#define ASSIGNMENT0_BASICSHAPES_H

#include "Eigen/Eigen"

#include <iostream>
#include <utility>
#include <vector>
#include <utility>

enum DRAW_MODE {
    POINTS, LINES, TRIANGLES
};

class BasicShape2D {

public:
    explicit BasicShape2D(Eigen::Vector2f center);

    void set_color(Eigen::Vector3f &RGB);

    void set_color(Eigen::Vector4f &RGBA);

    void set_draw_mode(DRAW_MODE mode);

    void set_color(int HEX);

    virtual void generate_data() = 0;

    virtual std::vector<float> get_positions() = 0;

public:
    std::vector<Eigen::Vector2f> positions;
    Eigen::Vector4f color;
    DRAW_MODE draw_mode;
    Eigen::Vector2f center;
    float rotation_angle;
};

class Rectangle : public BasicShape2D {

public:

    Rectangle(Eigen::Vector2f center, float harf_width, float harf_height);

    void generate_data() override;

    std::vector<float> get_positions() override;

private:
    float harf_width;
    float harf_height;
};

#endif //ASSIGNMENT0_BASICSHAPES_H
