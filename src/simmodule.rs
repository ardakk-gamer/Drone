#[derive(Debug, Clone, Copy)]
pub struct Vector3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

#[derive(Debug, Clone, Copy)]
pub struct DroneState {
    pub position: Vector3,
    pub velocity: Vector3,
}

pub struct DronePhysics {
    pub state: DroneState,
    pub mass: f32,
    pub gravity: f32,
}

impl DronePhysics {
    pub fn new(mass: f32, gravity: f32) -> Self {
        Self {
            state: DroneState {
                position: Vector3 { x: 0.0, y: 0.0, z: 0.0 },
                velocity: Vector3 { x: 0.0, y: 0.0, z: 0.0 },
            },
            mass,
            gravity,
        }
    }

    pub fn update(&mut self, thrust: f32, dt: f32) {
        if dt <= 0.0 {
            return;
        }
        let upward_acceleration = (thrust / self.mass) - self.gravity;
        self.state.velocity.z += upward_acceleration * dt;
        self.state.position.z += self.state.velocity.z * dt;

        if self.state.position.z < 0.0 {
            self.state.position.z = 0.0;
            self.state.velocity.z = 0.0;
        }
    }
}