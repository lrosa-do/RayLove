local angle =0
 function love.load()


  end

  function love.keypressed(key)
    if key == "Escape" then
      os.exit()
    end
  end

-- Simple function for translating on-screen coordinates to in-game coordinates.
local function screenToGame(xOnScreen, yOnScreen)
	local xInGame = xOnScreen
	local yInGame = yOnScreen

	return xInGame, yInGame
end

function love.draw()
	local screenWidth, screenHeight = love.graphics.getDimensions()


love.graphics.push()
	-- Transform the coordinate system so the top left in-game corner is in
	-- the bottom left corner of the screen.
    love.graphics.translate(0, screenHeight/2)
	love.graphics.rotate(-math.pi/2)

	-- Draw some text in the (translated) top left corner.
	love.graphics.setColor(255, 255, 255)
	love.graphics.print("Lorem ipsum.", 5, 5)

	-- Draw a rectangle that lights up when hovered.
	local rectX      = 150
	local rectY      = 250
	local rectWidth  = 180
	local rectHeight = 110

	local mouseX, mouseY = love.mouse.getPosition()     -- On-screen.
	--mouseX, mouseY       = screenToGame(mouseX, mouseY) -- In-game.

	local isHovered = (mouseX >= rectX and mouseY >= rectY and mouseX < rectX+rectWidth and mouseY < rectY+rectHeight)
	--print(isHovered,mouseX, mouseY)

	if isHovered then
		love.graphics.setColor(255, 255, 255)
	else
		love.graphics.setColor(255, 0, 0)
	end
	love.graphics.rectangle("fill", rectX, rectY, rectWidth, rectHeight)
love.graphics.pop()
end


