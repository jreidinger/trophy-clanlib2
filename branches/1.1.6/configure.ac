dnl Process this file with autoconf to produce a configure script.


AC_INIT(src/catrophy.cpp)

AM_INIT_AUTOMAKE(trophy,1.1.7)



PKG_CHECK_EXISTS([clanApp-1.0 >= 1.0.0], [have_clan_10=yes])
PKG_CHECK_EXISTS([clanApp-0.8 >= 0.8.0], [have_clan_08=yes])

AC_MSG_CHECKING(ClanLib version)
	if test "$have_clan_10" = "yes"; then
	    AC_MSG_RESULT(ClanLib 1.0)
	    AC_SUBST(CLANLIB_MODULES, ["clanSignals-1.0 clanGL-1.0 clanCore-1.0 clanSound-1.0 clanDisplay-1.0 clanApp-1.0"])
	else
        if test "$have_clan_08" = "yes"; then
	    AC_MSG_RESULT(ClanLib 0.8)
	    AC_SUBST(CLANLIB_MODULES, ["clanSignals-0.8 clanGL-0.8 clanCore-0.8 clanSound-0.8 clanDisplay-0.8 clanApp-0.8"])
	else
	   AC_MSG_RESULT(no)
	   AC_MSG_ERROR([Unable to find a compatible version of ClanLib. Check for ClanLib 0.8 or ClanLib 1.0])
	fi
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
AC_CHECK_LIB(clanSignals, main, , echo "Trophy requires ClanLib to run."; exit 1)

AC_OUTPUT(Makefile src/Makefile resources/Makefile)

