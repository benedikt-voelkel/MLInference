# Find Eigen

if(NOT EIGEN_DIR)
  message(FATAL_ERROR "Please specify -DEIGEN_DIR pointing to the top of the
                       install tree.")
endif()

find_path(EIGEN3_INCLUDE_DIR
          NAMES Eigen/Core
          PATHS ${EIGEN_DIR}/include/eigen3
)

if(NOT EIGEN3_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Eigen")
endif()

# Everything found
message(STATUS "Found Eigen")
message(STATUS "Found Eigen includes in ${EIGEN3_INCLUDE_DIR}")
set(Eigen_FOUND TRUE)
