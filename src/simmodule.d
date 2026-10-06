module simmodule;

import std.algorithm : clamp;

struct Vector3 {
    float x, y, z;
}

struct DroneState {
    Vector3 position;
    Vector3 velocity;
}

struct DronePhysics {
    DroneState state;
    float mass;
    float gravity;

    void initialize(float m, float g) {
        mass = m;
        gravity = g;
        state = DroneState(Vector3(0,0,0), Vector3(0,0,0));
    }

    void update(float thrust, float dt) {
        if (dt <= 0.0f) return;
        float upwardAcceleration = (thrust / mass) - gravity;
        state.velocity.z += upwardAcceleration * dt;
        state.position.z += state.velocity.z * dt;

        if (state.position.z < 0.0f) {
            state.position.z = 0.0f;
            state.velocity.z = 0.0f;
        }
    }
}