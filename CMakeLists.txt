# The MIT License (MIT)
# Copyright(c) 2024 tsai shie jiun
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without  restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, #ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


# -----------------------------------------------------------
# Setting cmake requirements
# -----------------------------------------------------------
cmake_minimum_required(VERSION 3.6)
project(SimpleCameraApp VERSION 2)

# image saving option. Default - ON
option(OPTION_IMGSAVE "Build feature imgsave option" ON)

# App installation options
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr/local")
endif()
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_RPATH INSTALL_RPATH_USE_LINK_PATH TRUE)
# -----------------------------------------------------------
# Find core dependencies
# -----------------------------------------------------------

# find Qt in system
set(QT Qt6)
set(APP_MODULES Widgets Core Gui )
find_package(QT NAMES Qt6 REQUIRED) 
if(${QT_FOUND})
	message(STATUS "Found Qt version:" ${QT_VERSION} ", Prefix:" ${QT})
	find_package(${QT} COMPONENTS ${APP_MODULES} REQUIRED)
else()
	message(FATAL_ERROR "Unable to find Qt6 in system.")
endif()

#find opencv
find_package(OpenCV REQUIRED)
if(${OpenCV_FOUND})
	message(STATUS "OpenCV information:") 
	message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
	message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
else()
	message(FATAL_ERROR "Unable to find OpenCV in system.")
endif()

# core libs deps
set(MY_APP_LIBS ${OpenCV_LIBS} ${QT}::Widgets ${QT}::Core ${QT}::Gui curl)

if(OPTION_IMGSAVE)
	# CMake 3.30 and above prefer to not provide the FindBoost module so that find_package(Boost) calls
	# see https://cmake.org/cmake/help/latest/policy/CMP0167.html for more info!
 	cmake_policy(SET CMP0167 NEW) 
	message(STATUS "OPTION_IMGSAVE= On")
	#=========================================================
	# Find boost filesystem
	find_package(Boost REQUIRED filesystem)
	if(${Boost_FOUND})
		message(STATUS "Boost information:")
		message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
		message(STATUS "Boost_LIBS = ${Boost_LIBRARIES}")
		list(APPEND MY_APP_LIBS ${Boost_LIBRARIES})
	else()
		message(FATAL_ERROR "Unable to find Boost in system!")
		#stop build!
	endif()
endif()
message(STATUS "Added to list: ${MY_APP_LIBS}")
# -----------------------------------------------------------
# Find Qt resource file
# -----------------------------------------------------------
set(QTAPP_RESOURCE ${CMAKE_SOURCE_DIR}/camera.qrc)
# -----------------------------------------------------------
# Link core dependencies
# -----------------------------------------------------------
add_subdirectory(Base)

