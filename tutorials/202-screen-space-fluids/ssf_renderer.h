#ifndef HINAPE_SSF_RENDERER_H
#define HINAPE_SSF_RENDERER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/shader.h"

namespace HinaPE
{
class SSFRenderer
{
public:
	SSFRenderer();

private:
	Kasumi::ShaderPtr
			_shader,
			_thickness_shader,
			_noise_shader,
			_blur_shader,
			curvature_flow_shader,
			quad_thickness_shader,
			quad_trans_shader;
};
} // namespace HinaPE

#endif //HINAPE_SSF_RENDERER_H
