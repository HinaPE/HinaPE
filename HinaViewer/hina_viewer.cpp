#include "hina_viewer.h"

HinaPE::Viewer::HinaViewer::HinaViewer(unsigned int width, unsigned int height, Eigen::Vector4d background_color) : width_(width), height_(height), background_color_(std::move(background_color))
{

}

HinaPE::Viewer::HinaViewer::~HinaViewer()
= default;
