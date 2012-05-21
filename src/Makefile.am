## Process this file with automake to produce Makefile.in

INCLUDES = \
	-I/usr/include \
	@GTK_CFLAGS@ \
	@XMMS_CFLAGS@

bin_PROGRAMS = xmms_gtk_playlist

xmms_gtk_playlist_SOURCES = \
	main.c init.c.h \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	globals.c globals.h \
	worker.c worker.h

xmms_gtk_playlist_LDADD = @GTK_LIBS@

xmms_gtk_playlist_LDADD += @XMMS_LIBS@

libdir = $(XMMS_GENERAL_PLUGIN_DIR)

lib_LTLIBRARIES     = libgtkplaylist.la

libgtkplaylist_la_SOURCES = \
	gtkplaylist.c init.c.h \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	globals.c globals.h \
	worker.c worker.h

libgtkplaylist_la_LIBADD = @GTK_LIBS@

libgtkplaylist_la_LIBADD += @XMMS_LIBS@
