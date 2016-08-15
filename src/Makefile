# 可执行文件
TARGET=libuev_test
# 依赖目标
SRCS=main.c timer.c uv.c watch.c

# 目标文件
OBJS = $(SRCS:.c=.o)

# 指令编译器和选项
CC=gcc
CFLAGS=-Wall -std=gnu99

$(TARGET):$(OBJS)
	$(CC) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJS)

# 连续动作，先清除再编译链接，最后执行
exec:clean $(TARGET)
	@echo 开始执行
	sudo ./$(TARGET)
	@echo 执行结束

# 编译规则 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<