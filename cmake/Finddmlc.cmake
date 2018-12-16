# Find dmlc

set(dmlc_FOUND FALSE)
if(NOT dmlc_DIR)
  message(STATUS "dmlc cannot be found. Specify -Ddmlc_DIR pointing to the top
                  of the install tree.")
  return()
endif()

set(LIBRARY_NAME dmlc)

find_path(dmlc_INCLUDE_DIR
          NAMES ${LIBRARY_NAME}/any.h
          PATHS ${dmlc_DIR}/include
)
find_path(dmlc_LIBRARY_DIR
          NAMES "lib${LIBRARY_NAME}.a"
          PATHS ${dmlc_DIR}
)

if(NOT dmlc_INCLUDE_DIR )
  message(FATAL_ERROR "Could not find dmlc include")
endif()

if(NOT dmlc_LIBRARY_DIR)
  message(FATAL_ERROR "Could not find dmlc libs")
endif()

set(dmlc_LIBRARIES "-L${dmlc_LIBRARY_DIR} -l${LIBRARY_NAME}")

# Everything found
message(STATUS "Found dmlc")
message(STATUS "Found dmlc includes in ${dmlc_INCLUDE_DIR}")
message(STATUS "Found dmlc libraries in ${dmlc_LIBRARY_DIR}")
set(dmlc_FOUND TRUE)
