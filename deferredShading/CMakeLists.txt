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
add_custom_target(copyAssetsDS ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/assets/
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${DEFERRED_SHADNG_INC} DESTINATION include/deferredShading)
add_executable(deferredShading ${DEFERRED_SHADING_SRC} ${DEFERRED_SHADNG_INC})
target_link_libraries(deferredShading PUBLIC core IMGUI assimp)
target_include_directories(deferredShading PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when assignment0 is built
add_dependencies(deferredShading copyAssetsDS)