local drystal = drystal
local net = require 'net'
require 'common'

local hostname = 'localhost'
local sock

function drystal.init()
	drystal.resize(400, 400)

	sock = assert(net.connect(hostname, port))
	sock:set_debug()
	for i = 1, 5 do
		sock:sendlua {
			msg='hello'
		}
	end
end

function drystal.key_press(key)
	if key == 'escape' then
		sock:disconnect();
		drystal.stop()
	end
end

function drystal.update(dt)
	local x = sock:recvline()
	if x then
		sock:sendline(x + 1)
	end
	sock:flush()
end
