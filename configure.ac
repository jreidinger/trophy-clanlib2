dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/catrophy.cpp)

AM_INIT_AUTOMAKE(trophy,1.1.5)

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

# We check for main in clanSignals :
# AC_CHECK_LIB try to compile and link with the library (first argument) a file
# containing just a definition of and a call to the function (second argument).
# We have no function to call in ClanLib (only member functions that would need 
# to define more than just the function itself) so we call main (recursively).
# We try to link to clanSignals because this is the only library file that
# doesn't need another one to be linked with.
AC_CHECK_LIB(clanSignals, main, , echo "Trophy requires ClanLib to run."; exit 1)

AC_OUTPUT(Makefile src/Makefile resources/Makefile)
