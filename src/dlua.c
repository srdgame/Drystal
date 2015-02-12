/**
 * This file is part of Drystal.
 *
 * Drystal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Drystal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Drystal.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "lua_util.h"
#include "engine.h"
#include "util.h"
#include "log.h"
#include "dlua.h"
#include "luafiles.h"
#include "module.h"
#ifdef BUILD_PHYSICS
#include "physics/api.hpp"
#endif
#ifdef BUILD_FONT
#include "truetype/api.h"
#endif
#ifdef BUILD_PARTICLE
#include "particle/api.h"
#endif
#ifdef BUILD_AUDIO
#include "audio/api.h"
#endif
#ifdef BUILD_WEB
#include "web/api.h"
#endif
#ifdef BUILD_STORAGE
#include "storage/api.h"
#endif
#ifdef BUILD_GRAPHICS
#include "event/api.h"
#include "graphics/api.h"
#endif
#ifdef BUILD_UTILS
#include "utils/api.h"
#endif
#ifdef BUILD_LIVECODING
#include "livecoding.h"
#endif

log_category("lua");

static int luaopen_drystal(lua_State*); // defined at the end of this file

struct DrystalLua {
	lua_State* L;
	int drystal_table_ref;
	bool library_loaded;
#ifdef BUILD_LIVECODING
	bool need_to_reload;
#endif
	const char* filename;
} dlua;

void dlua_init(const char *filename)
{
	dlua.L = luaL_newstate();
	dlua.drystal_table_ref = LUA_NOREF;
	dlua.filename = filename;
#ifdef BUILD_LIVECODING
	dlua.need_to_reload = false;
#endif
	dlua.library_loaded = false;
	luaL_openlibs(dlua.L);
}

void dlua_free(void)
{
	luaL_unref(dlua.L, LUA_REGISTRYINDEX, dlua.drystal_table_ref);
	lua_close(dlua.L);
}

lua_State *dlua_get_lua_state(void)
{
	return dlua.L;
}

void dlua_get_drystal_field(const char* name)
{
	lua_State *L = dlua.L;

	assert(name);

	lua_rawgeti(L, LUA_REGISTRYINDEX, dlua.drystal_table_ref);
	lua_pushstring(L, name);
	lua_rawget(L, -2);
	lua_remove(L, lua_gettop(L) - 1);
}

/**
 * Search for a function named 'name' in the drystal table.
 * Return true if found and keep the function in the lua stack
 * Otherwise, return false (stack is cleaned as needed).
 */
bool dlua_get_function(const char* name)
{
	lua_State *L = dlua.L;

	assert(name);

	dlua_get_drystal_field(name);
	if (lua_isfunction(L, -1)) {
		return true;
	}
	lua_pop(L, 1);
	return false;
}

static void remove_userpackages(void)
{
	lua_State *L = dlua.L;

	assert(lua_gettop(L) == 0);
	log_debug("Removing old packages: ");
	const char* kept[] = {
		"_G",
		LUA_COLIBNAME,
		LUA_TABLIBNAME,
		LUA_IOLIBNAME,
		LUA_OSLIBNAME,
		LUA_STRLIBNAME,
		LUA_UTF8LIBNAME,
		LUA_BITLIBNAME,
		LUA_MATHLIBNAME,
		LUA_DBLIBNAME,
		LUA_LOADLIBNAME,
		"drystal",
	};
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "loaded");
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		bool remove = true;
		const char* name = lua_tostring(L, -2);
		unsigned long i;

		for (i = 0; i < sizeof(kept) / sizeof(const char*) && remove; i++) {
			remove = remove && !streq(name, kept[i]);
		}
		if (remove) {
			lua_pushnil(L);
			log_debug("    Removed %s", name);
			lua_setfield(L, -4, name);
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 2);
	assert(lua_gettop(L) == 0);
}

static void register_modules(void)
{
#ifdef BUILD_AUDIO
	REGISTER_MODULE(audio, dlua.L);
#endif
#ifdef BUILD_PARTICLE
	REGISTER_MODULE(particle, dlua.L);
#endif
#ifdef BUILD_PHYSICS
	REGISTER_MODULE(physics, dlua.L);
#endif
#ifdef BUILD_STORAGE
	REGISTER_MODULE(storage, dlua.L);
#endif
#ifdef BUILD_FONT
	REGISTER_MODULE(truetype, dlua.L);
#endif
#ifdef BUILD_WEB
	REGISTER_MODULE(web, dlua.L);
#endif
#ifdef BUILD_GRAPHICS
	REGISTER_MODULE(graphics, dlua.L);
	REGISTER_MODULE(event, dlua.L);
#endif
#ifdef BUILD_UTILS
	REGISTER_MODULE(utils, dlua.L);
#endif
}

