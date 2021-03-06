local drystal = require "drystal"

local spritesheet = assert(drystal.fromjson(io.open('image.json'):read('*all')))

vert = [[
attribute vec2 position;
attribute vec4 color;
attribute vec2 texCoord;

varying vec4 fColor;
varying vec2 fTexCoord;

uniform vec2 destinationSize;
uniform vec2 sourceSize;

#define pi ]]..math.pi..[[

uniform float tick;

float rand(vec2 co){
	return sin(dot(co.xy, vec2(12.9898,78.233)));
}

void main()
{
	vec2 pos = (2. * position/destinationSize) - 1.;
	pos.x += rand(vec2(pos.x, tick/10.+100.)) * .01;
	pos.y += rand(vec2(pos.y, tick/10.)) * .01;

	gl_Position = vec4(pos, 0., 1.);
	fColor = color;
	fTexCoord = texCoord / sourceSize;
}
]]

local x = 0
local y = 0
local width = 0
local height = 0
local shader

function drystal.init()
	drystal.resize(600, 400)
	image = assert(drystal.load_surface(spritesheet.meta.image))
	image:draw_from()
	drystal.set_alpha(0)
	surf = drystal.new_surface(64, 32)
	surf:draw_on()
	drystal.set_color(255, 0, 0)
	drystal.set_alpha(105)
	drystal.draw_rect(32, 0, 40, 40)

	drystal.set_color(200, 200, 200)
	drystal.set_alpha(255)
	local sprite = spritesheet.frames['character.png'].frame
	drystal.draw_sprite(sprite, 0, 0)
	drystal.screen:draw_on()

	shader = assert(drystal.new_shader(vert))
end

local tick = 0
function drystal.update(dt)
	tick = tick + dt
end

function drystal.draw()
	image:draw_from()
	drystal.set_alpha(255)
	drystal.set_color(10, 10, 30)
	drystal.draw_background()

	shader:use()
	shader:feed('tick', tick)
	drystal.set_color(255, 0, 0)
	drystal.set_alpha(105)
	drystal.draw_rect(16, 64, 40, 40)

	local sprite = spritesheet.frames['character.png'].frame
	drystal.set_color(200, 200, 200)
	drystal.set_alpha(255)
	drystal.draw_sprite_rotated(sprite, 16, 16, math.sin(tick))
	drystal.use_default_shader()

	drystal.set_color(100, 0, 0)
	drystal.set_alpha(200)
	drystal.draw_sprite(sprite, 16+32, 16)

	drystal.set_color(255,255,255)
	drystal.set_alpha((math.sin(tick*5)/2+0.5)*255)
	surf:draw_from()

	drystal.draw_image(0, 0, 64, 32, 0, 256)
end

function drystal.key_press(key)
	if key == 'a' then
		drystal.stop()
	end
end

function drystal.key_release(key)
end

function drystal.mouse_motion(_x, _y)
	x = _x
	y = _y
end

