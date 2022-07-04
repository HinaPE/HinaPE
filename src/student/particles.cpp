
#include "../scene/particles.h"
#include "../rays/pathtracer.h"

bool Scene_Particles::Particle::update(const PT::Object& scene, float dt, float radius) {

    // TODO(Animation): Task 4

    // Compute the trajectory of this particle for the next dt seconds.

    // (1) Build a ray representing the particle's path if it travelled at constant velocity.

    // (2) Intersect the ray with the scene and account for collisions. Be careful when placing
    // collision points using the particle radius. Move the particle to its next position.

    // (3) Account for acceleration due to gravity.

    // (4) Repeat until the entire time step has been consumed.

    // (5) Decrease the particle's age and return whether it should die.

    return false;
}
