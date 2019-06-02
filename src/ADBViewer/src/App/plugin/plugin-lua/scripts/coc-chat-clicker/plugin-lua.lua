
require "plugins.plugin-lua-coc-data"
require "plugins.plugin-lua-coc-speech"

local function tolog(s, r)
  io.write("\t+ stage ( "..string.format("%s", s).." ) ret ( "..string.format("%s", r).." )\n")
end

local function isreload()
  local ret
  io.write("-- begin isreload ( 000 )\n")
  
  ret = LuaObject:checkPixelsByPos(tbl.restartscreen1)
  if ret then
    LuaObject:adbClick(460,426)
    io.write("-- begin restartscreen2 ( 001 )\n")
    return 8
  end

  ret = LuaObject:checkPixelsByPos(tbl.restartscreen2)
  if ret then
    LuaObject:adbClick(460,426)
    io.write("-- begin restartscreen2 ( 002 )\n")
    return 8
  end

  ret = LuaObject:checkPixelsByPos(tbl.restartscreen3)
  if ret then
    LuaObject:adbClick(460,422)
    io.write("-- begin restartscreen3 ( 003 )\n")
    return 8
  end

  ret = LuaObject:checkPixelsByPos(tbl.loadlogo)
  if ret then
    io.write("-- begin loadlogo ( 004 )\n")
    return 5
  end

  ret = LuaObject:checkPixelsByPos(tbl.defscreen)
  if ret then
    LuaObject:adbClick(640,594)
    io.write("-- begin defscreen ( 005 )\n")
    return 5
  end

  io.write("-- begin isreload ( 0000 )\n")
  return 0
end

local function step(state)
  
  local ret
  local sleep = 5
  local oldstate = state
  
  if state == 0 then
    -- chat menu close
    ret = LuaObject:checkPixelsByPos(tbl.menuclose)
    tolog(state, ret)
    if ret then
      state = 1
      LuaObject:adbClick(18,414)
    end
    
  elseif state == 1 then
    -- chat menu switch to global area
    ret = LuaObject:checkPixelsByPos(tbl.menuglobal)
    tolog(state, ret)
    if ret then
      state = 2
      LuaObject:adbClick(94,28)
    end
    
  elseif state == 2 then
    -- chat menu is warning button global area (press)
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalwarnbutton)
    tolog(state, ret)
    state = 3
    if ret then
      LuaObject:adbClick(198,576)
    end
    
  elseif state == 3 then
    -- chat menu open message button (press)
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalchatbutton)
    tolog(state, ret)
    if ret then
      state = 4
      sleep = 7
      LuaObject:adbClick(344,764)
    end
    
  elseif state == 4 then
    -- opened keyboard check
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalchatkbd)
    tolog(state, ret)
    if ret then
      math.randomseed(os.time())
      local stxt = messages[math.random(#messages)]
      if not stxt then
        state = 5
        sleep = 1
      else
        io.write("\t+ text ( "..stxt.." )\n")
        state = 6
        sleep = 3
        LuaObject:adbText(stxt)
      end
    end
    
  elseif state == 5 then
    -- keyboard close (cancel)
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalchatkbd)
    tolog(state, ret)
    if ret then
      state = 7
      sleep = 10
      LuaObject:adbClick(474,772)
    end
  
  elseif state == 6 then
    -- keyboard enter (continue)
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalchatkbd)
    tolog(state, ret)
    if ret then
      state = 7
      sleep = 90
      LuaObject:adbClick(1188,538)
    end
    
  elseif state == 7 then
    -- keyboard enter (close)
    ret = LuaObject:checkPixelsByPos(tbl.menuglobalchatkbd)
    tolog(state, ret)
    if ret then
      state = 3
      sleep = 10
      LuaObject:adbKey(111)
    else
      -- end, close chat menu (for test only)
      ret = LuaObject:checkPixelsByPos(tbl.menuopen)
      tolog(state, ret)
      if ret then
        state = 0
        LuaObject:adbClick(410,414)
      end
    end
  end
  
  io.write("-- states ( "..string.format("%s", oldstate).."/"..string.format("%s", state).." )\n")
  
  if state == oldstate then
    if state > 0 and state < 7  then
      state = state + 1
    elseif state == 7 then
      state = 0
    end
    sleep = 1
  end
  
  LuaObject:stateSet(state)
  LuaObject:stateSleep(sleep)
  
  io.write("-- end step ( "..string.format("%s", state).." )\n")
  
  return state;
end

function main(state)
  
  -- local f = io.open("..\\bin\\__dump__coc_lua__", "a")
  local f = io.open("plugins\\__dump__coc_lua__", "a")
  io.output(f)
  io.write("-- begin main ( "..string.format("%s", state).." )\n")
  
  local newsleep = isreload()
  if newsleep > 0 then
    LuaObject:stateSleep(newsleep)
    io.write("-- newsleep.. ( "..string.format("%s", newsleep).." )\n")
    return
  end
  local newstate = step(state)
  io.write("-- end main ( "..string.format("%s", newstate).." )\n")
  io.close(f)
  return newstate
end

