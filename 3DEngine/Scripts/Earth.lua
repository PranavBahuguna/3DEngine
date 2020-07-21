function init()
end

function update()
	local angle = getAngle()
	angle = angle + 45.0 * getDeltaTime()
	if (angle >= 360.0) then
	  angle = 0.0
	end
	setAngle(angle)
end