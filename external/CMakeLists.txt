###
### Lua
###
set(CORE_C lua/src/lapi.c
        lua/src/lcode.c
        lua/src/lctype.c
        lua/src/ldebug.c
        lua/src/ldo.c
        lua/src/ldump.c
        lua/src/lfunc.c
        lua/src/lgc.c
        lua/src/llex.c
        lua/src/lmem.c
        lua/src/lobject.c
        lua/src/lopcodes.c
        lua/src/lparser.c
        lua/src/lstate.c
        lua/src/lstring.c
        lua/src/ltable.c
        lua/src/ltm.c
        lua/src/lundump.c
        lua/src/lvm.c
        lua/src/lzio.c)
set(LIB_C  lua/src/lauxlib.c
        lua/src/lbaselib.c
        lua/src/lbitlib.c
        lua/src/lcorolib.c
        lua/src/ldblib.c
        lua/src/liolib.c
        lua/src/lmathlib.c
        lua/src/loslib.c
        lua/src/lstrlib.c
        lua/src/ltablib.c
        lua/src/loadlib.c
        lua/src/linit.c
        lua/src/lutf8lib.c)

include_directories(lua/src)

add_library(lualib STATIC ${CORE_C} ${LIB_C})

add_definitions(-DLUA_COMPAT_5_1)
add_definitions(-DLUA_COMPAT_5_2)
if(NOT DEFINED EMSCRIPTEN)
    add_executable(lua ${CORE_C} ${LIB_C} lua/src/lua.c)
    target_link_libraries(lua m)

    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        add_definitions(-DLUA_USE_MACOSX)
        find_library(DL_LIBRARY NAMES dl)
        target_link_libraries(lualib ${DL_LIBRARY})
        target_link_libraries(lua ${DL_LIBRARY})
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        add_definitions(-DLUA_USE_WIN)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        add_definitions(-DLUA_USE_LINUX)
        find_library(DL_LIBRARY NAMES dl)
        find_library(READLINE_LIBRARY NAMES readline)
        target_link_libraries(lualib ${DL_LIBRARY})
        target_link_libraries(lua ${DL_LIBRARY})
        target_link_libraries(lua ${READLINE_LIBRARY})
    endif()
endif()


###
### Lua_cjson
###
if(BUILD_UTILS)
        find_program(PATCH_COMMAND patch)
        if (NOT PATCH_COMMAND)
		message(FATAL_ERROR "patch command not found")
        endif()
	add_custom_command(OUTPUT lua_cjson_patched.c
	    COMMAND ${PATCH_COMMAND} --quiet ${CMAKE_CURRENT_SOURCE_DIR}/lua-cjson/lua_cjson.c ${CMAKE_CURRENT_SOURCE_DIR}/cjson.patch -o lua_cjson_patched.c
	    COMMENT "Patching lua_cjson.c")
	add_custom_command(OUTPUT fpconv_patched.c
	    COMMAND ${PATCH_COMMAND} --quiet ${CMAKE_CURRENT_SOURCE_DIR}/lua-cjson/fpconv.c ${CMAKE_CURRENT_SOURCE_DIR}/cjson-fpconv.patch -o fpconv_patched.c
	    COMMENT "Patching fpconv.c")

	include_directories(lua-cjson)
	add_library(lua-cjson STATIC fpconv_patched.c lua-cjson/strbuf.c lua_cjson_patched.c)
	set_target_properties(lua-cjson PROPERTIES COMPILE_DEFINITIONS USE_INTERNAL_FPCONV)
endif()


###
### Box2D
###
if(BUILD_PHYSICS)
	if (EMSCRIPTEN)
		set(BOX2D_BUILD_STATIC ON CACHE BOOL "")
		set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "")
		set(BOX2D_INSTALL_BY_DEFAULT OFF)
		set(BOX2D_INSTALL OFF CACHE BOOL "")
		add_subdirectory(box2d/Box2D)
	endif()
endif()


###
### STB
###
if(BUILD_AUDIO)
	add_library(stb_vorbis STATIC stb_vorbis.c)
	set_target_properties(stb_vorbis PROPERTIES COMPILE_DEFINITIONS "STB_VORBIS_NO_PUSHDATA_API")
endif()

if(BUILD_FONT)
	add_library(stb_truetype STATIC stb_truetype.c)
	set_target_properties(stb_truetype PROPERTIES COMPILE_DEFINITIONS "STB_TRUETYPE_IMPLEMENTATION")
endif()


###
### miniz
###
if(EMSCRIPTEN)
	add_library(miniz STATIC miniz.c)
endif()

###
### wavloader
###
if(BUILD_AUDIO)
	add_library(wavloader STATIC wavloader.c)
endif()

###
### libpng
###
if(BUILD_GRAPHICS)
        if(EMSCRIPTEN)
		include(zlibCMakeLists.cmake)
		include(libpngCMakeLists.cmake)
        endif()
endif()
