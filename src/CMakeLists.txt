include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/external")
include_directories("${PROJECT_SOURCE_DIR}/external/lua/src")
if (EMSCRIPTEN)
	if (BUILD_PHYSICS)
		include_directories("${PROJECT_SOURCE_DIR}/external/box2d/Box2D")
	endif()
	if (BUILD_GRAPHICS)
		include_directories("${PROJECT_SOURCE_DIR}/external/libpng")
		include_directories("${CMAKE_BINARY_DIR}/external") # for pngconf.h
	endif()
endif()

set(SOURCES
	engine.c
	dlua.c
	lua_util.c
	util.c
	log.c
)
if(EMSCRIPTEN)
	list(APPEND SOURCES main_emscripten.c)
else()
	list(APPEND SOURCES main_native.c)
endif()
if(BUILD_PHYSICS)
	add_definitions(-DBUILD_PHYSICS)
	aux_source_directory(physics SOURCES)
endif()
if(BUILD_FONT)
	add_definitions(-DBUILD_FONT)
	aux_source_directory(font SOURCES)
endif()
if(BUILD_PARTICLE)
	add_definitions(-DBUILD_PARTICLE)
	aux_source_directory(particle SOURCES)
endif()
if(BUILD_AUDIO)
	add_definitions(-DBUILD_AUDIO)
	aux_source_directory(audio SOURCES)
endif()
if(BUILD_WEB)
	add_definitions(-DBUILD_WEB)
	aux_source_directory(web SOURCES)
endif()
if(BUILD_STORAGE)
	add_definitions(-DBUILD_STORAGE)
	aux_source_directory(storage SOURCES)
endif()
if(BUILD_GRAPHICS)
	add_definitions(-DBUILD_GRAPHICS)
	aux_source_directory(graphics SOURCES)
	aux_source_directory(event SOURCES)
endif()
if(BUILD_UTILS)
	add_definitions(-DBUILD_UTILS)
	aux_source_directory(utils SOURCES)
endif()
if(BUILD_LFS)
	add_definitions(-DBUILD_LFS)
	aux_source_directory(lfs SOURCES)
endif()
if(NOT DEFINED EMSCRIPTEN AND BUILD_LIVECODING)
	add_definitions(-DBUILD_LIVECODING)
	list(APPEND SOURCES livecoding_linux.c)
endif()

include(GenerateLuaFiles)

set(DRYSTAL_OUT drystal)

set(DRYSTAL_FLAGS " ")
set(DRYSTAL_DEBUG_FLAGS " ")
set(DRYSTAL_CXX_FLAGS " ")
set(DRYSTAL_C_FLAGS " ")
set(DRYSTAL_LD_FLAGS_DEBUG " ")

add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-fvisibility=hidden"                  HAS_FVISIBILITY_HIDDEN)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wall"                                HAS_WALL)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wextra"                              HAS_WEXTRA)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wundef"                              HAS_WUNDEF)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wunreachable-code"                   HAS_WUNREACHABLE_CODE)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wwrite-strings"                      HAS_WWRITE_STRINGS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wcast-align"                         HAS_WCAST_ALIGN)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wformat=2"                           HAS_WFORMAT_2)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wformat-security"                    HAS_WFORMAT_SECURITY)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wformat-nonliteral"                  HAS_WFORMAT_NONLITERAL)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wpointer-arith"                      HAS_WPOINTER_ARITH)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wmissing-include-dirs"               HAS_WMISSING_INCLUDE_DIRS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wmissing-declarations"               HAS_WMISSING_DECLARATIONS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wmissing-prototypes"                 HAS_WMISSING_PROTOTYPES)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wmissing-variable-declarations"      HAS_WMISSING_VARIABLE_DECLARATIONS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wredundant-decls"                    HAS_WREDUNDANT_DECLS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wcast-qual"                          HAS_WCAST_QUAL)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wlogical-op"                         HAS_WLOGICAL_OP)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wdouble-promotion"                   HAS_WDOUBLE_PROMOTION)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wundef"                              HAS_WUNDEF)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wunused-macros"                      HAS_WUNUSED_MACROS)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wextra-semi"                         HAS_WEXTRA_SEMI)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wlogical-not-parentheses"            HAS_WLOGICAL_NOT_PARENTHESES)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wbool-compare"                       HAS_WBOOL_COMPARE)
add_c_cxx_flag_if_supported(DRYSTAL_FLAGS       "-Wswitch-bool"                        HAS_WSWITCH_BOOL)
add_c_cxx_flag_if_supported(DRYSTAL_DEBUG_FLAGS "-Werror"                              HAS_WERROR)
add_c_cxx_flag_if_supported(DRYSTAL_DEBUG_FLAGS "-fno-omit-frame-pointer"              HAS_FNO_OMIT_FRAME_POINTER)
if(NOT DEFINED EMSCRIPTEN)
	add_c_cxx_flag_if_supported(DRYSTAL_DEBUG_FLAGS "-fsanitize=unsigned-integer-overflow" HAS_FSANITIZE_UNSIGNED_INTERGER_OVERFLOW)
	if (HAS_FSANITIZE_UNSIGNED_INTERGER_OVERFLOW)
		set(DRYSTAL_LD_FLAGS_DEBUG "${DRYSTAL_LD_FLAGS_DEBUG} -fsanitize=unsigned-integer-overflow")
	endif()
	add_c_cxx_flag_if_supported(DRYSTAL_DEBUG_FLAGS "-fsanitize=undefined"                 HAS_FSANITIZE_UNDEFINED)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.6")
		add_cxx_flag_if_supported(DRYSTAL_CXX_FLAGS "-Weffc++" HAS_WEFFCPP)
	endif()
