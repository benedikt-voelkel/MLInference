# @author Benedikt Volkel
# @brief  cmake setup for MLInference

set(MODULE_NAME "MLInference")

# Minimum version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.5.0 FATAL_ERROR)

# The modules
SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}
    )

project(${MODULE_NAME})

# Require variables set by user:
if(NOT EIGEN_DIR OR NOT LWTNN_DIR)
  MESSAGE(FATAL_ERROR "Please specify, -DEIGEN_DIR and -DLWTNN_DIR
                       to point to the respective top of the tree.")
endif()


#############################
# COMPILER AND LINKER FLAGS #
#############################
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 -g")

#########
# Boost #
#########
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

#########
# Eigen #
#########
find_package(Eigen REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

#########
# LWTNN #
#########
find_package(LWTNN REQUIRED)
include_directories(${LWTNN_INCLUDE_DIR})

########
# This #
########

# Sources for built are here.
SET(CXX_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
SET(CXX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
SET(SCRIPTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/scripts)
SET(CONFIG_SOURCE_DIR ${CMAKE_SOURCE_DIR}/config)

# Install paths
SET(INSTALL_BINARY_DIR ${CMAKE_INSTALL_PREFIX}/bin)
SET(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
SET(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/${MODULE_NAME})
SET(INSTALL_SCRIPTS_DIR ${CMAKE_INSTALL_PREFIX}/scripts)
SET(INSTALL_CONFIG_DIR ${CMAKE_INSTALL_PREFIX}/config)

include_directories(${CXX_INCLUDE_DIR})

# Library name
SET(LIBRARY_NAME ${MODULE_NAME})

# Required source files to build the library.
SET(LIB_SOURCES
    ${CXX_SOURCE_DIR}/LWTNNKernel.cxx
    ${CXX_SOURCE_DIR}/MLManager.cxx
   )




# Requried headers to build the library.
set(HEADERS
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLKernel.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/LWTNNKernel.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/MLManager.h
   ${CXX_INCLUDE_DIR}/${MODULE_NAME}/Types.h
  )

# The conversion script to produce LWTNN JSON model file from keras architecture
# and weights files
SET(KERAS_TO_LWTNN_CONVERSION_SCRIPT
    ${SCRIPTS_SOURCE_DIR}/keras2lwtnn
    )

# Build a library from the sources specified above
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${LIBRARY_NAME} ${LWTNN_LIBRARIES}
                                     ${Boost_LIBRARIES})


# Finally configure the setup script
# Boost (So far no libraries have been requested)
#SET(boost_lib_dir ${Boost_LIBRARY_DIR})
SET(boost_inc_dir ${Boost_INCLUDE_DIR})
# Eigen
SET(eigen3_inc_dir ${EIGEN3_INCLUDE_DIR})
# LWTNN
SET(lwtnn_lib_dir ${LWTNN_LIBRARY_DIR})
SET(lwtnn_inc_dir ${LWTNN_INCLUDE_DIR})
SET(lwtnn_bin_dir ${LWTNN_BIN_DIR})
SET(lwtnn_converter_dir ${LWTNN_CONVERTER_DIR})
# This project
SET(mlinference_inc_dir ${INSTALL_INCLUDE_DIR})
SET(mlinference_lib_dir ${INSTALL_LIBRARY_DIR})
SET(mlinference_scripts_dir ${INSTALL_SCRIPTS_DIR})
configure_file(${CONFIG_SOURCE_DIR}/setup.in ${CMAKE_BINARY_DIR}/setup)

# Install headers
install(FILES ${HEADERS} DESTINATION ${INSTALL_INCLUDE_DIR})
# Install the LWTNN converter script
install(PROGRAMS ${KERAS_TO_LWTNN_CONVERSION_SCRIPT}
        DESTINATION ${INSTALL_SCRIPTS_DIR})
# Install libraries
install(TARGETS ${LIBRARY_NAME} DESTINATION ${INSTALL_LIBRARY_DIR})
# Install the setup script
install(FILES ${CMAKE_BINARY_DIR}/setup DESTINATION ${INSTALL_BINARY_DIR})
