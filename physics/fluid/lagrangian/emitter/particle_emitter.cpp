#include "particle_emitter.h"

HinaPE::ParticleEmitter3::ParticleEmitter3() : Kasumi::ArrowObject() { _rebuild_(); }

void HinaPE::ParticleEmitter3::INSPECT()
{
	Kasumi::ArrowObject::INSPECT();

	ImGui::Text("Emitter Physics");
	ImGui::Separator();
	INSPECT_BOOL(_opt.enable, "Enable");
}
