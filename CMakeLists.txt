# @author Benedikt Volkel
# @brief  cmake setup for MLInference

set(MODULE_NAME "MLInference")

# Minimum version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0 FATAL_ERROR)

project(${MODULE_NAME})

# Include for building tests, this generates option BUILD_TESTING which is ON
# by default
include(CTest)

################################################################################
# Extend module file path
################################################################################
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}
)

################################################################################
# Basic configurations
################################################################################
# Sources for built are here.
set(CXX_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CXX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SCRIPTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/scripts)
set(CONFIG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/config)

# Install paths
set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/${MODULE_NAME})
set(INSTALL_SCRIPTS_DIR ${CMAKE_INSTALL_PREFIX}/scripts)
set(INSTALL_CONFIG_DIR ${CMAKE_INSTALL_PREFIX}/config)
# Collect for configure script
set(_configure_shared_library_paths)
set(_configure_static_library_paths)

################################################################################
# Some options to decide about backend installation
################################################################################
option(WITH_LWTNN "Build with LWTNN backend" ON)
option(WITH_XGBoost "Build with XGBoost backend" ON)

################################################################################
# Compiler and linker flags
################################################################################
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 -g")


################################################################################
# Base sources and headers
################################################################################
# HEADERS and LIBS sum everything required for building.
# NOTE So far everything is compiled into one lib.
set(SRCS
    ${CXX_SOURCE_DIR}/MLManager.cxx
)
set(HEADERS
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLKernel.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLManager.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/Types.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/Utilities.h
)


################################################################################
# Used for any additional directories and libraries to be included and used
# for built
################################################################################
set(INCLUDES)
set(LIBS)
set(SCRIPTS)


################################################################################
# Check for backends
################################################################################
include(DeployBackends)


################################################################################
# Setup for library built and installation
################################################################################

include_directories(${CXX_INCLUDE_DIR} ${INCLUDES})
set(LIBRARY_NAME ${MODULE_NAME})
add_library(${LIBRARY_NAME} SHARED ${SRCS})
target_link_libraries(${LIBRARY_NAME} ${LIBS})
list(APPEND _configure_shared_library_paths ${INSTALL_LIBRARY_DIR})

################################################################################
# Configure the setup config script
################################################################################
# This project
set(SETUP_SCRIPT_IN ${CONFIG_SOURCE_DIR}/setup.in)
set(SETUP_SCRIPT_OUT ${CMAKE_BINARY_DIR}/setup)
include(BuildSetup)

################################################################################
# Build tests
################################################################################
if(NOT BUILD_TESTING)
  message(WARNING "No tests are built.")
else()
  add_subdirectory(test)
endif(NOT BUILD_TESTS)

################################################################################
# Install the project
################################################################################
# Install headers
install(FILES ${HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR})
# Install the LWTNN converter script
install(PROGRAMS ${SCRIPTS} DESTINATION ${INSTALL_SCRIPTS_DIR})
# Install libraries
install(TARGETS ${LIBRARY_NAME} DESTINATION ${INSTALL_LIBRARY_DIR})
# Install the setup script
install(FILES ${SETUP_SCRIPT_OUT} DESTINATION ${INSTALL_CONFIG_DIR})
