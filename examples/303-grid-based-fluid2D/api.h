#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "renderer/scene/scene.h"

class GridBasedFluid2DExample : public Kasumi::Api
{
public:
    void ui_sidebar() final;
private:
    struct Fluid2dOpt
    {
        int numIters = 100;
        float gravity = 9.8f;
        float dt = 1 / 120;

    };
public:
    GridBasedFluid2DExample();
    GridBasedFluid2DExample(float density,int numX,int numY,float h);
    void Init();
    void integrate(float dt, float gravity);
    void solveIncompressibility(int numIters, float dt) const;
    void extrapolate();
    [[nodiscard]] float sampleField(float x, float y, int field) const;
    float avgU(int i, int j);
    float avgV(int i, int j);
    void advectVel(float dt);
    void advectSmoke(float dt);
    void simulate(float dt, float gravity, int numIters);
public:
    float _density;
    int _numX;
    int _numY;
    int _numCells;
    float _h;
    float *_u;
    float *_v;
    float *_newU;
    float *_newV;
    float *_p;
    float *_s;
    float *_m;
    float *_newM;
};

#endif //HINAPE_API_H
