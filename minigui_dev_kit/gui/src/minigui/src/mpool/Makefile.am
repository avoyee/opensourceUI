# also for makefile.ng and makefile.msvc
AM_CPPFLAGS = -I$(abs_top_srcdir)/src/include -I$(abs_top_srcdir)/include

noinst_LTLIBRARIES = libmpool.la

EXTRA_DIST=mplite.c 

libmpool_la_SOURCES = mplite.c mpool.c

# for makefile.ng and makefile.msvc
LIB_NAME=libmpool
SRC_FILES=$(libmpool_la_SOURCES)
