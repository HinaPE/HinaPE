/**
 * @authored by Xayah, BFU
 */
#ifndef ASSIGNMENT2_CONFIG_H
#define ASSIGNMENT2_CONFIG_H

#include "camera.h"
#include <string>

struct settings {
    settings() {
        default_sets();
    }
    void default_sets() {
        window_name = "I love Graphics!"; // Fait Lux! :P
        window_width = 1200;
        window_height = 800;
    }
    std::string window_name;
    int window_width;
    int window_height;
};

settings default_setting;
GLFWwindow* global_mainWindow;
camera2D global_camera;

#endif //ASSIGNMENT2_CONFIG_H
