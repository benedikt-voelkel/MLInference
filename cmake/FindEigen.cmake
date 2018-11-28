# Find Eigen

find_path(EIGEN3_INCLUDE_DIR
         NAMES Eigen/Core
         PATHS ${EIGEN_DIR}/include/eigen3
         )

if(NOT EIGEN3_INCLUDE_DIR)
  MESSAGE(FATAL_ERROR "Could not find Eigen")
endif()

# Everything found
MESSAGE(STATUS "Found Eigen")
MESSAGE(STATUS "Found Eigen includes in ${EIGEN3_INCLUDE_DIR}")
SET(Eigen_FOUND TRUE)
