local lume = rawget(_G, "lume") or require((...):gsub("[^/.\\]+$", "lume"))

local lurker = { _version = "1.0.1" }


local dir = love.filesystem.enumerate or love.filesystem.getDirectoryItems
local time = love.timer.getTime or os.time

local function isdir(path)
    local info = love.filesystem.getInfo(path)
    return info.type == "directory"
end

local function lastmodified(path)
    local info = love.filesystem.getInfo(path, "file")
    return info.modtime
end

local lovecallbacknames = {
  "update",
  "load",
  "draw",
  "mousepressed",
  "mousereleased",
  "keypressed",
  "keyreleased",
  "focus",
  "quit",
}


function lurker.init()
  lurker.print("Initing lurker")
  lurker.path = "/media/djoker/code/linux/c/testelua/bin/Debug"
  lurker.preswap = function() end
  lurker.postswap = function() end
  lurker.interval = 1.5
  lurker.protected = true
  lurker.quiet = false
  lurker.lastscan = 0
  lurker.lasterrorfile = nil
  lurker.files = {}
  lurker.funcwrappers = {}
  lurker.lovefuncs = {}
  lurker.state = "init"
  lume.each(lurker.getchanged(), lurker.resetfile)
  return lurker
end


function lurker.print(...)
  print("[lurker] " .. lume.format(...))
end


function lurker.listdir(path, recursive, skipdotfiles)
  path = (path == ".") and "" or path
  local function fullpath(x) return path .. "/" .. x end

--  path ="/media/djoker/code/linux/c/testelua/bin/Debug"
  local t = {}
  for _, f in pairs(lume.map(dir(path), fullpath)) do
    if not skipdotfiles or not f:match("/%.[^/]*$") then
      if recursive and isdir(f) then
        t = lume.concat(t, lurker.listdir(f, true, true))
      else
       -- print(lume.trim(f, "/"))
        table.insert(t, lume.trim(f, "/"))
      end
    end
  end
  return t
end


function lurker.initwrappers()
  for _, v in pairs(lovecallbacknames) do
    lurker.funcwrappers[v] = function(...)
      local args = {...}
      xpcall(function()
        return lurker.lovefuncs[v] and lurker.lovefuncs[v](unpack(args))
      end, lurker.onerror)
    end
    lurker.lovefuncs[v] = love[v]
  end
  lurker.updatewrappers()
end


function lurker.updatewrappers()
  for _, v in pairs(lovecallbacknames) do
    if love[v] ~= lurker.funcwrappers[v] then
      lurker.lovefuncs[v] = love[v]
      love[v] = lurker.funcwrappers[v]
    end
  end
end


function lurker.onerror(e, nostacktrace)
  lurker.print("An error occurred; switching to error state")
  lurker.state = "error"

  -- Release mouse
  local setgrab = love.mouse.setGrab or love.mouse.setGrabbed
  setgrab(false)

  -- Set up callbacks
  for _, v in pairs(lovecallbacknames) do
    love[v] = function() end
  end

  love.update = lurker.update

  love.keypressed = function(k)
    if k == "escape" then
      lurker.print("Exiting...")
      love.event.quit()
    end
  end

  local stacktrace = nostacktrace and "" or
                     lume.trim((debug.traceback("", 2):gsub("\t", "")))
  local msg = lume.format("{1}\n\n{2}", {e, stacktrace})

  end
end


function lurker.exitinitstate()
  lurker.state = "normal"
  if lurker.protected then
    lurker.initwrappers()
  end
end


function lurker.exiterrorstate()
  lurker.state = "normal"
  for _, v in pairs(lovecallbacknames) do
    love[v] = lurker.funcwrappers[v]
  end
end


function lurker.update()
  if lurker.state == "init" then
    lurker.exitinitstate()
  end
  local diff = time() - lurker.lastscan
  if diff > lurker.interval then
    lurker.lastscan = lurker.lastscan + diff
      local changed = lurker.scan()
      if lurker.lasterrorfile then
          local f = lurker.lasterrorfile
          lurker.lasterrorfile = nil
         lurker.hotswapfile(f)
    end
  end
end


function lurker.getchanged()
  local function fn(f)
   print(f)
    return f:match("%.lua$") and lurker.files[f] ~= lastmodified(f)
  end
  return lume.filter(lurker.listdir(lurker.path, true, true), fn)
end


function lurker.modname(f)
  return (f:gsub("%.lua$", ""):gsub("[/\\]", "."))
end


function lurker.resetfile(f)
  lurker.files[f] = lastmodified(f)
end


function lurker.hotswapfile(f)
 print("swap")
  lurker.print("Hotswapping '{1}'...", {f})
  if lurker.state == "error" then
    lurker.exiterrorstate()
  end
  if lurker.preswap(f) then
    lurker.print("Hotswap of '{1}' aborted by preswap", {f})
    lurker.resetfile(f)
    return
  end
  local modname = lurker.modname(f)
  local t, ok, err = lume.time(lume.hotswap, modname)
  if ok then
    lurker.print("Swapped '{1}' in {2} secs", {f, t})
  else
    lurker.print("Failed to swap '{1}' : {2}", {f, err})
    if not lurker.quiet and lurker.protected then
      lurker.lasterrorfile = f
      lurker.onerror(err, true)
      lurker.resetfile(f)
      return
    end
  end
  lurker.resetfile(f)
  lurker.postswap(f)
  if lurker.protected then
    lurker.updatewrappers()
  end
end


function lurker.scan()
  if lurker.state == "init" then
    lurker.exitinitstate()
  end
  local changed = lurker.getchanged()
  print(changed)
for key, value in pairs(changed) do
    print(key, " -- ", value)
end   
--  lume.each(changed, lurker.hotswapfile)
  return changed

end


return lurker.init()
