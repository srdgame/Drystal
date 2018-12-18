local drystal = require 'drystal'

--[[
for k, v in pairs(drystal) do
	print(k,v)
end
]]--

local lfs = drystal.lfs

for k, v in pairs(lfs) do
	print(k,v)
end

print(lfs)

drystal.stop()

