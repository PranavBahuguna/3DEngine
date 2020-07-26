ORBIT_DISTANCE = 12.0
ORBIT_SPEED = 25.0

function init()
  setPos(vec3.new(0.0, 0.0, ORBIT_DISTANCE))
  setEuler(vec3.new(0.0, 1.0, 0.0))
	setScale(vec3.new(0.002))
end

function update()
	local angle = getAngle()
	angle = angle + ORBIT_SPEED * getDeltaTime()
	setAngle(angle)

	angle = angle - 90.0
	local x = ORBIT_DISTANCE * sin(angle)
	local z = ORBIT_DISTANCE * cos(angle)
	setPos(vec3.new(x, 0.0, z))
end