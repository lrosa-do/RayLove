function love.load()
    target = {}
    target.x = 300
    target.y = 300
    target.radius = 50

    score = 0
    timer = 3

    gameFont = love.graphics.newFont(40)
    
    
    sprites = {}

    sprites.crosshairs = love.graphics.newImage('assets/crosshairs.png')
    sprites.target = love.graphics.newImage('assets/target.png')
    

    
   love.mouse.setVisible(false)
end

function love.update(dt)
    if timer > 0 then
        timer = timer - dt
    end

    if timer < 0 then
        timer = 0
    end
end

function love.draw()
   
    love.graphics.setColor(255, 255, 255)
    
    love.graphics.draw(sprites.target, target.x - target.radius, target.y - target.radius)
    love.graphics.draw(sprites.crosshairs, love.mouse.getX()-20, love.mouse.getY()-20)
    
    love.graphics.setFont(gameFont)
    love.graphics.print(score, 0, 0)
    love.graphics.print(math.ceil(timer), 300, 0)

    
end

function love.mousepressed( x, y, button, istouch, presses )
    if button == 1 then
        local mouseToTarget = distanceBetween(x, y, target.x, target.y)
        if mouseToTarget < target.radius then
            score = score + 1
            target.x = math.random(target.radius, love.graphics.getWidth() - target.radius)
            target.y = math.random(target.radius, love.graphics.getHeight() - target.radius)
        end
    end
end

function distanceBetween(x1, y1, x2, y2)
    return math.sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
end

function love.update(dt)

end

