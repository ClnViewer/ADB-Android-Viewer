local plugin = {}
    --
    plugin.name = "SaveAsByteCodeLuac"
    plugin.title = "Save as binary byte code .luac"
    plugin.type = 2
    plugin.savename = nil
    --
    -- 'LUACPATH' is the lua compile path, include executable name, required!
    local LUACPATH   = "luac53.exe"
    --
    --  DON'T MODIFY!
    local ENV = {}
    setmetatable(ENV, {
        __index = _ENV
    })
    --
    local function save(str)
        if str == nil then
            return {}
        end
        assert(plugin.savename ~= nil, "Save name and path not set (path expected)")
        local fni = "." .. os.tmpname() .. ".lua"
        local fno = plugin.savename:match("(.+)%..+") .. ".luac"
        local f = assert(io.open(fni, "w"))
        if f == nil then
            return {}
        end
        f:write(str)
        f:close()
        local list = {}
        local po = assert(io.popen(LUACPATH .. " -v"))
        for k in po:lines() do
                list[#list + 1] = k
        end
        po = assert(io.popen(LUACPATH .. " -s -o " .. fno .. " " .. fni))
        for k in po:lines() do
                list[#list + 1] = k
        end
        os.remove(fni)
        list[#list + 1] = "Save as: " .. fno .. ""
        return list
    end
    --
    --
    function plugin.init(p,d,s)
	ENV._G.print = p
	ENV._G.dprint = d
        plugin.savename = s
    end

    function plugin.callplugin(str)
	local o = save(str)
	return o
    end

    function plugin.debugplugin(str)
	local o = plugin.callplugin(str)
        for k in pairs(o) do
            print(o[k])
        end
	return o
    end

return plugin