else()
	add_cxx_flag_if_supported(DRYSTAL_CXX_FLAGS "-Weffc++" HAS_WEFFCPP)
endif()

add_c_flag_if_supported(DRYSTAL_C_FLAGS "-std=gnu99" HAS_STD_GNU99)

add_executable(${DRYSTAL_OUT} ${SOURCES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DRYSTAL_FLAGS} ${DRYSTAL_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DRYSTAL_FLAGS} ${DRYSTAL_C_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${DRYSTAL_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DRYSTAL_DEBUG_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${DRYSTAL_LD_FLAGS_DEBUG}")

set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${DRYSTAL_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${DRYSTAL_DEBUG_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${DRYSTAL_LD_FLAGS_DEBUG}")

if(NOT DEFINED EMSCRIPTEN)
	include(FindPkgConfig)

	if(BUILD_GRAPHICS)
		find_package(PNG 1.6 REQUIRED)
		include_directories(${PNG_INCLUDE_DIR})
		add_definitions(${PNG_DEFINITIONS})
	endif()
	if(BUILD_GRAPHICS OR BUILD_FONT)
		pkg_search_module(SDL2 REQUIRED sdl2)
		include_directories(${SDL2_INCLUDE_DIRS})
		target_link_libraries(drystal ${SDL2_LIBRARIES})
	endif()

	if(BUILD_GRAPHICS OR BUILD_FONT)
		pkg_search_module(GL REQUIRED gl)
		include_directories(${GL_INCLUDE_DIRS})
		target_link_libraries(drystal ${GL_LIBRARIES})
	endif()
	if(BUILD_AUDIO)
		pkg_search_module(OPENAL REQUIRED openal)
		include_directories(${OPENAL_INCLUDE_DIRS})
		target_link_libraries(drystal ${OPENAL_LIBRARIES})
	endif()
	if (BUILD_PHYSICS)
		find_package(Box2D REQUIRED)
		add_definitions(${BOX2D_DEFINITIONS})
		include_directories(${BOX2D_INCLUDE_DIRS})
		link_directories(${BOX2D_LIBRARY_DIRS})
	endif()

	include(BuildCoverage)

	add_definitions("-DDRYSTAL_VERSION=\"${DRYSTAL_VERSION}\"")
endif()

target_link_libraries(${DRYSTAL_OUT} lualib)
if(BUILD_UTILS)
	target_link_libraries(${DRYSTAL_OUT} lua-cjson)
endif()
if(BUILD_LFS)
	target_link_libraries(${DRYSTAL_OUT} pthread)
endif()
if(BUILD_GRAPHICS)
	target_link_libraries(${DRYSTAL_OUT} ${PNG_LIBRARY})
endif()
if(BUILD_AUDIO)
	target_link_libraries(${DRYSTAL_OUT} stb_vorbis)
	target_link_libraries(${DRYSTAL_OUT} wavloader)
endif()
if(BUILD_FONT)
	target_link_libraries(${DRYSTAL_OUT} stb_truetype)
endif()
if(BUILD_PHYSICS)
	target_link_libraries(${DRYSTAL_OUT} Box2D)
endif()
if(EMSCRIPTEN)
	target_link_libraries(${DRYSTAL_OUT} miniz)
else()
	target_link_libraries(${DRYSTAL_OUT} m)
endif()
if(BUILD_LIVECODING)
	target_link_libraries(${DRYSTAL_OUT} pthread)
endif()

include(GNUInstallDirs)
install(TARGETS ${DRYSTAL_OUT} DESTINATION ${CMAKE_INSTALL_BINDIR})

