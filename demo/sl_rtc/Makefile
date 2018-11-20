#CC = arm-linux-gcc 

TARGET	=  $(patsubst Source-%.c, source-%, $(wildcard *.c))
SRC 	=  $(wildcard *.c)
SRC 	+= $(wildcard *.h)

#-lm用的数学库 lslhl lslbsp用的我们自己编译出来的库
$(TARGET):$(SRC)
	$(CC) -o $@ -I ../../bsp/inc -I ../../lib/inc/so_core -I ../../lib/inc/sl_perpheral -L ../../bsp -L ../../lib $^ -lslbsp -lslhl -lm -Wall

.PHONY: clean 
clean:
	rm -f $(TARGET)
