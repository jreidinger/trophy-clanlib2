bin_PROGRAMS = trophy
trophy_SOURCES = cagoody.cpp cainfodialog.cpp camenu.h canetserver.cpp capositiontable.h casoundeffect.cpp \
		 cagoody.h cainfodialog.h camenuinput.cpp canetserver.h capositiontableview.cpp casoundeffect.h \
		 cagoodytype.h calabel.cpp camenuinput.h capanel.cpp capositiontableview.h caconfigurekey.cpp\
		 cartype.h caimagehueselector.cpp calabel.h camenuitem.cpp camenuselect.cpp capanel.h caresources.cpp caconfigurekey.h\
		 utils/hsvcolor.h caimagehueselector.h calineedit.cpp camenuitem.h player.cpp caresources.h track.h \
		 cacredits.cpp caimagemanipulation.cpp calineedit.h camenulabel.cpp player.h cascreen.cpp catrophy.cpp \
		 cacredits.h caimagemanipulation.h caloadingscreen.cpp camenulabel.h caplayersettingsdialog.cpp cascreen.h cachampionshipscreen.h \
		 catrophy.h cadialog.cpp caimageselector.cpp caloadingscreen.h caplayersettingsdialog.h cachampionshipscreen.cpp\
		 casignupscreen.cpp cawidget.cpp cadialog.h caimageselector.h utils/trophymath.cpp camenuselect.h caplayerview.cpp \
		 casignupscreen.h cawidget.h cadustclowd.h caimageview.cpp utils/trophymath.h canetclient.cpp caplayerview.h \
		 caslotselectiondialog.cpp cafogbomb.h caimageview.h camenu.cpp canetclient.h capositiontable.cpp \
		 caslotselectiondialog.h humanplayer.h humanplayer.cpp computerplayer.cpp computerplayer.h

CLANLIB_MODULES = clanSignals-0.8 clanGL-0.8 clanCore-0.8 clanSound-0.8 clanDisplay-0.8 clanApp-0.8
AM_LDFLAGS = `pkg-config --libs $(CLANLIB_MODULES)`
INCLUDES = `pkg-config --cflags $(CLANLIB_MODULES)`
AM_CPPFLAGS = -DPKGDATADIR=\"$(pkgdatadir)\"
AM_CXXFLAGS = -Wall
