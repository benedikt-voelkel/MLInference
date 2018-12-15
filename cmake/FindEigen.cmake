# Find Eigen

set(Eigen_FOUND FALSE)
if(NOT Eigen_DIR)
  message(STATUS "Eigen cannot be found. Specify -DEigen_DIR pointing to the top
                  of the install tree.")
  return()
endif()

find_path(Eigen3_INCLUDE_DIR
          NAMES Eigen/Core
          PATHS ${Eigen_DIR}/include/eigen3
)

if(NOT Eigen3_INCLUDE_DIR)
  message(FATAL_ERROR "Could not find Eigen")
endif()

# Everything found
message(STATUS "Found Eigen")
message(STATUS "Found Eigen includes in ${Eigen3_INCLUDE_DIR}")
set(Eigen_FOUND TRUE)
