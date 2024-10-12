#GLFW
string(TIMESTAMP BEFORE "%s")

if (NOT EMSCRIPTEN)
	CPMAddPackage(
		NAME "glfw"
		URL "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip"
		OPTIONS ("GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_DOCS OFF")
	)
	find_package(glfw REQUIRED)
	set (glfw_INCLUDE_DIR ${glfw_SOURCE_DIR}/include)
	include_directories(${glfw_INCLUDE_DIR})
else()
	CPMAddPackage(
		NAME "emscripten-glfw"
		URL "https://github.com/pongasoft/emscripten-glfw/releases/download/v3.4.0.20241004/emscripten-glfw3-3.4.0.20241004.zip"
	)

	find_package(emscripten-glfw REQUIRED)

	set (emscripten-glfw_INCLUDE_DIR ${emscripten-glfw_SOURCE_DIR}/include)
	include_directories(${emscripten-glfw_INCLUDE_DIR} ${emscripten-glfw_SOURCE_DIR}/external)
endif()

string(TIMESTAMP AFTER "%s")
math(EXPR DELTAglfw "${AFTER}-${BEFORE}")
MESSAGE(STATUS "glfw TIME: ${DELTAglfw}s")