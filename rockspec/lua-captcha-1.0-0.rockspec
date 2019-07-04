package = 'lua-captcha'
version = '1.0-0'
source = {
	url = 'git://github.com/lua-programming/lua-captcha.git',
	branch = 'master'
}
description = {
	summary = 'Lua-Captcha v1.0-0 by Otgo',
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
