function love.load()
   sprites = {}

    sprites.player = love.graphics.newImage('assets/spawner.png')


    player = {}
    player.x = love.graphics.getWidth() / 2
    player.y = love.graphics.getHeight() / 2
    player.quad=love.graphics.newQuad(0,0,24,24)
    player.speed = 3
    player.angle =0
    
   love.mouse.setVisible(false)
end

function love.update(dt)

--player.angle =     player.angle + (80 * dt)

 if love.keyboard.isDown("d") then
        player.x = player.x + player.speed
        
    end
    if love.keyboard.isDown("A") then
        player.x = player.x - player.speed
       
    end
    if love.keyboard.isDown("w") then
        player.y = player.y - player.speed
    end
    if love.keyboard.isDown("s") then
        player.y = player.y + player.speed
    end
  
end

function love.draw()

    love.graphics.Fps(5,5)   
    love.graphics.setColor(255, 255, 255)
    

     love.graphics.draw(sprites.player,    player.quad, player.x, player.y   ,false,    player.angle,2.5,2.5)
     love.graphics.draw(sprites.player,    player.quad, player.x-70, player.y,true,    player.angle,2.5,2.5)
    
end

function love.mousepressed( x, y, button,  presses )
   
end
function love.keypressed( key )
  if (key =="Escape") then
    love.event.quit()
  end
end

