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
    const char *output_file = luaL_checkstring(L, 1);
    int size_letters = luaL_checknumber(L, 2);
    int font_size = luaL_checknumber(L, 3);
    FILE *out;
    int x_image = font_size*size_letters;
    int y_image = font_size*2;
    char letter[2];
    int i, y, x;
    srand(time(NULL)); /* starts srand with os time */
    gdImagePtr im = gdImageCreateTrueColor (x_image, y_image);
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
    int random_color = gdImageColorAllocate(im, rand()%200, rand()%200, rand()%200);
    for (i = 0; i<=size_letters; i++) {
        char rdm_letter = troc(t_w[rand()%48]);
        sprintf(letter, "%c", rdm_letter);
        char *error = gdImageStringTTF(
            im, // im	The image to draw onto.
            NULL, //brect	The bounding rectangle as array of 8 integers where each pair represents the x- and y-coordinate of a point.  The points specify the lower left, lower right, upper right and upper left corner.
            random_color, // fg	The font color
            fonts[rand()%12], // fontlist	The semicolon delimited list of font filenames to look for.
            font_size, // ptsize	The height of the font in typographical points (pt).
            rand()%1, // angle	The angle in radian to rotate the font counter-clockwise.
            x, // x	The x-coordinate of the basepoint (roughly the lower left corner) of the first letter.
            rand()%y+y_image/2, // y	The y-coordinate of the basepoint (roughly the lower left corner) of the first letter.
            letter // string	The string to render.
        );
        x+=font_size;
        if(error) printf("Couldn't create image string. %s\n", error);
    }
    for (i=0; i<(y_image*x_image)/2; i++) {
        int random_x = rand()%x_image;
        int random_y = rand()%y_image;
        gdImageSetPixel(im, random_x, random_y, random_color-4000);
    }
    out = fopen (output_file, "wb");
    gdImagePng (im, out);
    fclose (out);
    gdImageDestroy(im);
    return 1;
}

static const struct luaL_Reg captcha[] = {
    {"generate", generate},
    {NULL, NULL}
};
int luaopen_captcha(lua_State* L) { // returns library in table: library.function, must be luapen_libname
    luaL_newlibtable(L, captcha); // adds "x" struct (those are the commands), only creates the table
    luaL_setfuncs(L, captcha, 0); // im not sure what does mean '0', i think 0 means start of struct, not sure.
    return 1;
}
