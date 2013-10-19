#include <SDL/SDL.h>

#include "event.hpp"
#include "engine.hpp"

typedef Sint32 SDL_Keycode;

static const char *keynames[0x8000];

/** from https://code.google.com/r/kyberneticist-webport/source/browse/project_files/web_exp/pas2c_build/emcc/patches/sdl_patch.c */
static void initKeys()
{
	keynames[SDLK_BACKSPACE] = "backspace";
	keynames[SDLK_TAB] = "tab";
	keynames[SDLK_CLEAR] = "clear";
	keynames[SDLK_RETURN] = "return";
	keynames[SDLK_PAUSE] = "pause";
	keynames[SDLK_ESCAPE] = "escape";
	keynames[SDLK_SPACE] = "space";
	keynames[SDLK_EXCLAIM]  = "!";
	keynames[SDLK_QUOTEDBL]  = "\"";
	keynames[SDLK_HASH]  = "#";
	keynames[SDLK_DOLLAR]  = "$";
	keynames[SDLK_AMPERSAND]  = "&";
	keynames[SDLK_QUOTE] = "'";
	keynames[SDLK_LEFTPAREN] = "(";
	keynames[SDLK_RIGHTPAREN] = ")";
	keynames[SDLK_ASTERISK] = "*";
	keynames[SDLK_PLUS] = "+";
	keynames[SDLK_COMMA] = ",";
	keynames[SDLK_MINUS] = "-";
	keynames[SDLK_PERIOD] = ".";
	keynames[SDLK_SLASH] = "/";
	keynames[SDLK_0] = "0";
	keynames[SDLK_1] = "1";
	keynames[SDLK_2] = "2";
	keynames[SDLK_3] = "3";
	keynames[SDLK_4] = "4";
	keynames[SDLK_5] = "5";
	keynames[SDLK_6] = "6";
	keynames[SDLK_7] = "7";
	keynames[SDLK_8] = "8";
	keynames[SDLK_9] = "9";
	keynames[SDLK_COLON] = ":";
	keynames[SDLK_SEMICOLON] = ";";
	keynames[SDLK_LESS] = "<";
	keynames[SDLK_EQUALS] = "=";
	keynames[SDLK_GREATER] = ">";
	keynames[SDLK_QUESTION] = "?";
	keynames[SDLK_AT] = "@";
	keynames[SDLK_LEFTBRACKET] = "[";
	keynames[SDLK_BACKSLASH] = "\\";
	keynames[SDLK_RIGHTBRACKET] = "]";
	keynames[SDLK_CARET] = "^";
	keynames[SDLK_UNDERSCORE] = "_";
	keynames[SDLK_BACKQUOTE] = "`";
	keynames[SDLK_a] = "a";
	keynames[SDLK_b] = "b";
	keynames[SDLK_c] = "c";
	keynames[SDLK_d] = "d";
	keynames[SDLK_e] = "e";
	keynames[SDLK_f] = "f";
	keynames[SDLK_g] = "g";
	keynames[SDLK_h] = "h";
	keynames[SDLK_i] = "i";
	keynames[SDLK_j] = "j";
	keynames[SDLK_k] = "k";
	keynames[SDLK_l] = "l";
	keynames[SDLK_m] = "m";
	keynames[SDLK_n] = "n";
	keynames[SDLK_o] = "o";
	keynames[SDLK_p] = "p";
	keynames[SDLK_q] = "q";
	keynames[SDLK_r] = "r";
	keynames[SDLK_s] = "s";
	keynames[SDLK_t] = "t";
	keynames[SDLK_u] = "u";
	keynames[SDLK_v] = "v";
	keynames[SDLK_w] = "w";
	keynames[SDLK_x] = "x";
	keynames[SDLK_y] = "y";
	keynames[SDLK_z] = "z";
	keynames[SDLK_DELETE] = "delete";

#ifndef EMSCRIPTEN
	keynames[SDLK_WORLD_0] = "world 0";
	keynames[SDLK_WORLD_1] = "world 1";
	keynames[SDLK_WORLD_2] = "world 2";
	keynames[SDLK_WORLD_3] = "world 3";
	keynames[SDLK_WORLD_4] = "world 4";
	keynames[SDLK_WORLD_5] = "world 5";
	keynames[SDLK_WORLD_6] = "world 6";
	keynames[SDLK_WORLD_7] = "world 7";
	keynames[SDLK_WORLD_8] = "world 8";
	keynames[SDLK_WORLD_9] = "world 9";
	keynames[SDLK_WORLD_10] = "world 10";
	keynames[SDLK_WORLD_11] = "world 11";
	keynames[SDLK_WORLD_12] = "world 12";
	keynames[SDLK_WORLD_13] = "world 13";
	keynames[SDLK_WORLD_14] = "world 14";
	keynames[SDLK_WORLD_15] = "world 15";
	keynames[SDLK_WORLD_16] = "world 16";
	keynames[SDLK_WORLD_17] = "world 17";
	keynames[SDLK_WORLD_18] = "world 18";
	keynames[SDLK_WORLD_19] = "world 19";
	keynames[SDLK_WORLD_20] = "world 20";
	keynames[SDLK_WORLD_21] = "world 21";
	keynames[SDLK_WORLD_22] = "world 22";
	keynames[SDLK_WORLD_23] = "world 23";
	keynames[SDLK_WORLD_24] = "world 24";
	keynames[SDLK_WORLD_25] = "world 25";
	keynames[SDLK_WORLD_26] = "world 26";
	keynames[SDLK_WORLD_27] = "world 27";
	keynames[SDLK_WORLD_28] = "world 28";
	keynames[SDLK_WORLD_29] = "world 29";
	keynames[SDLK_WORLD_30] = "world 30";
	keynames[SDLK_WORLD_31] = "world 31";
	keynames[SDLK_WORLD_32] = "world 32";
	keynames[SDLK_WORLD_33] = "world 33";
	keynames[SDLK_WORLD_34] = "world 34";
	keynames[SDLK_WORLD_35] = "world 35";
	keynames[SDLK_WORLD_36] = "world 36";
	keynames[SDLK_WORLD_37] = "world 37";
	keynames[SDLK_WORLD_38] = "world 38";
	keynames[SDLK_WORLD_39] = "world 39";
	keynames[SDLK_WORLD_40] = "world 40";
	keynames[SDLK_WORLD_41] = "world 41";
	keynames[SDLK_WORLD_42] = "world 42";
	keynames[SDLK_WORLD_43] = "world 43";
	keynames[SDLK_WORLD_44] = "world 44";
	keynames[SDLK_WORLD_45] = "world 45";
	keynames[SDLK_WORLD_46] = "world 46";
	keynames[SDLK_WORLD_47] = "world 47";
	keynames[SDLK_WORLD_48] = "world 48";
	keynames[SDLK_WORLD_49] = "world 49";
	keynames[SDLK_WORLD_50] = "world 50";
	keynames[SDLK_WORLD_51] = "world 51";
	keynames[SDLK_WORLD_52] = "world 52";
	keynames[SDLK_WORLD_53] = "world 53";
	keynames[SDLK_WORLD_54] = "world 54";
	keynames[SDLK_WORLD_55] = "world 55";
	keynames[SDLK_WORLD_56] = "world 56";
	keynames[SDLK_WORLD_57] = "world 57";
	keynames[SDLK_WORLD_58] = "world 58";
	keynames[SDLK_WORLD_59] = "world 59";
	keynames[SDLK_WORLD_60] = "world 60";
	keynames[SDLK_WORLD_61] = "world 61";
	keynames[SDLK_WORLD_62] = "world 62";
	keynames[SDLK_WORLD_63] = "world 63";
	keynames[SDLK_WORLD_64] = "world 64";
	keynames[SDLK_WORLD_65] = "world 65";
	keynames[SDLK_WORLD_66] = "world 66";
	keynames[SDLK_WORLD_67] = "world 67";
	keynames[SDLK_WORLD_68] = "world 68";
	keynames[SDLK_WORLD_69] = "world 69";
	keynames[SDLK_WORLD_70] = "world 70";
	keynames[SDLK_WORLD_71] = "world 71";
	keynames[SDLK_WORLD_72] = "world 72";
	keynames[SDLK_WORLD_73] = "world 73";
	keynames[SDLK_WORLD_74] = "world 74";
	keynames[SDLK_WORLD_75] = "world 75";
	keynames[SDLK_WORLD_76] = "world 76";
	keynames[SDLK_WORLD_77] = "world 77";
	keynames[SDLK_WORLD_78] = "world 78";
	keynames[SDLK_WORLD_79] = "world 79";
	keynames[SDLK_WORLD_80] = "world 80";
	keynames[SDLK_WORLD_81] = "world 81";
	keynames[SDLK_WORLD_82] = "world 82";
	keynames[SDLK_WORLD_83] = "world 83";
	keynames[SDLK_WORLD_84] = "world 84";
	keynames[SDLK_WORLD_85] = "world 85";
	keynames[SDLK_WORLD_86] = "world 86";
	keynames[SDLK_WORLD_87] = "world 87";
	keynames[SDLK_WORLD_88] = "world 88";
	keynames[SDLK_WORLD_89] = "world 89";
	keynames[SDLK_WORLD_90] = "world 90";
	keynames[SDLK_WORLD_91] = "world 91";
	keynames[SDLK_WORLD_92] = "world 92";
	keynames[SDLK_WORLD_93] = "world 93";
	keynames[SDLK_WORLD_94] = "world 94";
	keynames[SDLK_WORLD_95] = "world 95";
#endif

	keynames[SDLK_KP0] = "[0]";
	keynames[SDLK_KP1] = "[1]";
	keynames[SDLK_KP2] = "[2]";
	keynames[SDLK_KP3] = "[3]";
	keynames[SDLK_KP4] = "[4]";
	keynames[SDLK_KP5] = "[5]";
	keynames[SDLK_KP6] = "[6]";
	keynames[SDLK_KP7] = "[7]";
	keynames[SDLK_KP8] = "[8]";
	keynames[SDLK_KP9] = "[9]";
	keynames[SDLK_KP_PERIOD] = "[.]";
	keynames[SDLK_KP_DIVIDE] = "[/]";
	keynames[SDLK_KP_MULTIPLY] = "[*]";
	keynames[SDLK_KP_MINUS] = "[-]";
	keynames[SDLK_KP_PLUS] = "[+]";
	keynames[SDLK_KP_ENTER] = "enter";
	keynames[SDLK_KP_EQUALS] = "equals";

	keynames[SDLK_UP] = "up";
	keynames[SDLK_DOWN] = "down";
	keynames[SDLK_RIGHT] = "right";
	keynames[SDLK_LEFT] = "left";
	keynames[SDLK_DOWN] = "down";
	keynames[SDLK_INSERT] = "insert";
	keynames[SDLK_HOME] = "home";
	keynames[SDLK_END] = "end";
	keynames[SDLK_PAGEUP] = "page up";
	keynames[SDLK_PAGEDOWN] = "page down";

	keynames[SDLK_F1] = "f1";
	keynames[SDLK_F2] = "f2";
	keynames[SDLK_F3] = "f3";
	keynames[SDLK_F4] = "f4";
	keynames[SDLK_F5] = "f5";
	keynames[SDLK_F6] = "f6";
	keynames[SDLK_F7] = "f7";
	keynames[SDLK_F8] = "f8";
	keynames[SDLK_F9] = "f9";
	keynames[SDLK_F10] = "f10";
	keynames[SDLK_F11] = "f11";
	keynames[SDLK_F12] = "f12";
	keynames[SDLK_F13] = "f13";
	keynames[SDLK_F14] = "f14";
	keynames[SDLK_F15] = "f15";

	keynames[SDLK_NUMLOCK] = "numlock";
	keynames[SDLK_CAPSLOCK] = "caps lock";
	keynames[SDLK_SCROLLOCK] = "scroll lock";
	keynames[SDLK_RSHIFT] = "right shift";
	keynames[SDLK_LSHIFT] = "left shift";
	keynames[SDLK_RCTRL] = "right ctrl";
	keynames[SDLK_LCTRL] = "left ctrl";
	keynames[SDLK_RALT] = "right alt";
	keynames[SDLK_LALT] = "left alt";
	keynames[SDLK_RMETA] = "right meta";
	keynames[SDLK_LMETA] = "left meta";
	keynames[SDLK_LSUPER] = "left super";	/* "Windows" keys */
	keynames[SDLK_RSUPER] = "right super";
	keynames[SDLK_MODE] = "alt gr";
	keynames[SDLK_COMPOSE] = "compose";

	keynames[SDLK_HELP] = "help";
	keynames[SDLK_PRINT] = "print screen";
	keynames[SDLK_SYSREQ] = "sys req";
	keynames[SDLK_BREAK] = "break";
	keynames[SDLK_MENU] = "menu";
	keynames[SDLK_POWER] = "power";
	keynames[SDLK_EURO] = "euro";
	keynames[SDLK_UNDO] = "undo";
}


