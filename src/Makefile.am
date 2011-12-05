bin_PROGRAMS = trophy
trophy_SOURCES = cagoody.cpp cainfodialog.cpp caescapedialog.cpp camenu.h canetserver.cpp capositiontable.h casoundeffect.cpp \
		 cagoody.h cainfodialog.h caescapedialog.h camenuinput.cpp canetserver.h capositiontableview.cpp casoundeffect.h shopscreen.h \
		 cagoodytype.h cagoodytype.cpp calabel.cpp camenuinput.h capanel.cpp capositiontableview.h caconfigurekey.cpp shopscreen.cpp \
		 cartype.h cartype.cpp caimagehueselector.cpp calabel.h camenuitem.cpp camenuselect.cpp capanel.h caresources.cpp caconfigurekey.h \
		 utils/hsvcolor.h caimagehueselector.h calineedit.cpp camenuitem.h player.cpp caresources.h track.h track.cpp guibox.h guibox.cpp \
		 cacredits.cpp caimagemanipulation.cpp calineedit.h camenulabel.cpp player.h cascreen.cpp catrophy.cpp cacarupgrades.h cacarupgrades.cpp \
		 cacredits.h caimagemanipulation.h caloadingscreen.cpp camenulabel.h caplayersettingsdialog.cpp cascreen.h cachampionshipscreen.h \
		 catrophy.h cadialog.cpp caimageselector.cpp caloadingscreen.h caplayersettingsdialog.h cachampionshipscreen.cpp \
		 casignupscreen.cpp cawidget.cpp cadialog.h caimageselector.h utils/trophymath.cpp camenuselect.h caplayerview.cpp \
		 casignupscreen.h cawidget.h cadustclowd.h caimageview.cpp utils/trophymath.h canetclient.cpp caplayerview.h \
		 caslotselectiondialog.cpp cafogbomb.h caimageview.h camenu.cpp canetclient.h capositiontable.cpp \
		 caslotselectiondialog.h humanplayer.h humanplayer.cpp computerplayer.cpp computerplayer.h \
		 upgradespanel.cpp upgradespanel.h windialog.h windialog.cpp


AM_LDFLAGS = `pkg-config --libs $(CLANLIB_MODULES)` -lpthread
INCLUDES = `pkg-config --cflags $(CLANLIB_MODULES)`
AM_CPPFLAGS = -DPKGDATADIR=\"$(pkgdatadir)\"
AM_CXXFLAGS = -Wall
