# @author Benedikt Volkel
# @brief  cmake setup for MLInference

set(MODULE_NAME "MLInference")

# Minimum version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0 FATAL_ERROR)

project(${MODULE_NAME})

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
set(LIB_SOURCES
    ${CXX_SOURCE_DIR}/MLManager.cxx
)
set(HEADERS
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLKernel.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLManager.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/Types.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/Utilities.h
)


################################################################################
# Check for backends
################################################################################

if(WITH_LWTNN)

  #########
  # Boost #
  #########
  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIR})
  set(LIBS ${LIBS} ${Boost_LIBRARIES})

  # For setup config script
  #set(boost_lib_dir ${Boost_LIBRARY_DIR})
  set(boost_inc_dir ${Boost_INCLUDE_DIR})

  #########
  # Eigen #
  #########
  find_package(Eigen REQUIRED)
  include_directories(${EIGEN3_INCLUDE_DIR})

  # For setup config script
  set(eigen3_inc_dir ${EIGEN3_INCLUDE_DIR})

  #########
  # LWTNN #
  #########
  find_package(LWTNN REQUIRED)
  include_directories(${LWTNN_INCLUDE_DIR})

  set(LIB_SOURCES ${LIB_SOURCES}
      ${CXX_SOURCE_DIR}/LWTNNKernel.cxx
      ${CXX_SOURCE_DIR}/LWTNNConfig.cxx
  )

  set(HEADERS ${HEADERS}
     ${CXX_INCLUDE_DIR}/${MODULE_NAME}/LWTNNKernel.h
     ${CXX_INCLUDE_DIR}/${MODULE_NAME}/LWTNNConfig.h
  )
  set(LIBS ${LIBS} ${LWTNN_LIBRARIES})

  set(SCRIPTS ${SCRIPTS}
      ${SCRIPTS_SOURCE_DIR}/keras2lwtnn
  )

  # For setup config script
  set(lwtnn_lib_dir ${LWTNN_LIBRARY_DIR})
  set(lwtnn_inc_dir ${LWTNN_INCLUDE_DIR})
  set(lwtnn_bin_dir ${LWTNN_BIN_DIR})
  set(lwtnn_converter_dir ${LWTNN_CONVERTER_DIR})

endif(WITH_LWTNN)

if(WITH_XGBoost)



endif(WITH_XGBoost)

################################################################################
# Setup for library built and installation
################################################################################

include_directories(${CXX_INCLUDE_DIR})
set(LIBRARY_NAME ${MODULE_NAME})
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} ${LIBS})


################################################################################
# Configure the setup config script
################################################################################
# This project
set(SETUP_SCRIPT_IN ${CONFIG_SOURCE_DIR}/setup.in)
set(SETUP_SCRIPT_OUT ${CMAKE_BINARY_DIR}/setup)

set(mlinference_inc_dir ${INSTALL_INCLUDE_DIR})
set(mlinference_lib_dir ${INSTALL_LIBRARY_DIR})
set(mlinference_scripts_dir ${INSTALL_SCRIPTS_DIR})
configure_file(${SETUP_SCRIPT_IN} ${SETUP_SCRIPT_OUT})


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