bool dlua_load_code(void)
{
	lua_State *L = dlua.L;

	assert(lua_gettop(L) == 0);
	if (!dlua.library_loaded) {
		// add drystal lib
		luaL_requiref(L, "drystal", luaopen_drystal, 0 /* not as global */);
		register_modules();
		lua_pop(L, 1);  /* remove lib */

		lua_pushcfunction(L, traceback);
		if (!load_luafiles(L, lua_gettop(L))) {
			lua_pop(L, 2);
			assert(lua_gettop(L) == 0);
			return false;
		}
		lua_pop(L, 1);
		dlua.library_loaded = true;
	}

	if (luaL_loadfile(L, dlua.filename)) {
		fprintf(stderr, "*** ERROR ***\n%s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
		assert(lua_gettop(L) == 0);
#ifdef BUILD_LIVECODING
		if (livecoding_is_running()) {
			engine_wait_next_reload();
		}
#endif
		return false;
	}

	call_lua_function(L, 0, 0);

	assert(lua_gettop(L) == 0);
	return true;
}

#ifdef BUILD_LIVECODING
void dlua_set_need_to_reload()
{
	dlua.need_to_reload = true;
}

bool dlua_is_need_to_reload()
{
	return dlua.need_to_reload;
}
#endif

void dlua_foreach(const char* type, void(*callback)(void* data, void* callback_arg), void* callback_arg)
{
	assert(type);
	assert(callback);

	lua_State *L = dlua.L;
	lua_getfield(L, LUA_REGISTRYINDEX, "objects");
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		if (lua_istable(L, -1)) {
			lua_getfield(L, -1, "__self");
			lua_replace(L, -2);
		}

		lua_getfield(L, -1, "__type");
		const char* t = lua_tostring(L, -1);
		lua_pop(L, 1);
		if (streq(type, t)) {
			void* data = * (void**) lua_touserdata(L, -1);
			callback(data, callback_arg);
		}

		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

bool dlua_reload_code(void)
{
	lua_State *L = dlua.L;
	if (dlua_get_function("prereload")) {
		call_lua_function(L, 0, 0);
	}
	remove_userpackages();

	log_info("Reloading code...");
	bool ok = dlua_load_code();
	if (ok) {
		dlua_call_init();
		if (dlua_get_function("postreload")) {
			call_lua_function(L, 0, 0);
		}
	}

#ifdef BUILD_LIVECODING
	dlua.need_to_reload = false;
#endif

	return ok;
}

void dlua_call_init(void)
{
	if (dlua_get_function("init")) {
		call_lua_function(dlua.L, 0, 0);
	}
}

void dlua_call_update(float dt)
{
	if (dlua_get_function("update")) {
		lua_State *L = dlua.L;
		lua_pushnumber(L, dt);
		call_lua_function(L, 1, 0);
	}
}

void dlua_call_draw(void)
{
	if (dlua_get_function("draw")) {
		call_lua_function(dlua.L, 0, 0);
	}
}

void dlua_call_atexit(void)
{
	if (dlua_get_function("atexit")) {
		call_lua_function(dlua.L, 0, 0);
	}
}

static int mlua_stop(_unused_ lua_State *L)
{
	engine_stop();
	return 0;
}

static int mlua_reload(_unused_ lua_State *L)
{
	dlua_reload_code();
	return 0;
}

static int mlua_drystal_index(_unused_ lua_State *L)
{
#ifdef BUILD_GRAPHICS
	int r;

	r = graphics_index(dlua.L);
	if (r > 0) {
		return r;
	}
#endif
	return 0;
}

int luaopen_drystal(lua_State *L)
{
	assert(L);

	lua_newtable(L);
	luaL_newmetatable(L, "__objects");
	lua_pushliteral(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	lua_setfield(L, LUA_REGISTRYINDEX, "objects");

	static const luaL_Reg lib[] = {
		{"stop", mlua_stop},
		{"reload", mlua_reload},
		{NULL, NULL}
	};

	luaL_newlib(L, lib);

	luaL_newmetatable(L, "_drystal");
	lua_pushcfunction(L, mlua_drystal_index);
	lua_setfield(L, -2, "__index");
	lua_setmetatable(L, -2);

	lua_pushvalue(L, -1);
	dlua.drystal_table_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	assert(lua_gettop(L) == 2);
	return 1;
}

