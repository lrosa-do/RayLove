local angle =0
 function love.load()


  end

  function love.keypressed(key)
    if key == "Escape" then
      os.exit()
    end
  end

  local function drawText(str, y)
  
  end

function drawRotatedRectangle(mode, x, y, width, height, angle)
	local cosa, sina = math.cos(angle), math.sin(angle)

	local dx1, dy1 = width*cosa,   width*sina
	local dx2, dy2 = -height*sina, height*cosa

	local px1, py1 = x,         y
	local px2, py2 = x+dx1,     y+dy1
	local px3, py3 = x+dx1+dx2, y+dy1+dy2
	local px4, py4 = x+dx2,     y+dy2
	
    love.graphics.polygon(mode, px1,py1, px2,py2, px3,py3, px4,py4)
end
  function love.draw()

   angle = angle +1 

    love.graphics.setColor(255, 255, 255)    
    love.graphics.print("raylib",100,100)
    love.graphics.Fps(10,10)    

 local vertices  = {100,100, 200,100, 200,200, 300,200, 300,300, 100,300} 
  love.graphics.bezier(4,vertices)
  
  drawRotatedRectangle('line',200,200,100,100,90)
  
  love.graphics.pie('fill',250,200,100,-angle,angle)
  
    love.graphics.pie('line',250,300,100,-angle,angle)
    
    love.graphics.ellipse('line',350,100,10,20)
    love.graphics.ellipse('fill',390,100,10,20)
    
        love.graphics.arc(100,100,90,angle,360)
        
        love.graphics.rounded('line',300,100,350,120,2,2)
        
        
    
    
  

  end

