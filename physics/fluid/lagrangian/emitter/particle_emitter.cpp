#include "particle_emitter.h"

HinaPE::ParticleEmitter3::ParticleEmitter3() : Kasumi::ArrowObject()
{
	NAME = "Emitter";
}

void HinaPE::ParticleEmitter3::INSPECT()
{
	Kasumi::ArrowObject::INSPECT();

	ImGui::Text("Emitter Physics");
	ImGui::Separator();
	INSPECT_BOOL(_opt.enable, "Enable");
	INSPECT_VEC3(_opt.origin, "origin");
	INSPECT_VEC3(_opt.direction, "direction");
	INSPECT_REAL(_opt.speed, "speed");
	INSPECT_REAL(_opt.spread_angle, "spread angle");

	_opt.origin = POSE.position;
	_opt.direction = mQuaternion(POSE.euler) * mVector3::UnitZ();
	_dirty = false;
}
