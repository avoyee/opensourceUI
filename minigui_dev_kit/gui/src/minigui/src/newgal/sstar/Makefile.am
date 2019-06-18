## Makefile.am for MiniGUI using the framebuffer console video driver

AM_CPPFLAGS = -I$(abs_top_srcdir)/src/include -I$(abs_top_srcdir)/include \
			            -I$(abs_top_srcdir)/src/newgal/

noinst_LTLIBRARIES = libvideo_sstar.la
libvideo_sstar_la_SOURCES = $(SSTAR_SRCS)

SSTAR_SRCS = fbsstar.c fbsstar.h fbsstaraccel.c fbsstaraccel.h \
			 fbsstaraccel_gfx.c fbsstaraccel_gfx.h fbsstaraccel_dma.c \
			 fbsstaraccel_dma.h compiler.h