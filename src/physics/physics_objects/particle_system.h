//
// Created by 86151 on 2022/10/19.
//

#include "lib/vec3.h"

#ifndef HINAPE_PARTICLE_SYSTEM_H
#define HINAPE_PARTICLE_SYSTEM_H

class Particle
{
public:
    float mass, size, elasticity;
    float density, pressure, id;
    static int count;

    Vec3 position, velocity, acceleration;
    Vec3 force;
    Particle* next;

    Particle(float mass, float size, Vec3 position, Vec3 velocity);
    ~Particle();
};

#endif //HINAPE_PARTICLE_SYSTEM_H
