#ifndef SIMMODULE_H
#define SIMMODULE_H

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    Vector3 position;
    Vector3 velocity;
} DroneState;

typedef struct {
    DroneState state;
    float mass;
    float gravity;
} DronePhysics;

void fizik_init(DronePhysics* fizik, float mass, float gravity);
void fizik_update(DronePhysics* fizik, float thrust, float dt);
DroneState fizik_get_state(const DronePhysics* fizik);

#endif