local drystal = drystal

local piou = drystal.load_sound("test.wav")

function drystal.init()
	print("press p to play a wav sound")
	print("press i to play a ogg music")
	print("press l to lower the music volume")
	print("press z to lower the sound volume")
	drystal.resize(40, 40)
end

function drystal.key_press(key)
	if key == 'a' then
		drystal.engine_stop()
	elseif key == 'p' then
		drystal.play_sound(piou)
	elseif key == 'l' then
		drystal.set_music_volume(0.3)
	elseif key == 'z' then
		drystal.set_sound_volume(0.3)
	elseif key == 'i' then
		local music = drystal.load_music("test.ogg")
		drystal.play_music(music)
	end
end