const char * mySDL_GetKeyName(SDL_Keycode key)
{
	const char *keyname;

	keyname = keynames[key];
	if (keyname == NULL) {
		keyname = "unknown key";
	}
	return keyname;
}

EventManager::EventManager(Engine& eng) : engine(eng), _grab_cursor(false)
{
	initKeys();
	SDL_EnableKeyRepeat(0, 0);
}

void EventManager::poll()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		handle_event(event);
	}
	_warped = false;
}

void EventManager::grab_cursor(bool grab)
{
#ifdef EMSCRIPTEN
	SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF); // in native, this grabs keyboard too
#else
	_grab_cursor = grab;
	check_grab();
#endif
}

void EventManager::handle_event(const SDL_Event& event)
{
	switch(event.type)
	{
		case SDL_QUIT:
			engine.stop();
			break;
		case SDL_KEYUP:
			engine.key_release(mySDL_GetKeyName(event.key.keysym.sym));
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_F1) {
				engine.toggle_update();
			} else if (event.key.keysym.sym == SDLK_F2) {
				engine.toggle_draw();
			} else if (event.key.keysym.sym == SDLK_F3) {
				engine.lua.reload_code();
			} else if (event.key.keysym.sym == SDLK_F4) {
				engine.display.toggle_debug_mode();
			} else {
				engine.key_press(mySDL_GetKeyName(event.key.keysym.sym));
			}
			break;
		case SDL_MOUSEMOTION:
#ifndef EMSCRIPTEN
			if (not _warped) {
#endif
				engine.mouse_motion(event.motion.x, event.motion.y,
									event.motion.xrel, event.motion.yrel);
#ifndef EMSCRIPTEN
				check_grab();
			}
#endif
			break;
		case SDL_MOUSEBUTTONDOWN:
			engine.mouse_press(event.button.x, event.button.y, event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			engine.mouse_release(event.button.x, event.button.y, event.button.button);
			break;
		case SDL_VIDEORESIZE:
			engine.resize_event(event.resize.w, event.resize.h);
			break;
		default:
			break;
	}
}

void EventManager::check_grab()
{
	if (_grab_cursor) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		int w = SDL_GetVideoInfo()->current_w;
		int h = SDL_GetVideoInfo()->current_h;
		if (x < w / 4 || x > w * 3/4 || y < h / 4 || y > h * 3/4) {
			SDL_WarpMouse(w / 2, h / 2);
			_warped = true;
		}
	}
}

