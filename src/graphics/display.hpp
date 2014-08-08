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

#define GL_GLEXT_PROTOTYPES
#ifndef EMSCRIPTEN
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL/SDL_opengl.h>
#endif

#include <cassert>

struct SDL_Surface;
struct SDL_Window;

#include "buffer.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "surface.hpp"

enum BlendMode {
	DEFAULT = 0,
	ALPHA = 0,
	ADD,
	MULT,
};

class Display
{
private:
	Buffer default_buffer;
	SDL_Window * sdl_window;
	Surface * screen;

	Shader * default_shader;
	Shader * current_shader;

	Surface * current;
	Surface * current_from;

	Buffer * current_buffer;

	float r;
	float g;
	float b;
	float alpha;

	Camera camera;

	float point_size;

	int original_width;
	int original_height;

	bool available;
	bool debug_mode;

	Display(const Display&);
	Display& operator=(const Display&);

	inline void convert_texcoords(float x, float y, float *dx, float *dy) const
	{
		*dx = x / current_from->texw;
		*dy = y / current_from->texh;
	}

	int create_window(int w, int h);
	Shader* create_default_shader();

public:
	Display(bool server_mode);
	~Display();

	void set_title(const char *title) const;
	void show_cursor(bool) const;
	void resize(int w, int h);
	void screen2scene(float x, float y, float * tx, float * ty) const;
	void toggle_debug_mode();
	void set_fullscreen(bool fullscreen);

	void set_color(int red, int green, int blue);
	void set_alpha(int a);

	void get_color(int *red, int *green, int *blue)
	{
		assert(red);
		assert(green);
		assert(blue);

		*red = this->r * 255;
		*green = this->g * 255;
		*blue = this->b * 255;
	};
	void get_alpha(int *a)
	{
		assert(a);

		*a = this->alpha * 255;
	};

	void set_point_size(float size);
	void get_point_size(float *size)
	{
		*size = this->point_size;
	};
	void set_line_width(float width);
	void set_blend_mode(BlendMode mode);
	void set_filter(Surface* surface, FilterMode mode) const;

	void reset_camera();
	void set_camera_position(float dx, float dy);
	void set_camera_angle(float angle);
	void set_camera_zoom(float zoom);
	const Camera& get_camera() const
	{
		return camera;
	};

	Surface* get_screen() const;
	Surface* create_surface(unsigned int w, unsigned int h, unsigned int texw, unsigned int texh, unsigned char* pixels) const;
	Surface* new_surface(int w, int h, bool force_npot) const;
	int load_surface(const char *filename, Surface **surface) const;
	void free_surface(Surface*);

	void draw_on(Surface*);
	void draw_from(Surface*);
	Surface* get_draw_on() const
	{
		return current;
	};
	Surface* get_draw_from() const
	{
		return current_from;
	};

	void draw_background() const;
	void draw_point(float x, float y);
	void draw_point_tex(float xi, float yi, float xd, float yd);
	void draw_line(float x1, float y1, float x2, float y2);
	void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void draw_surface(float, float, float, float, float, float,
	                  float, float, float, float, float, float);
	void draw_quad(float, float, float, float, float, float, float, float,
	               float, float, float, float, float, float, float, float);

	Shader* new_shader(const char* strvert, const char* strfragcolor, const char* strfragtex, char** error);
	void use_shader(Shader*);
	void use_default_shader();
	void free_shader(Shader*);

	Buffer* new_buffer(unsigned int size = BUFFER_DEFAULT_SIZE);
	void use_buffer(Buffer*);
	void use_default_buffer();
	void draw_buffer(Buffer*, float dx, float dy);

	void flip();
	bool is_available() const;

	Buffer* get_current_buffer() const
	{
		return current_buffer;
	}

	bool is_debug() const
	{
		return debug_mode;
	}
};

