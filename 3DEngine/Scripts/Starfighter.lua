ORBIT_DISTANCE = 12.0
ORBIT_SPEED = 25.0
ANGLE = -90.0

function init()
  setPos(vec3.new(0.0, 0.0, ORBIT_DISTANCE))
	setScale(vec3.new(0.002))
end

function update()
	ANGLE = ANGLE + ORBIT_SPEED * getDeltaTime()
	setEuler(vec3.new(0.0, ANGLE, -30.0))

	rotAngle = ANGLE - 90.0
	local x = ORBIT_DISTANCE * sin(rotAngle)
	local z = ORBIT_DISTANCE * cos(rotAngle)
	setPos(vec3.new(x, 0.0, z))
end