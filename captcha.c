// if we're using g++ compiler
#ifdef __cplusplus
extern "C" {
    #endif
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
    #include <gd.h>
    #include <time.h>
    #include "captcha.h"
    static int generate(lua_State* L) {
        const char *usr_path=NULL, *usr_fmt = NULL; // set user path and user format to NULL
        int usr_fntsize = 0, usr_length = 0; // set sizes to 0 (false)
        luaL_checktype(L, 1, LUA_TTABLE); // check if arg1 is a table
        luaL_getmetatable(L, "CAPTCHA"); // get metatable
        lua_pushnil(L);               // Push nil
        while(lua_next(L, 1) != 0) { // goes element by element in metatable
            const char *var = lua_tostring(L, -2); // get variable metatable
            const char *val = lua_tostring(L, -1); // get value metatable
            if(!strcmp(var, "__path")) usr_path = val;
            if(!strcmp(var, "__format")) usr_fmt = val;
            else if(!strcmp(var, "__length")) usr_length = atoi(val);
            else if(!strcmp(var, "__fntsize")) usr_fntsize = atoi(val); // set usr settings
            lua_pop(L, 1); // Pop value
        }
        lua_pop(L, 1); // pop tab, continue code
        if (usr_path==NULL) {
            usr_path = "./"; // default path
        }
        if (usr_length==0) {
            usr_length = 5; // default length
        }
        if (usr_fntsize==0) {
            usr_fntsize = 60; // default font size
        }
        if (usr_fmt==NULL) {
            usr_fmt = "png"; // default format
        }
        char *path = (char*)lua_newuserdata(L, strlen(usr_path)+usr_length); // works like malloc, adding dat to gc
        lua_pop(L, 1); // continues
        sprintf(path, "%s/", usr_path);
        FILE *out;
        int x_image = usr_fntsize*usr_length; // calcule width image, using length * font size
        int y_image = usr_fntsize*2; // calcule height image, using font size * 2
        char letter[2]; // set letters string with 2 bytes
        int i, y, x;
        srand(time(NULL)); /* starts srand with os time */
        gdImagePtr im = gdImageCreateTrueColor (x_image, y_image); // creates image
        /* Background color for gfImageCreateTrueColor */
        gdImageFilledRectangle (im, 0, 0, gdImageSX (im), gdImageSY (im), gdImageColorAllocate(im, 255, 255, 255));
        /* gdImageCreate, will be first allocated */
        char *fonts[13] = {
            "./fonts/FreeMonoBoldOblique.ttf", "./fonts/FreeMonoBold.ttf", "./fonts/FreeMonoOblique.ttf",
            "./fonts/FreeMono.ttf", "./fonts/FreeSansBoldOblique.ttf", "./fonts/FreeSansBold.ttf",
            "./fonts/FreeSansOblique.ttf", "./fonts/FreeSans.ttf", "./fonts/FreeSerifBoldItalic.ttf",
            "./fonts/FreeSerifBold.ttf", "./fonts/FreeSerifItalic.ttf", "./fonts/FreeSerif.ttf"
        };
        y = y_image/2; // set first letter in the center
        x = 0; // set first letter at the start (0xY)
        int random_color = gdImageColorAllocate(im, rand()%200, rand()%200, rand()%200); // generate a random color, used by text
        for (i = 0; i<=usr_length; i++) {
            char rdm_letter = troc(t_w[rand()%48]);
            sprintf(path, "%s%c", path, rdm_letter); // add char on path, to get final filename
            sprintf(letter, "%c", rdm_letter); // converts char to char *, gd needs it
            char *error = gdImageStringTTF(
                im, // im	The image to draw onto.
                NULL, //brect	The bounding rectangle as array of 8 integers where each pair represents the x- and y-coordinate of a point.  The points specify the lower left, lower right, upper right and upper left corner.
                random_color, // fg	The font color
                fonts[rand()%12], // fontlist	The semicolon delimited list of font filenames to look for.
                usr_fntsize, // ptsize	The height of the font in typographical points (pt).
                rand()%1, // angle	The angle in radian to rotate the font counter-clockwise.
                x, // x	The x-coordinate of the basepoint (roughly the lower left corner) of the first letter.
                rand()%y+y_image/2, // y	The y-coordinate of the basepoint (roughly the lower left corner) of the first letter.
                letter // string	The string to render.
            );
            if (error) luaL_argerror (L, 0, error); // show error
            x+=usr_fntsize; // sum x + font size, creates and space between each letter
        }
        for (i=0; i<(y_image*x_image)/2; i++) {
            int random_x = rand()%x_image;
            int random_y = rand()%y_image;
            gdImageSetPixel(im, random_x, random_y, random_color-4000); // generate another "random" color subtracting 4000
        }
        sprintf(path, "%s.%s", path, usr_fmt);
        out = fopen (path, "wb");
        gdImagePng (im, out);
        fclose (out);
        gdImageDestroy(im);
        lua_pushstring(L, path);
        return 1;
    }
    static int setlength(lua_State* L) {
        lua_Number xsize = luaL_checknumber(L, 2);
        lua_pushnumber(L, xsize); // push length into table
        lua_setfield(L, -3, "__length"); // works by gettings metatable (-3)
        luaL_getmetatable(L, "CAPTCHA"); // get captcha table
        lua_setmetatable(L, -2);
        return 1;
    }
    static int setfontsize(lua_State* L) {
        lua_Number xsize = luaL_checknumber(L, 2);
        lua_pushnumber(L, xsize); // push fontsize into table
        lua_setfield(L, -3, "__fntsize"); // works by gettings metatable (-3)
        luaL_getmetatable(L, "CAPTCHA");
        lua_setmetatable(L, -2);
        return 1;
    }
    static int setpath(lua_State* L) {
        const char *path = luaL_checkstring(L, 2);
        lua_pushstring(L, path); // push path into table
        lua_setfield(L, -3, "__path"); // works by gettings metatable (-3)
        luaL_getmetatable(L, "CAPTCHA");
        lua_setmetatable(L, -2);
        return 1;
    }
    static int setformat(lua_State* L) {
        const char *format = luaL_checkstring(L, 2);
        lua_pushstring(L, format); // push format into table
        lua_setfield(L, -3, "__format"); // works by gettings metatable (-3)
        luaL_getmetatable(L, "CAPTCHA");
        lua_setmetatable(L, -2);
        return 1;
    }
    static int L_new(lua_State *L) {
        static const struct luaL_Reg L_methods[] = {
            {"generate", &generate},
            {"setlength", &setlength},
            {"setfontsize", &setfontsize},
            {"setpath", &setpath},
            {"setformat", &setformat},
            {NULL, NULL}
        };
        luaL_newmetatable(L, "CAPTCHA");
        lua_setmetatable(L, -1);
        luaL_newlibtable(L, L_methods); // adds struct (those are the commands), only creates the table
        luaL_setfuncs(L, L_methods, 0);  // register methods struct
        lua_pushvalue(L, -1); // push the metatable
        lua_setfield(L, -1,"__index"); // adds metatable to index
        //printf("%i\n", LUA_VERSION_NUM ); // testing, lua version
        return 1;
    }
    int luaopen_captcha(lua_State* L) {
        static const struct luaL_Reg L_register[] = {
            {"new", &L_new}, // creates a new beginning to make a captcha, adding methods and mt table
            {NULL, NULL}
        };
        luaL_newlib(L, L_register); // adds L_register struct
        luaL_setfuncs(L, L_register, 0); // register L_register struct
        lua_pushstring(L, LUA_CAPTCHA_VERSION); // add version value
        lua_setfield(L, -2, "VERSION"); // add to version variable
        return 1;
    }
    #ifdef __cplusplus
}
#endif
