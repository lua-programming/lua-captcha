#!/usr/bin/env lua
local captchas = require 'captcha' -- uses LuaRocks module
print("Version", captchas.VERSION) -- get version
local captcha = captchas.new() -- create a new metatable, to create a new captcha
captcha:setlength(5)
captcha:setfontsize(60)
captcha:setpath("/tmp")
captcha:setformat("jpg")
captcha:setfontsfolder("../fonts") -- get fonts from previous dir
local path = captcha:generate() -- generates
local text = path:match(".+/(%S+)%.%S+") -- gets filename
print("Got it!")
print("Path", ('"%s"'):format(path))
print("Captcha", ('"%s"'):format(text))
