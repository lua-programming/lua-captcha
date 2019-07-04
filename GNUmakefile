CC?=c89
# you can set your lua version here
# 0 == will get your lua version by 'lua' binary
ifeq ($(LUA_VERSION),0)
	LUA_VERSION=$(shell /usr/bin/env lua$(LUA_VERSION) -e "print(({_VERSION:gsub('^%S+%s', '')})[1])")
endif
BUILDDIR?=./src
CFLAGS?=-lgd -fpic -shared -llua$(LUA_VERSION) -I/usr/include/lua$(LUA_VERSION) -Wall
BUILD?=$(BUILDDIR)/*.c
OBJ?=captcha.so
captcha_install:
	$(info Compiling for Lua ${LUA_VERSION}...)
	@ $(CC) $(BUILD) -o $(OBJ) $(CFLAGS)
