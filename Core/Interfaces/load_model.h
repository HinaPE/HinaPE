#ifndef HINAPE_LOAD_MODEL_H
#define HINAPE_LOAD_MODEL_H

#include "define.h"

#include <igl/readOBJ.h>

#include <string>
#include <tuple>

class LoadModel
{
public:
    inline static std::tuple<HMatrixXr, HMatrixXi> load_model(const std::string &name);
};

inline std::tuple<HMatrixXr, HMatrixXi> LoadModel::load_model(const std::string &name)
{
    HMatrixXr V;
    HMatrixXi F;

    igl::readOBJ(std::string(DATA_DIR) + std::string("models/") + name, V, F);

    return std::make_tuple(V, F);
}

#endif //HINAPE_LOAD_MODEL_H
