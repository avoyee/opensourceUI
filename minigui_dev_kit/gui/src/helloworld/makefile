PWD=`pwd`
SDK_LIBS=/home/avoy/sstar/project/release/nvr/i2m/011A/glibc/8.2.1/lib/dynamic
SDK_LIBS_STATIC=$(patsubst %dynamic,%static,$(SDK_LIBS))
PLAT_DEPENDENT_LIB=-lmi_gfx -lmi_sys -lmi_common -ldl
PLAT_DEPENDENT_LIB_ST=$(filter-out -ldl,$(PLAT_DEPENDENT_LIB))
PLAT_DEPENDENT_LIB_EX= -lmi_disp -lmi_panel -lmi_hdmi 
MINIGUI_LIBS=-lmgncs -lmgutils -lmgeff -lmgplus -lminigui_procs -lfreetype -ljpeg -lpng -lts -lz -lmgncs4touch  -lchipmunk 
SDK_INCS=/home/avoy/sstar/project/release//include
LDFLAGS+=-L$(SDK_LIBS)  -L$(SDK_LIBS_STATIC)
LDFLAGS+=-L$(PWD)/../../library/lib -L/usr/local/lib
STDLIBS:=  -lstdc++ -ldl  -lpthread -lm 
LIBS:=-Wl,-Bstatic ${MINIGUI_LIBS} ${PLAT_DEPENDENT_LIB_EX} ${PLAT_DEPENDENT_LIB_ST} -Wl,-Bdynamic $(STDLIBS)
CFLAGS:=-I$(SDK_INCS) -I$(PWD)/../../library/include -I$(PWD)/../../library/include/ctrl -I/usr/local/include
CC:=arm-linux-gnueabihf-gcc

HELLO_SRC:=helloworld.c
HELLO:=helloworld
MGINIT_SRC:=mginit.c sstardisp.c
MGINIT:=mginit
.PHONY : all clean


all:
	$(CC) -o $(HELLO)  $(HELLO_SRC) $(CFLAGS) $(LDFLAGS) $(LIBS)
	$(CC) -o  $(MGINIT) $(MGINIT_SRC) $(CFLAGS) $(LDFLAGS) $(LIBS)
clean:
	rm -f $(patsubst %.c,%.o,$(HELLO_SRC))
	rm -f $(HELLO)
	rm -f $(patsubst %.c,%.o,$(MGINIT_SRC))
	rm -f $(MGINIT)
install:
	@echo done!
