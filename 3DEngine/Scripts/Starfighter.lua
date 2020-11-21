ORBIT_DISTANCE = 12.0
ORBIT_SPEED = 25.0
ANGLE = -90.0

function init()
    getTransform():setPosition(vec3.new(0.0, 0.0, ORBIT_DISTANCE)):setRotation(
        vec3.new(0.0, 0.0, deg2rad(-30.0))):setScale(vec3.new(0.002))
end

function update()
    local deltaAngle = ORBIT_SPEED * getDeltaTime()
    getTransform():rotate(vec3.new(0.0, deg2rad(deltaAngle), 0.0))

    ANGLE = ANGLE + deltaAngle
    if (ANGLE >= 270) then ANGLE = ANGLE - 360 end
    local x = ORBIT_DISTANCE * sin(ANGLE)
    local z = ORBIT_DISTANCE * cos(ANGLE)
    getTransform():setPosition(vec3.new(x, 0.0, z))
end
