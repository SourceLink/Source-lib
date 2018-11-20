
#说明下面当前目录下的哪些子目录需要编译,多个目录用空格分开
#手动增加需要编译目录
SUBDIRS		+= sl_oled
SUBDIRS		+= sl_uart
SUBDIRS		+= sl_mpu6050
SUBDIRS		+= sl_can
SUBDIRS		+= sl_rtc

TARGET		= subdirs
PHONY		 += $(TARGET) $(SUBDIRS) %.clean

#all缺失依赖$(TARGET),则去依这个依赖为目标去推到；
#$(TARGET)缺失依赖$(SUBDIRS)，则去以这个依赖为目标去推导，实质为了执行make -C语句
all: $(TARGET)
#推导出all所需要的依赖，则会执行下面的语句
$(TARGET): $(SUBDIRS)
#推导出$(TARGET)所需要的依赖，就会层层调用下面这条语句
# 如：make -C lib make -C demo
$(SUBDIRS):
	make -C $@


#把SUBDIRS中的文件替换成.clean格式 如lib.clean
#而且这个文件作为clean的依赖，然后自动去推导得出这个依赖，如lib.clean
#自然而然的会去执行下面%.clean的语句
clean: $(patsubst %, %.clean, $(SUBDIRS))

#替换掉目标文件中的后缀是.clean的文件全都替换成%，如lib.clean目标则替换成 lib
#这条语句产生的目标是上一条语句的依赖
#%.clean是个伪目标，没有依赖关系，只用于执行语句
#能执行的原因是因为上面的clean需要依赖
%.clean:
	@(cd $(patsubst %.clean, %, $@) && make clean)


install:
#	make -C ./demo install
	make -C ./lib/src install


#声明伪目标
.PHONY: $(PHONY) clean
