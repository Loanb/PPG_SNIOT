# Install script for directory: /home/seatech/Documents/my-workspace/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/seatech/zephyr-sdk-0.17.4/xtensa-espressif_esp32_zephyr-elf/bin/xtensa-espressif_esp32_zephyr-elf-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/arch/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/lib/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/soc/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/boards/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/subsys/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/drivers/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/sniot_zephyr_template/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/cmsis-dsp/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/cmsis_6/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/hal_espressif/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/hal_nordic/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/hal_stm32/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/xtensa/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/modules/mbedtls/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/kernel/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/cmake/flash/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/cmake/usage/cmake_install.cmake")
  include("/home/seatech/Documents/my-workspace/sniot_zephyr_template/build/zephyr/cmake/reports/cmake_install.cmake")

endif()

