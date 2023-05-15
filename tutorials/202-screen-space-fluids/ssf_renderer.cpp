#include "ssf_renderer.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(ScreenSpaceFluids);

void HinaPE::SSFRenderer::init_shaders()
{
	auto fs = cmrc::ScreenSpaceFluids::get_filesystem();
	auto f_blur = fs.open("shaders/blur.frag").begin();
	auto v_blur = fs.open("shaders/blur.vert").begin();
	auto f_curvatureflow = fs.open("shaders/curvatureflow.frag").begin();
	auto g_particle = fs.open("shaders/particle.geo").begin();
	auto v_particle = fs.open("shaders/particle.vert").begin();
	auto f_particle_depth = fs.open("shaders/particleDepth.frag").begin();
	auto f_particle_noise = fs.open("shaders/particleNoise.frag").begin();
	auto f_particle_thickness = fs.open("shaders/particleThickness.frag").begin();
	auto f_quad = fs.open("shaders/quad.frag").begin();
	auto v_quad = fs.open("shaders/quad.vert").begin();
	auto f_quad_thickness = fs.open("shaders/quadThickness.frag").begin();
	auto f_quadtrans = fs.open("shaders/quadtrans.frag").begin();
	auto v_quadtrans = fs.open("shaders/quadtrans.vert").begin();

	_shader = std::make_shared<Kasumi::Shader>(v_particle, f_particle_depth, g_particle);
	_thickness_shader = std::make_shared<Kasumi::Shader>(v_particle, f_particle_thickness, g_particle);
	_noise_shader = std::make_shared<Kasumi::Shader>(v_particle, f_particle_noise, g_particle);
	_blur_shader = std::make_shared<Kasumi::Shader>(v_blur, f_blur);
	curvature_flow_shader = std::make_shared<Kasumi::Shader>(v_blur, f_curvatureflow);
	quad_thickness_shader = std::make_shared<Kasumi::Shader>(v_quad, f_quad_thickness);
	quad_trans_shader = std::make_shared<Kasumi::Shader>(v_quadtrans, f_quadtrans);
}
