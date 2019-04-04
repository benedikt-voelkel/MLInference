# Load backends headers and source files if found

message(STATUS "Look for ML backends")

# Nothing found so far
set(HAVE_BACKENDS FALSE)

# Boost
# Required for LWTNN
find_package(Boost)

# Eigen
# Requried for LWTNN
find_package(Eigen)

if(Boost_FOUND AND Eigen3_FOUND)
  find_package(LWTNN)
  if(LWTNN_FOUND)
    # Add directories to be included
    set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIR} ${Eigen3_INCLUDE_DIR}
                                 ${LWTNN_INCLUDE_DIR})
    set(LIBS ${LIBS} ${Boost_LIBRARIES} ${LWTNN_LIBRARIES})
    # Additional header and source files to be used
    set(HEADERS ${HEADERS}
      ${CXX_INCLUDE_DIR}/${MODULE_NAME}/LWTNNKernel.h
      ${CXX_INCLUDE_DIR}/${MODULE_NAME}/LWTNNConfig.h
    )
    set(SRCS ${SRCS}
        ${CXX_SOURCE_DIR}/LWTNNKernel.cxx
        ${CXX_SOURCE_DIR}/LWTNNConfig.cxx
    )
    # Add LWTNN backend used in configuration setup
    list(APPEND _backends LWTNN)
    # Build the conversion script
    set(SCRIPTS ${SCRIPTS} ${SCRIPTS_SOURCE_DIR}/keras2lwtnn)

    # Add library paths to setup
    list(APPEND _configure_shared_library_paths ${LWTNN_LIBRARY_DIR})
  endif(LWTNN_FOUND)
endif(Boost_FOUND AND Eigen3_FOUND)


# rabit
# Required by XGBoost
find_package(rabit)

# dmlc (aka dmlc-core)
# Required by XGBoost
find_package(dmlc)

if(rabit_FOUND AND dmlc_FOUND)
  # Try to find XGBoost
  find_package(XGBoost)
  if(XGBoost_FOUND)
    # Add directories to be included
    set(INCLUDES ${INCLUDES} ${XGBoost_INCLUDE_DIR} ${rabit_INCLUDE_DIR}
                             ${dmlc_INCLUDE_CIR})
    set(LIBS ${LIBS} ${XGBoost_LIBRARIES} ${rabit_LIBRARIES}
                             ${dmlc_LIBRARIES})
    # Additional header and source files to be used
    set(HEADERS ${HEADERS}
      ${CXX_INCLUDE_DIR}/${MODULE_NAME}/XGBoostKernel.h
    )
    set(SRCS ${SRCS}
        ${CXX_SOURCE_DIR}/XGBoostKernel.cxx
    )
    # Add LWTNN backend used in configuration setup
    list(APPEND _backends XGBoost)

    # Add library paths to setup
    list(APPEND _configure_static_library_paths ${rabit_LIBRARY_DIR}
                                                ${dmlc_LIBRARY_DIR})
    list(APPEND _configure_shared_library_paths ${XGBoost_LIBRARY_DIR})
  endif(XGBoost_FOUND)
endif(rabit_FOUND AND dmlc_FOUND)


if(NOT _backends)
  message(WARNING "No backends found.")
else()
  message(STATUS "Build with backends:")
  foreach(b ${_backends})
    message("${b}")
  endforeach()
  set(HAVE_BACKENDS TRUE)
endif(NOT _backends)
