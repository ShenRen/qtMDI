######################################################################
# Automatically generated by qmake (2.01a) Thu Nov 8 17:03:03 2012
######################################################################

TEMPLATE = app
TARGET = qtMDI

QT += core gui widgets

QMAKE_CXXFLAGS += -D_FORTIFY_SOURCE=0 -D_GNU_SOURCE -DULAPI -mhard-float -fno-fast-math -mieee-fp -fno-unsafe-math-optimizations -Wframe-larger-than=2560 -Os -std=c++0x
INCLUDEPATH += . ../include /usr/include usr/include/linux /opt/Qt/5.4/gcc_64/include
QMAKE_LIBDIR += /lib /usr/lib /usr/local/lib/machinekit
LIBS += -llinuxcncini -llinuxcnchal -lnml -llinuxcnc -lposemath

DEPENDPATH += .

# Input

FORMS += axis.ui touch-off_dlg.ui recent_files_dialog.ui 

HEADERS += qtMDI.h \
	   touch-off_dlg.h \
	   recent_files_dialog.h \
	   code_editor.h \
	   comment_highlighter.h \
	   HAL_Access_Slim.h \
	   NML_Access_Slim.h
	   

SOURCES += main.cpp \
	   qtMDI.cpp \
	   touch-off_dlg.cpp \
	   qtMDI_files.cpp \
	   qtMDI_funcs.cpp \
	   recent_files_dialog.cpp \
	   code_editor.cpp \
	   comment_highlighter.cpp \
	   HAL_Access_Slim.cpp \
	   HAL_Access_Slim_Commands.cpp \	   
	   HAL_Access_Slim_Local.cpp \
	   NML_Access_Slim.cpp

RESOURCES += qtMDI.qrc
