# Configure the setup script

set(mlinference_inc_dir ${INSTALL_INCLUDE_DIR})
set(mlinference_lib_dir ${INSTALL_LIBRARY_DIR})
set(mlinference_scripts_dir ${INSTALL_SCRIPTS_DIR})

# These are the enabled backends (if any)
set(enabled_backends "\"${_backends}\"")

string(REPLACE ";" ":" _conc_shared_library_paths "${_configure_shared_library_paths}")
string(REPLACE ";" ":" _conc_static_library_paths "${_configure_static_library_paths}")
set(shared_lib_paths "${_conc_shared_library_paths}")
set(static_lib_paths "${_conc_static_library_paths}")

configure_file(${SETUP_SCRIPT_IN} ${SETUP_SCRIPT_OUT})
