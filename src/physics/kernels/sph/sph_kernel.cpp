#include "sph_kernel.h"

#include <cmath>
#define PI 3.14

auto HinaPE::SPHKernel::init_particle_system() -> void {

}

auto HinaPE::SPHKernel::buildTable() -> void {
    for(int i = 0; i < 1000000;i++){
        particleTable[i] = nullptr;
    }
    /// build hash table
    Particle* p;
    for(auto & particle : particles){
        p = particle;

        int index = getHash(getCell(p));

        if (particleTable[index] == nullptr) {
            p->next = nullptr;
            particleTable[index] = p;
        }
        else {
            p->next = particleTable[index];
            particleTable[index] = p;
        }
    }
}

auto HinaPE::SPHKernel::getHash(Vec3 cell) -> unsigned int {
    cell.x *= 73856093;
    cell.y *= 19349663;
    cell.z *= 83492791;
    return ((unsigned int)cell.x ^ (unsigned int)cell.y ^ (unsigned int)cell.z) % 1000000;
}

auto HinaPE::SPHKernel::getCell(Particle *p) const -> Vec3 {
    /// position
    return Vec3(p->position.x / h, p->position.y / h, p->position.z / h);
}

auto HinaPE::SPHKernel::simulate(float dt) -> void
{
    buildTable();
    for(auto pi : particles){
        float pDensity = 0;
        Vec3 cell = getCell(pi);

        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                for (int z = -1; z <= 1; z++)
                {
                    Vec3 near_cell = cell + Vec3(x, y, z);
                    unsigned int index = getHash(near_cell);
                    Particle* pj = particleTable[index];

                    while (pj != nullptr) {
                        float dist2 = (pj->position.x - pi->position.x) * (pj->position.x - pi->position.x) +
                                (pj->position.y - pi->position.y) * (pj->position.y - pi->position.y) +
                                (pj->position.z - pi->position.z) * (pj->position.z - pi->position.z);
                        if (dist2 < (h * h) && pi != pj) {
                            pDensity += MASS * (315 / (64 * PI)) * (h * h * h) * pow(1 - (dist2 / (h * h)), 3);
                        }
                        pj = pj->next;
                    }
                }
            }
        }

        /// Include self density
        pi->density = pDensity + self_density;

        ///  Calculate pressure
        float Pressure = (eosScale / eosExponent) * pow(pi->density/targetDensity -1, eosExponent);
        pi->pressure = Pressure;
    }

    /// calculate forces
    for (auto pi : particles){
        pi->force = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 cell = getCell(pi);

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                for (int z = -1; z <= 1; z++) {
                    Vec3 NearCell = cell + Vec3(x, y, z);
                    unsigned int index = getHash(NearCell);
                    Particle* pj = particleTable[index];

                    while(pj!=NULL){
                        float dist2 = (pj->position.x - pi->position.x) * (pj->position.x - pi->position.x) +
                                (pj->position.y - pi->position.y) * (pj->position.y - pi->position.y) +
                                (pj->position.z - pi->position.z) * (pj->position.z - pi->position.z);
                        if (dist2 < (h * h) && pi != pj) {
                            Vec3 dist = pj->position - pi->position;
                            float distLen = sqrt(dist2);
                            Vec3 dir = normaize(dist);
                            Vec3 pressureForce = -dir * MASS * (pi->pressure + pj->pressure) / (2 * pj->density) * (-45.0f / (PI * pow(h, 6)));
                            Vec3 viscosityForce = (viscosity * MASS / pj->density) * (pj->velocity - pi->velocity) * (h - dist) * (45.0f / (PI * pow(h, 6)));
                            pi->force += pressureForce + viscosityForce;
                        }
                        pj = pj -> next;
                    }
                }
            }
        }
    }
    /// update particle positions
    for (auto pi : particles){
        Particle *p = particles[i];
        Vec3 acceleration = p->force / p->density + Vec3(0.f, -9.8f, 0.f);
        p->velocity += p->force / p->density * dt;
        p->position += p->velocity * dt;
    }
}

//float HinaPE::SPHKernel::cubic_kernel(float r_norm) {
//    float res = 0.0;
//    /// particle_radius
//    float particle_radius = 0.01;
//    float h = 4.0 * particle_radius;
//    float k = 1.0;
//
//    /// dim  domain bounding box
//    float dim;
//
//    if(dim == 1){
//        k = (float) 4 / 3;
//    }else if(dim == 2){
//        k = (float) 40 / 7 / PI;
//    }else if(dim == 3){
//        k = (float) 8 / PI;
//    }
//
//    k /= pow(h, dim);
//    float q = r_norm / h;
//
//    float q2, q3;
//    if(q <= 1.0){
//        if(q <= 0.5){
//            q2 = q * q;
//            q3 = q2 * q;
//            res = k * (6.0 * q3 - 6.0 * q2 + 1);
//        }else{
//            res = k * 2 * pow(1 - q, 3.0);
//        }
//    }
//    return res;
//}
//
//float HinaPE::SPHKernel::cubic_kernel_derivative(float r_norm) {
//    float res = 0.0;
//
//    float particle_radius = 0.01;
//    float h = 4.0 * particle_radius;
//    float k = 1.0;
//
//    /// dim domain bounding box
//    float dim;
//
//    if(dim == 1){
//        k = (float) 4 / 3;
//    }else if(dim == 2){
//        k = (float) 40 / 7 / PI;
//    }else if(dim == 3){
//        k = (float) 8 / PI;
//    }
//
//    /// ?
//    k = pow(6. * k / h, dim);
//
//    float q = r_norm / h;
//
//    float q2, q3;
//    if(q <= 1.0){
//        if(q <= 0.5){
//            q2 = q * q;
//            q3 = q2 * q;
//            res = k * (6.0 * q3 - 6.0 * q2 + 1);
//        }else{
//            res = k * 2 * pow(1 - q, 3.0);
//        }
//    }
//    return res;
//}












