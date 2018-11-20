CROSS_COMPILE ?=

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib

CFLAGS	= -Wall -Os
LDFLAGS	= -lpthread
LIB		= libslbsp.so
#SLIB	= libslbsp.a
CFLAGS	+= -fPIC

OBJDIR	= .obj
DEPDIR	= .deps

LIBDIR  = src

LIBSRC	+= $(wildcard $(LIBDIR)/*.c)

LIBOBJ	= $(patsubst %.c, $(OBJDIR)/%.o, $(notdir  $(LIBSRC)))
DEPS	= $(patsubst %.c, $(DEPDIR)/%.Po, $(notdir $(LIBSRC)))


SUBDIRS	= $(LIBDIR)
TARGET		= subdirs
CTARGET     = copyo
DTARGET     = mkpath
CLTARGET    = clearself
PHONY		+= $(TARGET) $(SUBDIRS) %.clean %.copyo


all: $(TARGET) $(LIB) $(SLIB) 

$(LIB): $(LIBOBJ)	
	$(CC) $(LDFLAGS) -shared -o $@ $^

$(SLIB): $(LIBOBJ)
	$(AR) rcu $@ $^
	$(RANLIB) $@
	
$(TARGET): $(SUBDIRS) $(DTARGET) $(CTARGET)
#推导出$(TARGET)所需要的依赖，就会层层调用下面这条语句
# 如：make -C lib make -C demo
copyo: $(patsubst %, %.copyo, $(SUBDIRS))

%.copyo:
	@(mv $(patsubst %.copyo, %, $@)/*.o $(OBJDIR))
		
$(SUBDIRS):
	make -C $@
	
$(DTARGET):
#	@test -d $(DEPDIR) || mkdir -p $(DEPDIR)
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)

#把SUBDIRS中的文件替换成.clean格式 如lib.clean
#而且这个文件作为clean的依赖，然后自动去推导得出这个依赖，如lib.clean
#自然而然的会去执行下面%.clean的语句
clean: $(patsubst %, %.clean, $(SUBDIRS)) $(CLTARGET)

#替换掉目标文件中的后缀是.clean的文件全都替换成%，如lib.clean目标则替换成 lib
#这条语句产生的目标是上一条语句的依赖
#%.clean是个伪目标，没有依赖关系，只用于执行语句
#能执行的原因是因为上面的clean需要依赖
%.clean:
	@(cd $(patsubst %.clean, %, $@) && make clean)

$(CLTARGET):
	rm -rf $(LIB) $(LIBOBJ) $(SLIB) $(OBJDIR)
	
install: $(LIB)
#	install ./lib/inc/libslhl*.h /usr/local/include
	install --mode=644 $(LIB) /usr/local/lib
	install --mode=644 $(SLIB)  /usr/local/lib
	ldconfig
	
#声明伪目标
.PHONY: $(PHONY) clean 

