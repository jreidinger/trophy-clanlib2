dnl Process this file with autoconf to produce a configure script.


AC_INIT(src/catrophy.cpp)

AM_INIT_AUTOMAKE(trophy,1.2.0)



PKG_CHECK_EXISTS([clanApp-2.3 >= 2.3.0], [have_clan_23=yes])

AC_MSG_CHECKING(ClanLib version)
	if test "$have_clan_23" = "yes"; then
	    AC_MSG_RESULT(ClanLib 2.3)
	    AC_SUBST(CLANLIB_MODULES, ["clanGL-2.3 clanCore-2.3 clanSound-2.3 clanDisplay-2.3 clanApp-2.3"])
	else
	   AC_MSG_RESULT(no)
	   AC_MSG_ERROR([Unable to find a compatible version of ClanLib. Check for ClanLib 0.8 or ClanLib 1.0])
	fi
	


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
AC_CHECK_LIB(clan23Core, main, , echo "Trophy requires ClanLib to run."; exit 1, -lpthread)

AC_OUTPUT(Makefile src/Makefile resources/Makefile)

