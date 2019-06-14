package = 'captcha'
version = '0.1-1'
source = {
    url = "git://github.com/lua-programming/lua-captcha.git",
    branch="master"
}
description = {
	summary = 'xxxxx',
	detailed = "xxxxx",
	homepage = 'https://github.com/lua-programming/lua-captcha',
	license = 'MIT/X11'
}
dependencies = {
	'lua >= 5.2'
}
build = {
	type = 'builtin',
    modules = {
        captcha = {
            sources = {
                "captcha.c"
            },
            libraries = {"gd"}
        }

    }
}
