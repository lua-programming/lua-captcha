#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <gd.h>
#include <time.h>
#include <dirent.h>
#include "captcha.h"
static int generate(lua_State* L) {
    int i, y, x, elements = 0;
    char *fonts[21];
    struct dirent *de;
    const char *usr_path="./", *usr_fmt = "png", *usr_ffonts="./"; // set user path and user format to NULL
    char *path;
    int usr_fntsize = 60, usr_length = 5; // set sizes to 0 (false)
    luaL_checktype(L, 1, LUA_TTABLE); // check if arg1 is a table
    lua_getmetatable(L, 1); // get metatable
    lua_pushnil(L);               // Push nil
    while(lua_next(L, 1) != 0) { // goes element by element in metatable
        const char *var = lua_tostring(L, -2); // get variable metatable
        const char *val = lua_tostring(L, -1); // get value metatable
        if(!strcmp(var, "__path")) usr_path = val;
        if(!strcmp(var, "__format")) usr_fmt = val;
        else if(!strcmp(var, "__length")) usr_length = atoi(val);
        else if(!strcmp(var, "__fntsize")) usr_fntsize = atoi(val); // set usr settings
        else if(!strcmp(var, "__ffolder")) usr_ffonts =val; // set usr settings
        lua_pop(L, 1); // Pop value
    }
    lua_pop(L, 1); // pop tab, continue code
    DIR *dr = opendir(usr_ffonts);
    if (dr == NULL) {
        luaL_argerror (L, 0, "Could not open fonts directory");
        return 0;
    }
    char tmp[1024]; // 1024, fonts name, n folder
    while ((de = readdir(dr)) != NULL) {
        if (elements == 21) { // limit
            luaL_argerror (L, 0, "Fonts directory has 21+ fonts");
            return 1;
        }
        if (strstr(de->d_name, ".otf") || strstr(de->d_name, ".ttf")) {
            sprintf(tmp, "%s/%s", usr_ffonts, de->d_name);
            fonts[elements] = tmp;
            elements++;
        }
    }
    closedir(dr);
    if (elements == 0) {
        luaL_argerror (L, 0, "Fonts directory has not fonts");
    }
    path = (char*)lua_newuserdata(L, strlen(usr_path)+usr_length); // works like malloc, adding dat to gc
    lua_pop(L, 1); // continues
    sprintf(path, "%s/", usr_path);
    FILE *out;
    int x_image = usr_fntsize*usr_length; // calcule width image, using length * font size
    int y_image = usr_fntsize*2; // calcule height image, using font size * 2
    char letter[2]; // set letters string with 2 bytes
    srand(time(NULL)); /* starts srand with os time */
    gdImagePtr im = gdImageCreateTrueColor (x_image, y_image); // creates image
    /* Background color for gfImageCreateTrueColor */
    gdImageFilledRectangle (im, 0, 0, gdImageSX (im), gdImageSY (im), gdImageColorAllocate(im, 255, 255, 255));
    /* gdImageCreate, will be first allocated */
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
            fonts[rand()%elements], // fontlist	The semicolon delimited list of font filenames to look for.
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
    luaL_getmetatable(L, "CAPTCHA"); // gets metatable
    lua_setmetatable(L, -1); // and save
    return 1;
}
static int setfontsize(lua_State* L) {
    lua_Number xsize = luaL_checknumber(L, 2);
    lua_pushnumber(L, xsize); // push fontsize into table
    lua_setfield(L, -3, "__fntsize"); // works by gettings metatable (-3)
    luaL_getmetatable(L, "CAPTCHA"); // gets metatable
    lua_setmetatable(L, -1); // and save
    return 1;
}
static int setpath(lua_State* L) {
    const char *path = luaL_checkstring(L, 2);
    lua_pushstring(L, path); // push path into table
    lua_setfield(L, -3, "__path"); // works by gettings metatable (-3)
    luaL_getmetatable(L, "CAPTCHA"); // gets metatable
    lua_setmetatable(L, -1); // and save
    return 1;
}
static int setformat(lua_State* L) {
    const char *format = luaL_checkstring(L, 2);
    lua_pushstring(L, format); // push format into table
    lua_setfield(L, -3, "__format"); // works by gettings metatable (-3)
    luaL_getmetatable(L, "CAPTCHA"); // gets metatable
    lua_setmetatable(L, -1); // and save
    return 1;
}
static int setfontsfolder(lua_State* L) {
    const char *ffolder = luaL_checkstring(L, 2);
    lua_pushstring(L, ffolder); // push format into table
    lua_setfield(L, -3, "__ffolder"); // works by gettings metatable (-3)
    luaL_getmetatable(L, "CAPTCHA"); // gets metatable
    lua_setmetatable(L, -1); // and save
    return 1;
}
static int L_new(lua_State *L) {
    static const struct luaL_Reg L_methods[] = {
        {"generate", &generate},
        {"setlength", &setlength},
        {"setfontsize", &setfontsize},
        {"setpath", &setpath},
        {"setformat", &setformat},
        {"setfontsfolder", &setfontsfolder},
        {NULL, NULL}
    };
    luaL_newmetatable(L, "CAPTCHA");
    lua_setmetatable(L, -1);
    #if LUA_VERSION_NUM == 501
    luaL_openlib( L, "m_captcha", L_methods, 0 );
    #else
    luaL_newlibtable(L, L_methods); // adds struct (those are the commands), only creates the table
    luaL_setfuncs(L, L_methods, 0);  // register methods struct
    #endif
    lua_pushvalue(L, -1); // push the metatable
    lua_setfield(L, -1,"__index"); // adds metatable to index
    return 1;
}
int luaopen_captcha(lua_State* L) {
    static const struct luaL_Reg L_register[] = {
        {"new", &L_new}, // creates a new beginning to make a captcha, adding methods and mt table
        {NULL, NULL}
    };
    #if LUA_VERSION_NUM == 501
    luaL_openlib( L, "libcaptcha", L_register, 0 );
    #else
    luaL_newlib(L, L_register); // adds L_register struct
    luaL_setfuncs(L, L_register, 0); // register L_register struct
    #endif

    lua_pushstring(L, LUA_CAPTCHA_VERSION); // add version value
    lua_setfield(L, -2, "VERSION"); // add to version variable
    return 1;
}
