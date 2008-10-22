# Install script for directory: /home/scarab/tuxanci/src/modules

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodBasic.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodBasic.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodWall.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodWall.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodPipe.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodPipe.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodMove.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodMove.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodTeleport.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodTeleport.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so")
  FILE(INSTALL DESTINATION "/usr/local/lib/tuxanci-dev" TYPE SHARED_LIBRARY FILES "/home/scarab/tuxanci/build/src/modules/libmodAI.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib/tuxanci-dev/libmodAI.so")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

