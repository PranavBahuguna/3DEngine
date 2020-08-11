ORBIT_DISTANCE = 12.0
ORBIT_SPEED = 25.0
ANGLE = -90.0

function init()
  setPos(vec3.new(0.0, 0.0, ORBIT_DISTANCE))
	setEuler(vec3.new(0.0, 0.0, -30.0))
	setScale(vec3.new(0.002))
end

function update()
	local deltaAngle = ORBIT_SPEED * getDeltaTime()
	rotate(vec3.new(0.0, deltaAngle, 0.0))

	ANGLE = ANGLE + deltaAngle
	if (ANGLE >= 270) then
		ANGLE = ANGLE - 360
	end
	local x = ORBIT_DISTANCE * sin(ANGLE)
	local z = ORBIT_DISTANCE * cos(ANGLE)
	setPos(vec3.new(x, 0.0, z))
end