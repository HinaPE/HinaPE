#ifndef HINAPE_RENDERABLE_H
#define HINAPE_RENDERABLE_H

#include "define.h"

class Renderable
{
public:

    /// Get Render Particle
    virtual HMatrixXr &get_render_particle() = 0;

    /// Get Render Mesh
    virtual HMatrixXi &get_render_mesh() = 0;
};

#endif //HINAPE_RENDERABLE_H
