# Find LWTNN

SET(LIBRARY_NAME lwtnn)

find_path(LWTNN_INCLUDE_DIR
         NAMES lwtnn/Graph.hh
         PATHS ${LWTNN_DIR}/include
         )
find_path(LWTNN_LIBRARY_DIR
         NAMES "lib${LIBRARY_NAME}.so"
         PATHS ${LWTNN_DIR}/lib
         )
find_path(LWTNN_CONVERTER_DIR
        NAMES "kerasfunc2json.py"
        PATHS ${LWTNN_DIR}/converters
        )
find_path(LWTNN_BIN_DIR
      NAMES "lwtnn-test-graph"
      PATHS ${LWTNN_DIR}/bin
      )

if(NOT LWTNN_INCLUDE_DIR AND NOT LWTNN_LIBRARY_DIR)
  MESSAGE(FATAL_ERROR "Could not find LWTNN")
endif()

SET(LWTNN_LIBRARIES "-L${LWTNN_LIBRARY_DIR} -l${LIBRARY_NAME}")

# Everything found
MESSAGE(STATUS "Found LWTNN")
MESSAGE(STATUS "Found LWTNN includes in ${LWTNN_INCLUDE_DIR}")
MESSAGE(STATUS "Found LWTNN libraries in ${LWTNN_LIBRARY_DIR}")
MESSAGE(STATUS "Found LWTNN convrters in ${LWTNN_CONVERTER_DIR}")
MESSAGE(STATUS "Found LWTNN binaries in ${LWTNN_BIN_DIR}")
SET(LWTNN_FOUND TRUE)
