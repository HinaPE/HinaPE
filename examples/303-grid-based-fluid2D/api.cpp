#include "api.h"

#define U_FIELD 0
#define V_FIELD 1
#define S_FIELD 2

int cnt = 0;

GridBasedFluid2DExample::GridBasedFluid2DExample() {

}

GridBasedFluid2DExample::GridBasedFluid2DExample(float density, int numX, int numY, float h) {
    _density = density;
    _numX = numX + 2;
    _numY = numY + 2;
    _numCells = _numX * _numY;
    _h = h;
}

void GridBasedFluid2DExample::Init() {
    _u = new float[_numCells];
    _v = new float[_numCells];
    _newU = new float[_numCells];
    _newV = new float[_numCells];
    _p = new float[_numCells];
    _s = new float[_numCells];
    _m = new float[_numCells];
    _newM = new float[_numCells];
    for (int i = 0; i < _numCells; i++) {
        _u[i] = 0.0f;
        _v[i] = 0.0f;
        _newU[i] = 0.0f;
        _newV[i] = 0.0f;
        _p[i] = 0.0f;
        _s[i] = 0.0f;
        _m[i] = 0.0f;
        _newM[i] = 0.0f;
    }
}

void GridBasedFluid2DExample::integrate(float dt, float gravity) {
    int n = _numY;
    for(int i = 1; i < _numX; i++) {
        for(int j = 1; j < _numY; j++) {
            if(_s[i * n + j] != 0.0 && _s[i * n + j - 1] != 0.0){
                _v[i * n + j] +=  gravity * dt;
            }
        }
    }
}

void GridBasedFluid2DExample::solveIncompressibility(int numIters, float dt) const {
    int n = _numY;
    float cp = _density * _h / dt;

    for(int iter = 0; iter < numIters; iter++) {
        for(int i = 1; i < _numX - 1; i++) {
            for(int j = 1; j < _numY - 1; j++) {
                if(_s[i * n + j] != 0.0)
                    continue;

                float s = _s[i * n + j];
                float sx0 = _s[(i - 1) * n + j];
                float sx1 = _s[(i + 1) * n + j];
                float sy0 = _s[i * n + j - 1];
                float sy1 = _s[i * n + j + 1];
                if(s == 0.0)
                    continue;

                float div = _u[(i + 1) * n + j] - _u[i * n + j] + _v[i * n + j + 1] - _v[i * n + j];
                float p = -div / s;

                float overRelaxation = 1.9;
                p *= overRelaxation;
                _p[i * n + j] += cp * p;

                _u[i * n + j] -= p * sx0;
                _u[(i + 1) * n + j] += p * sx1;
                _v[i * n + j] -= p * sy0;
                _v[i * n + j + 1] += p * sy1;
            }
        }
    }
}

void GridBasedFluid2DExample::extrapolate() {
    int n = _numY;
    for(int i = 0; i <_numX; i++)
    {
        _u[i * n] = _u[i * n + 1];
        _u[i * n + _numY - 1] = _u[i * n + _numY - 2];
    }
    for(int j = 0; j < _numY; j++)
    {
        _v[j] = _v[n + j];
        _u[(_numX - 1) * n + j] = _u[(_numX - 2) * n + j];
    }
}

float GridBasedFluid2DExample::sampleField(float x, float y, int field) const {
    int n = _numY;
    float h = _h;
    float h1 = 1.0f / h;
    float h2 = 0.5f * h;
    
    x = std::max(std::min(x, _numX * h), h);
    y = std::max(std::min(y, _numY * h), h);
    
    float dx = 0.f;
    float dy = 0.f;

    float *f;
    switch (field) {
        case U_FIELD:
            f = _u;
            dy = h2;
            break;
        case V_FIELD:
            f = _v;
            dx = h2;
            break;
        case S_FIELD:
            f = _m;
            dx = h2;
            dy = h2;
            break;
    }

    float x0 = std::min(std::floor((x - dx) * h1), _numX - 1.0f);
    float tx = ((x - dx) - x0 * h) * h1;
    float x1 = std::min(x0 + 1.0f, _numX - 1.0f);

    float y0 = std::min(std::floor((y - dy) * h1), _numY - 1.0f);
    float ty = ((y - dy) - y0 * h) * h1;
    float y1 = std::min(y0 + 1.0f, _numY - 1.0f);

    float sx = 1.0 - tx;
    float sy = 1.0 - ty;

    float val = sx * sy * f[(int)x0 * n + (int)y0] + tx * sy * f[(int)x1 * n + (int)y0] + sx * ty * f[(int)x0 * n + (int)y1] + tx * ty * f[(int)x1 * n + (int)y1];
    return val;
}

float GridBasedFluid2DExample::avgU(int i, int j) {
    int n = _numY;
    float u = (_u[i * n + j - 1] + _u[i * n + j]) + _u[(i + 1) * n + j - 1] + _u[(i + 1) * n + j] * 0.25;
    return u;
}

float GridBasedFluid2DExample::avgV(int i, int j) {
    int n = _numY;
    float v = (_v[i * n + j] + _v[i * n + j + 1]) + _v[(i - 1) * n + j] + _v[(i - 1) * n + j + 1] * 0.25;
    return 0;
}

void GridBasedFluid2DExample::advectVel(float dt) {
    _newU = _u;
    _newV = _v;

    int n = _numY;
    float h = _h;
    float h2 = 0.5f * h;

    for(int i = 0; i < _numX; i++) {
        for(int j = 0; j < _numY; j++) {
            cnt++;

            // u component
            if(_s[i * n + j] != 0.0 && _s[(i - 1) * n + j] != 0.0 && j < _numY - 1) {
                float u = _u[i * n + j];
                float v = avgV(i, j);
                float x = i * h;
                float y = j * h + h2;
                //float v = sampleField(x, y, V_FIELD);
                _newU[i * n + j] = sampleField(x, y, U_FIELD);
            }

            // v component
            if(_s[i * n + j] != 0.0 && _s[i * n + j - 1] != 0.0 && i < _numX - 1) {
                float u = avgU(i, j);
                float v = _v[i * n + j];
                float x = i * h + h2;
                float y = j * h;
                x = x - dt * 0.5;
                //float u = sampleField(x, y, U_FIELD);
                _newV[i * n + j] = sampleField(x, y, V_FIELD);
            }
        }
    }
    _u = _newU;
    _v = _newV;
}

void GridBasedFluid2DExample::advectSmoke(float dt) {
    _newM = _m;

    int n = _numY;
    float h = _h;
    float h2 = 0.5f * h;

    for(int i = 1; i < _numX - 1; i++) {
        for(int j = 1; j < _numY - 1; j++) {
            cnt++;

            // u component
            if(_s[i * n + j] != 0.0) {
                float u = (_u[i * n + j] + _u[(i + 1) * n + j]) * 0.5;
                float v = (_v[i * n + j] + _v[i * n + j + 1]) * 0.5;
                float x = i * h + h2 - dt * u;
                float y = j * h + h2 - dt * v;
                //float v = sampleField(x, y, V_FIELD);
                _newM[i * n + j] = sampleField(x, y, S_FIELD);
            }
        }
    }
    _m = _newM;
}

void GridBasedFluid2DExample::simulate(float dt, float gravity, int numIters) {
    integrate(dt, gravity);
    solveIncompressibility(numIters,dt);
    extrapolate();
    advectVel(dt);
    advectSmoke(dt);
}

void GridBasedFluid2DExample::ui_sidebar() {
    
}








