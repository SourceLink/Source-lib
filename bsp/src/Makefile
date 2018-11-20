#CC = arm-linux-gcc

TARGET	=  $(patsubst %.c, %.o, $(wildcard *.c))

#TARGET +=  bsp_oled.o

CFLAGS	= -Wall -Os
CFLAGS	+= -fPIC

all: $(TARGET)
#-lm用的数据库
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ -I ../../lib/inc/sl_perpheral -I ../../lib/inc/so_core  -I ../inc/ -L ../../lib/src $< -lslhl -lm -Wall

.PHONY: clean 
clean:
	rm -f $(TARGET)
