local plugin = {}
    --
    plugin.name = "ByteCodeLuac"
    plugin.title = "Byte Code instruction list"
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
    local UNKVAL     = "???"
    local INDENT10   = "    | "
    local INDENT100  = "  | "
    local INDENT1000 = "| "
    local HEADERSTR  = "IN  | L    |  INSTRUCTION CODE"
    local sectiondb  = {}
    --
    local function parse(str)
        local fn = "." .. os.tmpname() .. ".lua"
        local f = assert(io.open(fn, "w"))
        if f == nil then
            return {}
        end
        f:write(str)
        f:close()
        local list = {}
        local pipe = assert(io.popen(LUACPATH .. " -l -p " .. fn))
        for line in pipe:lines() do
                list[#list + 1] = line
        end
        os.remove(fn)
        return list
    end
    --
    local function split(str)
	local place, cmt, val
        local idx, line, code, A, B, C, tmp = 
            string.match(str, "%s+(%d+)%s+%[(%d+)%]%s+(%a+)%s+([-%d]+)%s+([-%d]+)%s*([-%d]*)%s*;?%s*(.*)")
        if not idx then
            cmt = string.match(str, "%s*at%s+([abcdef1234567890]+)")
            if not cmt then
                return nil
            end
            idx  = "0"
            line = "0"
            code = "SECTION"
        end
        if tmp ~= nil then
        	if code == "SETTABUP" then
			place, cmt, val = string.match(tmp, "%s*([^%s]+)%s+([^%s]+)%s+([^%s]+)")
			if place == nil and cmt == nil and val == nil then
				place, cmt = string.match(tmp, "%s*([^%s]+)%s+([^%s]+)")
			end
	        elseif code == "GETTABUP" then
			place, cmt = string.match(tmp, "%s*([^%s]+)%s+([^%s]+)")
        	elseif code == "CLOSURE" or code == "LOADK" or code == "JMP" then
			cmt = tmp
	        elseif code == "LT" or code == "EQ" then
			cmt, val = string.match(tmp, "%s*([^%s]+)%s+([^%s]+)")
	        end
        end
        if cmt ~= nil then
		cmt = cmt:gsub("\"", "")
        end
        return {
            idx  = idx,
            line = line,
            code = code,
            A = A,
            B = B,
            C = C,
            place = place,
            comment = cmt,
            value = val
        }
    end
    --
    local function getIndent(o)
        local s = ""
        local i = tonumber(o.idx)
        local l = tonumber(o.line)
        if i < 10 then
            s = o.idx .. INDENT10
        elseif i < 100 then
            s = o.idx .. INDENT100
        else
            s = o.idx .. INDENT1000
        end
        if l < 10 then
            s = s .. o.line .. INDENT10
        elseif l < 100 then
            s = s .. o.line .. INDENT100
        else
            s = s .. o.line .. INDENT100
        end
        return s
    end
    --
    local function getSECTION(o)
        local s = nil
        if o.comment == nil then
            return ""
        end
        for k in pairs(sectiondb) do
            if sectiondb[k][2] == o.comment then
                s = "[" .. sectiondb[k][1] .. "]"
            end
        end
        if s ~= nil then
            return string.format("Function %s '%s' section", s, o.comment)
        else
            return string.format("Address section '%s' [script body]", o.comment)
        end
    end
    --
    local function getSETTABUP(o, cl)
        local s = ""
        if o.place ~= "_ENV" and o.comment ~= nil then
            s = "(" .. o.place .. ") -> [" .. o.comment .. "]"
        elseif o.place == "_ENV" and o.comment ~= nil then
            s = "[" .. o.comment .. "]"
        elseif o.comment ~= nil then
            s = "[" .. o.comment .. "]"
        elseif o.place ~= nil then
            s = "(" .. o.place .. ")"
        end
        if o.value ~= nil then
            s = s .. " = " .. o.value
        end
        if cl ~= nil then
            s = s .. " -> (" .. cl .. ")"
            sectiondb[#sectiondb + 1] = { o.comment, cl }
        end
        return s
    end
    --
    local function getGETTABUP(o)
        local s = ""
        if o.place ~= "_ENV" and o.comment ~= nil then
            s = "(" .. o.place .. ") -> [" .. o.comment .. "]"
        elseif o.place == "_ENV" and o.comment ~= nil then
            s = "[" .. o.comment .. "]"
        elseif o.comment ~= nil then
            s = "[" .. o.comment .. "]"
        elseif o.place ~= nil then
            s = "(" .. o.place .. ")"
        end
        return s
    end
    --
    local function getLT(o)
        local s = "IF ("
        local val1
        local val2
        if o.comment ~= nil then
            val1 = o.comment
        else
            val1 = UNKVAL
        end
        if o.value ~= nil then
            val2 = o.value
        else
            val2 = UNKVAL
        end
        if o.B ~= nil and o.C ~= nil then
            if o.B > o.C then
                s = s .. val2 .. " > " .. val1
            else
                s = s .. val1 .. " < " .. val2
            end
        else
            s = s .. val1 .. UNKVAL .. val2
        end
        s = s .. ")"
        return s
    end
    --
    local function getEQ(o)
        local s = "IF ("
        local val1
        local val2
        if o.comment ~= nil then
            val1 = o.comment
        else
            val1 = UNKVAL
        end
        if o.value ~= nil then
            val2 = o.value
        else
            val2 = UNKVAL
        end
        if o.B ~= nil and o.C ~= nil then
            if o.B == o.C then
                s = s .. val2 .. " == " .. val1
            else
                s = s .. val1 .. " != " .. val2
            end
        else
            s = s .. val1 .. UNKVAL .. val2
        end
        s = s .. ")"
        return s
    end
    --
    local function run(str)
        sectiondb = {}
	if str == nil then
	    return {}
	end
        local d = parse(str)
        local o = {}
	local closure = nil
        for k in pairs(d) do
	    --print(d[k])
            local info = split(d[k])
            if info then
		--print(i, info.code)
		--printobj(info)
		if info.code == "SETTABUP" then
			o[#o + 1] = string.format("%s SET %s", getIndent(info), getSETTABUP(info, closure))
			closure = nil
		elseif info.code == "GETTABUP" then
			o[#o + 1] = string.format("%s GET %s", getIndent(info), getGETTABUP(info))
		elseif info.code == "LOADK" then
			o[#o + 1] = string.format("%s LOAD \"%s\"", getIndent(info), info.comment)
		elseif info.code == "CALL" then
			o[#o + 1] = string.format("%s CALL", getIndent(info))
		elseif info.code == "RETURN" then
			o[#o + 1] = string.format("%s RETURN", getIndent(info))
		elseif info.code == "LT" then
			o[#o + 1] = string.format("%s %s", getIndent(info), getLT(info))
		elseif info.code == "EQ" then
			o[#o + 1] = string.format("%s %s", getIndent(info), getEQ(info))
		elseif info.code == "SECTION" then
			o[#o + 1] = getSECTION(info)
			o[#o + 1] = HEADERSTR
		elseif info.code == "CLOSURE" then
			closure = info.comment
		elseif info.code == "JMP" then
			o[#o + 1] = string.format("%s != go%s", o[#o], info.comment)
		end
	    end
        end
	return o
    end
    --
    function plugin.init(p,d,s)
	ENV._G.print = p
	ENV._G.dprint = d
        plugin.savename = s
    end

    function plugin.callplugin(str)
	local o=run(str)
	return o
    end

    function plugin.debugplugin(str)
	local o=run(str)
        for k in pairs(o) do
            print(o[k])
        end
	return o
    end
    -- plugin.callplugin("")

return plugin
