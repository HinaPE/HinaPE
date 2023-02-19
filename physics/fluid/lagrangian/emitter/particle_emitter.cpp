#include "particle_emitter.h"

ParticleEmitter3::ParticleEmitter3() = default;

void ParticleEmitter3::INSPECT()
{
	ImGui::Text("Emitter");
	ImGui::Separator();
	INSPECT_BOOL(_opt.enable, "Enable");
}
