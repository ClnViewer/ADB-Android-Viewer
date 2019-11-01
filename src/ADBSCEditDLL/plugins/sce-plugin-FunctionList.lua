local plugin = {}
    --
    plugin.name = "FunctionList"
    plugin.title = "List function in code used"
    plugin.type = 1
    --
    plugin.savename = nil
    plugin.outtbl={}
    --
    --  DON'T MODIFY!
    local ENV = {}
    setmetatable(ENV, {
        __index = _ENV
    })
    --
    local tblseen={}
    --
    local tblclass =
    {
	"LuaObject",
	"plugin",
	"plugins",
	"plugins.sce-plugin-FunctionList",
	"math",
	"io",
	"os",
	"string",
	"table",
	"debug",
	"coroutine",
	"bit32",
	"utf8",
	"package",
	"searchers"
    }
    local tblfunction =
    {
	"f",
	"assert",
	"collectgarbage",
	"dofile",
	"dprint",
	"error",
	"getmetatable",
	"ipairs",
	"load",
	"loadfile",
	"next",
	"pairs",
	"pcall",
	"print",
	"rawequal",
	"rawget",
	"rawlen",
	"rawset",
	"require",
	"select",
	"setmetatable",
	"tonumber",
	"tostring",
	"type",
	"xpcall"
    }
    --
    --
    function plugin.functionSign(f, n)
        assert(type(f) == 'function', "bad argument #1 to 'functionSign' (function expected)")
        local p = {}
        pcall(
           function()
              local oldhook
              local delay = 2
              local function hook(event, line)
              delay = delay - 1
              if delay == 0 then
                  for i = 1, math.huge do
                      local k, v = debug.getlocal(2, i)
                      if type(v) == "table" then
                          table.insert(p, "...")
                          break
                      elseif (k or '('):sub(1, 1) == '(' then
                          break
                      else
                          table.insert(p, k)
                      end
                  end
                  if debug.getlocal(2, -1) then
                      table.insert(p, "...")
                  end
                  debug.sethook(oldhook)
                  error("'functionSign' (function aborting the call)")
              end
           end
           oldhook = debug.sethook(hook, "c")
           local arg = {}
           for j = 1, 64 do arg[#arg + 1] = true end
           f((table.unpack or unpack)(arg))
        end)
	if n ~= nil then
	    return n.."("..table.concat(p, ",")..")"
	else
	    return "?("..table.concat(p, ",")..")"
	end
    end
    --
    --
    function plugin.functionList(t, n)
        tblseen[t] = true
        local s = {}
        for k in pairs(t) do
            s[#s + 1] = k
        end
        table.sort(s)
        for k,v in ipairs(s) do
            local f = t[v]
            local x = tostring(v)
            if type(f) == "table" and not tblseen[f] and not plugin.stopWord(x, tblclass) then
                plugin.functionList(f, (x .. "."))
            elseif type(f) == "function" and not plugin.stopWord(x, tblfunction) then
		plugin.outtbl[#plugin.outtbl + 1] = "" .. n .. plugin.functionSign(f, v) .. ""
            end
        end
    end
    --
    function plugin.stopWord(n, t)
        for k in pairs(t) do
            if t[k] == n then
                return true
            end
        end
        return false
    end
    --
    --
    function plugin.init(p,d,s)
	ENV._G.print  = p
	ENV._G.dprint = d
        plugin.savename = s
    end
    --
    function plugin.callplugin()
        tblseen = {}
        plugin.outtbl = {}
	plugin.functionList(ENV._G, "")
        table.sort(plugin.outtbl)
	return plugin.outtbl
    end
    --
    function plugin.debugplugin()
        local o = plugin.callplugin()
        for k in pairs(o) do
            print(k, " : ", o[k])
        end
	return o
    end

return plugin
