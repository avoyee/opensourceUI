AM_CPPFLAGS = -I$(abs_top_srcdir)/src/include -I$(abs_top_srcdir)/include

noinst_LTLIBRARIES=libnative.la

EXTRA_DIST=README

MOUSE_SRC= mou_gpm.c mou_ps2.c mou_ms3.c mou_ms.c mou_imps2.c touchscreen.c tslib.c  \
	ugpi_input.c ugpi_input.h uhid_input.c uhid_input.h hashtable.c hashtable.h murmur.c murmur.h hashfunc.h 
CFLAGS+= -D__WITH_MURMUR

libnative_la_SOURCES=native.c native.h vtswitch-lite.c kbd_tty.c kbd_event.c  $(MOUSE_SRC)
