# The MIT License (MIT)
# Copyright(c) 2024 tsai shie jiun
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without  restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, #ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# -----------------------------------------------------------
# Dependencies
# -----------------------------------------------------------
# Get headers and sources
set(PROJECT_SOURCES
	src/main.cpp
	src/mainWindow.cpp
	src/tz.cpp
	src/videoIO.cpp
)

set(PROJECT_HEADERS
	include/mainWindow.h
	include/videoIO.h
	include/date.h
	include/tz.h
	include/tz_private.h
)

if(OPTION_IMGSAVE)
	list(APPEND PROJECT_SOURCES src/image_save.cpp)
	list(APPEND PROJECT_HEADERS include/image_save.h)
endif()

# Get QT wrappers and resource
qt_wrap_cpp(PROJECT_SOURCES ${PROJECT_HEADERS}) #wrap sources and headers for qt
# qt resource files
qt_add_resources(PROJECT_SOURCES ${QTAPP_RESOURCE})

# -----------------------------------------------------------
# Build rules
# -----------------------------------------------------------
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

if(OPTION_IMGSAVE)
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_IMGSAVE)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${MY_APP_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include 
							${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


# -----------------------------------------------------------
# Install rules
# -----------------------------------------------------------
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
