CC = $(CROSS_COMPILE)gcc

TARGET	=  $(patsubst %.c, %.o, $(wildcard *.c))
#SRC 	=  $(wildcard *.c)
#TARGET  =  so_file.o
#TARGET +=  so_lib.o

CFLAGS += -fPIC
all: $(TARGET)
#-lm用的数据库
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ -I ../../inc/sl_perpheral/ -I ../../inc/so_core/ $< -lm -Wall

.PHONY: clean 
clean:
	rm -f $(TARGET)
