-- lick.lua
--
-- simple LIVECODING environment for Löve
-- overwrites love.run, pressing all errors to the terminal/console

local lick = {}
lick.file = "main.lua"
lick.debug = false
lick.reset = false
lick.clearFlag = false
lick.sleepTime =  0.001

local last_modified = 0

local function handle(err)
  return "ERROR: " .. err
end

-- Initialization
local function load()
  last_modified = 0
end

local function update(dt)
print("update")
    local info = love.filesystem.getInfo(lick.file)
    if info and last_modified < info.modtime then
        last_modified = info.modtime
        success, chunk = pcall(love.filesystem.load, lick.file)
        if not success then
            print(tostring(chunk))
            lick.debugoutput = chunk .. "\n"
        end
        ok,err = xpcall(chunk, handle)

        if not ok then 
            print(tostring(err))
            if lick.debugoutput then
                lick.debugoutput = (lick.debugoutput .."ERROR: ".. err .. "\n" )
            else 
                lick.debugoutput =  err .. "\n" 
            end 
        else
            print("CHUNK LOADED\n")
            lick.debugoutput = nil
        end
        if lick.reset then
            loadok, err = xpcall(love.load, handle)
            if not loadok and not loadok_old then
                print("ERROR: "..tostring(err))
                if lick.debugoutput then
                    lick.debugoutput = (lick.debugoutput .."ERROR: ".. err .. "\n" ) 
                else
                    lick.debugoutput =  err .. "\n"
                end
                loadok_old = not loadok
            end
        end
    end

    updateok, err = pcall(love.update,dt)
    if not updateok and not updateok_old then 
        print("ERROR: "..tostring(err))
        if lick.debugoutput then
            lick.debugoutput = (lick.debugoutput .."ERROR: ".. err .. "\n" ) 
        else
            lick.debugoutput =  err .. "\n"
        end
  end
  
  updateok_old = not updateok
end

local function draw()
print("lick draw")
    drawok, err = xpcall(love.draw, handle)
    if not drawok and not drawok_old then 
        print(tostring(err))
        if lick.debugoutput then
            lick.debugoutput = (lick.debugoutput .. err .. "\n" ) 
        else
            lick.debugoutput =  err .. "\n"
        end 
    end

    if lick.debug and lick.debugoutput then 
        love.graphics.setColor(1,1,1,0.8)
        love.graphics.printf(lick.debugoutput, (love.graphics.getWidth()/2)+50, 0, 400, "right")
    end
    drawok_old = not drawok
end

local function Run()
   


    
end


return lick
