# Find rabit

set(rabit_FOUND FALSE)
if(NOT rabit_DIR)
  message(STATUS "rabit cannot be found. Specify -Drabit_DIR pointing to the top
                  of the install tree.")
  return()
endif()

set(LIBRARY_NAME rabit)

find_path(rabit_INCLUDE_DIR
          NAMES ${LIBRARY_NAME}/c_api.h
          PATHS ${rabit_DIR}/include
)
find_path(rabit_LIBRARY_DIR
          NAMES "lib${LIBRARY_NAME}.a"
          PATHS ${rabit_DIR}/lib
)

if(NOT rabit_INCLUDE_DIR )
  message(FATAL_ERROR "Could not find rabit include")
endif()

if(NOT rabit_LIBRARY_DIR)
  message(FATAL_ERROR "Could not find rabit libs")
endif()

set(rabit_LIBRARIES "-L${rabit_LIBRARY_DIR} -l${LIBRARY_NAME}")

# Everything found
message(STATUS "Found rabit")
message(STATUS "Found rabit includes in ${rabit_INCLUDE_DIR}")
message(STATUS "Found rabit libraries in ${rabit_LIBRARY_DIR}")
set(rabit_FOUND TRUE)
