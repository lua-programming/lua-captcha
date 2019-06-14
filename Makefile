CC:=gcc
LUA_VERSION:=5.2
CFLAGS:=-lgd -fpic -shared -llua$(LUA_VERSION) -I/usr/include/lua$(LUA_VERSION) -Wall -lcaptcha
BUILD:=*.c
OBJ:=captcha.so
all:
	@$(CC) $(BUILD) -o $(OBJ) $(CFLAGS)
	$(info ************  build success ************)
