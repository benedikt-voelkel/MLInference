# Configure the setup script

set(mlinference_inc_dir ${INSTALL_INCLUDE_DIR})
set(mlinference_lib_dir ${INSTALL_LIBRARY_DIR})
set(mlinference_scripts_dir ${INSTALL_SCRIPTS_DIR})

# These are the enabled backends (if any)
set(enabled_backends "${_backends}")

configure_file(${SETUP_SCRIPT_IN} ${SETUP_SCRIPT_OUT})
