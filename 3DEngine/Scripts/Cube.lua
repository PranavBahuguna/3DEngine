ROTATION_SPEED = 60.0

function init()
  setPos(vec3.new(-3.0, 0.0, 6.0))
  setEuler(vec3.new(1.0, 1.0, 1.0))
	setScale(vec3.new(0.4))
end

function update()
	setAngle(getAngle() + ROTATION_SPEED * getDeltaTime())
end