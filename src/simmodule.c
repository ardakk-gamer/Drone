#include "simmodule.h"

void fizik_init(DronePhysics* fizik, float mass, float gravity) {
    fizik->mass = mass;
    fizik->gravity = gravity;
    fizik->state.position = (Vector3){0, 0, 0};
    fizik->state.velocity = (Vector3){0, 0, 0};
}

void fizik_update(DronePhysics* fizik, float thrust, float dt) {
    if (dt <= 0.0f) return;
    
    float upwardAcceleration = (thrust / fizik->mass) - fizik->gravity;
    fizik->state.velocity.z += upwardAcceleration * dt;
    fizik->state.position.z += fizik->state.velocity.z * dt;

    if (fizik->state.position.z < 0.0f) {
        fizik->state.position.z = 0.0f;
        fizik->state.velocity.z = 0.0f;
    }
}

DroneState fizik_get_state(const DronePhysics* fizik) {
    return fizik->state;
}