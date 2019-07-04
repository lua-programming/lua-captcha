package = 'lua-captcha'
version = '0.1-3'
source = {
	url = 'git://github.com/lua-programming/lua-captcha.git',
	branch = '0.1-3'
}
description = {
	summary = 'Lua-Captcha v0.1-3 by Otgo',
	detailed = 'Create captcha images.',
	homepage = 'https://github.com/lua-programming/lua-captcha',
	license = 'MIT/X11'
}
dependencies = {
	'lua >= 5.1'
}
build = {
	type = 'builtin',
	modules = {
		captcha = {
			sources = {'src/captcha.c'},
			libraries = {'gd','m'}
		}
	},
	copy_directories = {'examples','fonts'}
}
