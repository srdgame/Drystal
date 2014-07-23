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
#pragma once

struct lua_State;
struct b2Joint;

struct Joint {
	b2Joint* joint;
	int ref;
};

int mlua_set_target_joint(lua_State* L);
int mlua_set_length_joint(lua_State* L);
int mlua_set_frequency_joint(lua_State* L);
int mlua_set_max_length_joint(lua_State* L);
int mlua_set_angle_limits_joint(lua_State* L);
int mlua_set_motor_speed_joint(lua_State* L);

