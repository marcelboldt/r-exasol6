# Process this file with autoconf to produce a configure script.
AC_INIT(exasol6, 0.1, marcel.boldt@exasol.com)

m4_include([M4/m4-ax_require_defined.m4])

AC_LANG(C++)
AC_PROG_CXX

m4_include([M4/m4-ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX([11],,)

# Checks for OpenSSL and fail if not found
m4_include([M4/m4-ax_check_openssl.m4])
AX_CHECK_OPENSSL(, AC_MSG_FAILURE([OpenSSL is missing]))
LIBS="$LIBS ${OPENSSL_LIBS}"

AC_CHECK_HEADERS([string.h unistd.h limits.h sys/socket.h arpa/inet.h], [],
            [AC_MSG_FAILURE([a necessary header file is missing])])

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
