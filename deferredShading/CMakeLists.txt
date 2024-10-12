file(
 GLOB_RECURSE DEFERRED_SHADNG_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE DEFERRED_SHADING_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)

#Copies this project's asset folder to bin when it is built
if (EMSCRIPTEN)
	add_custom_target(copyAssetsDS ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/assets/
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/)
else()
	add_custom_target(copyAssetsDS ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/assets/
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)
endif()

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASYNCIFY -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3=1 -s USE_GLFW=3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ASYNCIFY -s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3=1 -s USE_GLFW=3")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sMIN_WEBGL_VERSION=2 -sNO_DISABLE_EXCEPTION_CATCHING -s ASSERTIONS=1 -s SAFE_HEAP=1 --use-port=contrib.glfw3 --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell.html")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/brick_color.jpg\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/brick2_color.jpg\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/brick2_normal.jpg\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/deferredLit.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/depthOnly.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/depthOnly.vert\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/geometryPass.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/lightVolume.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/lit.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/lit.vert\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/plane.obj\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/postprocess.frag\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/postprocess.vert\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/rock_color.jpg\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/rock_normal.jpg\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/sphere.obj\
	--preload-file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../assets/Suzanne.obj\
	")
else()
	install(FILES ${DEFERRED_SHADNG_INC} DESTINATION include/deferredShading)
endif()

add_executable(deferredShading ${DEFERRED_SHADING_SRC} ${DEFERRED_SHADNG_INC})

if(EMSCRIPTEN)
	if (BUILD_HTML)
		set_target_properties(deferredShading PROPERTIES SUFFIX ".html")
	endif()
	
	target_compile_definitions(deferredShading PUBLIC -DEMSCRIPTEN=1)
    set_target_properties(deferredShading PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

target_link_libraries(deferredShading PUBLIC glfw3 core IMGUI assimp)
target_include_directories(deferredShading PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when assignment0 is built
add_dependencies(deferredShading copyAssetsDS)