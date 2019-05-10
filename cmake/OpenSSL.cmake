function(_build_and_install_openssl openssl_install_dir openssl_vendor_dir)
  set(FETCH_MINIMUM_PERL_VERSION 5.13.4)

  find_package(Perl REQUIRED)
  if (PERL_VERSION_STRING VERSION_LESS FETCH_MINIMUM_PERL_VERSION)
    message(FATAL_ERROR "Perl version ${PERL_VERSION_STRING} found; minimum required version is ${FETCH_MINIMUM_PERL_VERSION}")
  endif()

  if (WIN32)
    find_program(FETCH_NASM nasm)
    if (${FETCH_NASM} STREQUAL FETCH_NASM-NOTFOUND)
      message(FATAL_ERROR "NASM not found in PATH")
    endif()

    find_program(FETCH_NMAKE nmake)
    if (${FETCH_NMAKE} STREQUAL FETCH_NMAKE-NOTFOUND)
      message(FATAL_ERROR "nmake not found in PATH (it comes with Visual Studio)")
    endif()
  else()
    find_program(FETCH_MAKE make)
    if (${FETCH_MAKE} STREQUAL FETCH_MAKE-NOTFOUND)
      message(FATAL_ERROR "make not found")
    endif()
  endif()

  set(FETCH_OPENSSL_BUILD_WORKDIR ${openssl_vendor_dir}/build)
  file(MAKE_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR})

  if(WIN32)
    set(FETCH_OPENSSL_BUILD_CONF fetchai_win64)
  elseif(APPLE)
    set(FETCH_OPENSSL_BUILD_CONF fetchai_mac64)
  else()
    set(FETCH_OPENSSL_BUILD_CONF fetchai_linux64)
  endif()

  execute_process(
    COMMAND ${PERL_EXECUTABLE} ${openssl_vendor_dir}/src/Configure ${FETCH_OPENSSL_BUILD_CONF}
      --config=${FETCH_ROOT_VENDOR_AUX_DIR}/openssl/fetchai_openssl.conf
      --prefix=${openssl_install_dir}
      --openssldir=${openssl_vendor_dir}/dist/config
    WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
    RESULT_VARIABLE OPENSSL_BUILD_STATUS
  )
  execute_process(
    COMMAND ${PERL_EXECUTABLE} configdata.pm --dump
    WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
    RESULT_VARIABLE OPENSSL_BUILD_STATUS
  )

  if(WIN32)
    execute_process(
      COMMAND nmake
      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
      RESULT_VARIABLE OPENSSL_BUILD_STATUS
    )
    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
      message(FATAL_ERROR "OpenSSL build failure: nmake")
    endif()

#    execute_process(
#      COMMAND nmake test
#      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
#      RESULT_VARIABLE OPENSSL_BUILD_STATUS
#    )
#    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
#      message(FATAL_ERROR "OpenSSL build failure: nmake test")
#    endif()

    execute_process(
      COMMAND nmake install
      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
      RESULT_VARIABLE OPENSSL_BUILD_STATUS
    )
    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
      message(FATAL_ERROR "OpenSSL build failure: nmake install")
    endif()
  else()
    execute_process(
      COMMAND make -j
      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
      RESULT_VARIABLE OPENSSL_BUILD_STATUS
    )
    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
      message(FATAL_ERROR "OpenSSL build failure: make -j")
    endif()

#    execute_process(
#      COMMAND make test
#      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
#      RESULT_VARIABLE OPENSSL_BUILD_STATUS
#    )
#    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
#      message(FATAL_ERROR "OpenSSL build failure: make test")
#    endif()

    execute_process(
      COMMAND make install
      WORKING_DIRECTORY ${FETCH_OPENSSL_BUILD_WORKDIR}
      RESULT_VARIABLE OPENSSL_BUILD_STATUS
    )
    if(NOT OPENSSL_BUILD_STATUS EQUAL 0)
      message(FATAL_ERROR "OpenSSL build failure: make install")
    endif()
  endif()
endfunction(_build_and_install_openssl)

function(configure_openssl)
#  set(FETCH_OPENSSL_VERSION 1.1.1c)

  # Configure OpenSSL library lookup
  set(OPENSSL_ROOT_DIR "C:/openssl")
#  set(OPENSSL_ROOT_DIR ${FETCH_ROOT_VENDOR_DIR}/openssl/dist/openssl)

#  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
#  unset(OPENSSL_INCLUDE_DIR CACHE)
#  unset(OPENSSL_SSL_LIBRARY CACHE)
#  find_package(OpenSSL)
#  if(NOT (OPENSSL_FOUND AND (OPENSSL_VERSION STREQUAL FETCH_OPENSSL_VERSION)))
#    message("OpenSSL not found or unexpected version: building from source")
#    _build_and_install_openssl(
#      ${OPENSSL_ROOT_DIR}
#      ${FETCH_ROOT_VENDOR_DIR}/openssl
#    )
#  endif()

  # Confirm successful installation and version correctness
#  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
#  unset(OPENSSL_INCLUDE_DIR CACHE)
#  unset(OPENSSL_SSL_LIBRARY CACHE)
  find_package(OpenSSL REQUIRED)
#  if(NOT (OPENSSL_VERSION STREQUAL FETCH_OPENSSL_VERSION))
#    message(FATAL_ERROR "Unexpected version of OpenSSL: ${OPENSSL_VERSION} instead of ${FETCH_OPENSSL_VERSION}")
#  endif()

  if(FETCH_VERBOSE_CMAKE)
    message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
    if(OPENSSL_USE_STATIC_LIBS)
      message(STATUS "OpenSSL linking: Static")
    else()
      message(STATUS "OpenSSL linking: Dynamic")
    endif()
  endif(FETCH_VERBOSE_CMAKE)

  add_library(vendor-openssl INTERFACE)
  target_link_libraries(vendor-openssl INTERFACE ${OPENSSL_LIBRARIES})

  if(OPENSSL_USE_STATIC_LIBS)
    target_link_libraries(vendor-openssl INTERFACE ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
  endif(OPENSSL_USE_STATIC_LIBS)

  target_include_directories(vendor-openssl INTERFACE ${OPENSSL_INCLUDE_DIR})
endfunction(configure_openssl)
