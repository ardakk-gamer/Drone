package simmodule

type Vector3 struct {
	X, Y, Z float32
}

type DroneState struct {
	Position Vector3
	Velocity Vector3
}

type DronePhysics struct {
	State   DroneState
	Mass    float32
	Gravity float32
}

func (dp *DronePhysics) Init(mass, gravity float32) {
	dp.Mass = mass
	dp.Gravity = gravity
	dp.State = DroneState{}
}

func (dp *DronePhysics) Update(thrust float32, dt float32) {
	if dt <= 0 {
		return
	}
	upwardAcceleration := (thrust / dp.Mass) - dp.Gravity
	dp.State.Velocity.Z += upwardAcceleration * dt
	dp.State.Position.Z += dp.State.Velocity.Z * dt

	if dp.State.Position.Z < 0 {
		dp.State.Position.Z = 0
		dp.State.Velocity.Z = 0
	}
}