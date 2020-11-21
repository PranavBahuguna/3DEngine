ROTATION_SPEED = 60.0

function init()
    getTransform():setPosition(vec3.new(0.0, 0.0, 3.0))
    getTransform():setScale(vec3.new(0.4))
end

function update()
    local angle = deg2rad(ROTATION_SPEED) * getDeltaTime()
    getTransform():rotate(vec3.new(angle, angle, angle))
end
