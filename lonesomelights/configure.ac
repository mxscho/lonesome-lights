#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lonesomelights], [0.0.0.1], [maximilian.scholze@fau.de patrick.becker@fau.de])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([src/server_main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile res/Makefile src/Makefile])

# Checks for programs.
AC_PROG_CXX

# Checks for pkg-configs.
PKG_CHECK_MODULES(SFML, sfml-all >= 2.1)

# Checks for header files.
AC_CHECK_HEADERS([GL/glew.h])
AC_CHECK_HEADERS([glm/glm.hpp])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_LIB([GL], [main])
AC_CHECK_LIB([GLEW], [main])

AC_OUTPUT
