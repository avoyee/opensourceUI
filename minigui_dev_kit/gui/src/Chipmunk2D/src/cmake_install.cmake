# Install script for directory: /home/yongliang.li/i2/gui/src/Chipmunk2D/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/yongliang.li/i2/gui/libarary")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/src/CMakeFiles/CMakeRelink.dir/libchipmunk.so.7.0.2"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/src/CMakeFiles/CMakeRelink.dir/libchipmunk.so.7"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/src/CMakeFiles/CMakeRelink.dir/libchipmunk.so"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/yongliang.li/i2/gui/src/Chipmunk2D/src/libchipmunk.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/chipmunk" TYPE FILE FILES
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpConstraint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpPolyline.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk_types.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpSpace.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpMarch.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpHastySpace.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpRatchetJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpBB.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpGrooveJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpSpatialIndex.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpSlideJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpPolyShape.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpArbiter.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpRobust.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpSimpleMotor.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpDampedRotarySpring.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpVect.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpDampedSpring.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpShape.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk_ffi.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk_structs.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk_private.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpRotaryLimitJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpBody.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpTransform.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpGearJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpPivotJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/cpPinJoint.h"
    "/home/yongliang.li/i2/gui/src/Chipmunk2D/include/chipmunk/chipmunk_unsafe.h"
    )
endif()

