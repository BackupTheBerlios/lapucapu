#!/bin/bash
CXXFLAGS="\
	-pthread \
	-Wall \
	"
INCLUDES="\
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	 -I/usr/include/glib-2.0 \
	 -I/usr/lib/glib-2.0/include \
	 -I/usr/lib/gtk-2.0/include \
	 -I/usr/lib/gdkmm-2.4/include \
	 -I/usr/include/cairo \
	 -I/usr/lib/glibmm-2.4/include \
	 -I/usr/lib/gtkmm-2.4/include \
	 -I/usr/include/glibmm-2.4 \
	 -I/usr/include/libglademm-2.4 \
	 -I/usr/include/gtkmm-2.4 \
	 -I/usr/include/pangomm-1.4 \
	 -I/usr/include/pango-1.0 \
	 -I/usr/include/atkmm-1.6 \
	 -I/usr/include/gdkmm-2.4 \
	 -I/usr/include/gtk-2.0 \
	 -I/usr/include/gdkmm-2.4 \
	 -I/home/czysty/mysql++-2.1.1/lib \
	 -I/usr/include/mysql \
	 -I/usr/include/libxml++-2.6 \
	 -I/usr/lib/sigc++-2.0/include \
	 -I/usr/include/atk-1.0 \
	 -I/usr/include/sigc++-2.0 \
	 -I./klient
	 "

SOURCES="\
	main.cc\
	BazaDanych.cpp\
	BazaDanych.h\
	LapucapuException.cpp\
	LapucapuException.h\
	ObslugaKlienta.cpp\
	ObslugaKlienta.h\
	OnlineUsersList.cpp\
	OnlineUsersList.h\
	Package.cpp\
	Package.h\
	ThreadPool.cc\
	ThreadPool.h\
	User.cpp\
	User.h\
	./klient/xml_explode.cpp\
	./klient/xml_explode.h \
	./klient/user.cpp \
	./klient/user.h \
	./klient/messages.h \
	./klient/messages.cpp \
	./klient/mytypes.h \
	./klient/users_list.h \
	./klient/users_list.cpp \
	"

LDFLAGS="\
	-L/usr/lib \
	-L/home/czysty/mysql++-2.1.1/lib\
	-L./klient \
	"

LIBS="\
	-pthread \
	-lgthread-2.0 \
	-lglib-2.0 \
	-lm \
	-lmysqlpp \
	-lglibmm-2.4 \
	-lxml++-2.6"

g++ -o lapucapud $CFLAGS $INCLUDES $LDFLAGS $LIBS $SOURCES
