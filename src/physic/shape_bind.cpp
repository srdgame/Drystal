#include <cassert>
#include <cstring>
#include <lua.hpp>

#include "macro.hpp"

DISABLE_WARNING_EFFCPP;
#include <Box2D/Box2D.h>
REENABLE_WARNING;

#include "shape_bind.hpp"
#include "physic_p.hpp"

int mlua_new_shape(lua_State* L)
{
	assert(L);

	const char* type = luaL_checkstring(L, 1);

	b2FixtureDef* fixtureDef = new b2FixtureDef;
	fixtureDef->density = 1.0f;

	if (!strcmp(type, "box")) {
		b2PolygonShape* polygon = new b2PolygonShape;
		lua_Number w = luaL_checknumber(L, 2) / 2;
		lua_Number h = luaL_checknumber(L, 3) / 2;
		lua_Number centerx = 0;
		lua_Number centery = 0;
		if (lua_gettop(L) > 3) {
			centerx = luaL_checknumber(L, 4);
			centery = luaL_checknumber(L, 5);
		}
		polygon->SetAsBox(w, h, b2Vec2(centerx, centery), 0);
		fixtureDef->shape = polygon;
	} else if (!strcmp(type, "circle")) {
		b2CircleShape* circle = new b2CircleShape;
		circle->m_radius = luaL_checknumber(L, 2);
		if (lua_gettop(L) > 2) {
			lua_Number dx = luaL_checknumber(L, 3);
			lua_Number dy = luaL_checknumber(L, 4);
			circle->m_p.Set(dx, dy);
		}
		fixtureDef->shape = circle;
	} else if (!strcmp(type, "chain")) {
		b2ChainShape* chain = new b2ChainShape;
		int number = (lua_gettop(L) - 1) / 2;
		b2Vec2* vecs = new b2Vec2[number];
		for (int i = 0; i < number; i++) {
			vecs[i].x = luaL_checknumber(L, (i + 1)*2);
			vecs[i].y = luaL_checknumber(L, (i + 1)*2 + 1);
		}
		chain->CreateLoop(vecs, number);
		delete[] vecs;
		fixtureDef->shape = chain;
	} else {
		assert(false);
		return 0;
	}

	lua_newtable(L);
	lua_pushlightuserdata(L, fixtureDef);
	lua_setfield(L, -2, "__self");
	luaL_getmetatable(L, "shape");
	lua_setmetatable(L, -2);
	return 1;
}

#define SHAPE_GETSET_SOME_VALUE(value) \
	int mlua_set_##value##_shape(lua_State* L) \
	{ \
		b2FixtureDef* fixtureDef = luam_tofixture(L, 1); \
		lua_Number value = luaL_checknumber(L, 2); \
		fixtureDef->value = value; \
		return 0; \
	} \
	int mlua_get_##value##_shape(lua_State* L) \
	{ \
		b2FixtureDef* fixtureDef = luam_tofixture(L, 1); \
		lua_pushnumber(L, fixtureDef->value); \
		return 1; \
	}
SHAPE_GETSET_SOME_VALUE(density)
SHAPE_GETSET_SOME_VALUE(restitution)
SHAPE_GETSET_SOME_VALUE(friction)

int mlua_set_sensor_shape(lua_State* L)
{
	assert(L);

	b2FixtureDef* fixtureDef = luam_tofixture(L, 1);
	bool sensor = lua_toboolean(L, 2);
	fixtureDef->isSensor = sensor;
	return 0;
}

int mlua_gc_shape(lua_State* L)
{
	assert(L);

	b2FixtureDef* fixtureDef = luam_tofixture(L, 1);
	delete fixtureDef->shape;
	delete fixtureDef;
	return 0;
}
