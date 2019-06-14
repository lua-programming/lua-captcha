local captchas = require 'captcha'
print("Version", captchas.VERSION)
local captcha = captchas.new()
captcha:setlength(5)
captcha:setfontsize(60)
captcha:setpath("/tmp")
captcha:setformat("png")
local path = captcha:generate()
print("Path:", path)
