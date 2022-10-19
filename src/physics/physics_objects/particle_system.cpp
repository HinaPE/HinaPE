//
// Created by 86151 on 2022/10/19.
//
#include "particle_system.h"

int Particle::count = 0;

Particle::Particle(float mass, float size, Vec3 position, Vec3 velocity) {
    this->mass = mass;
    this->size = size;
    this->position = position;
    this->velocity = velocity;

    force = Vec3(0.0f,0.0f,0.0f);
    acceleration = Vec3(0.0f,0.0f,0.0f);

    density = 0.0f;
    pressure = 0.0f;
    id = count++;

    next = NULL;
}

Particle::~Particle() {

}
