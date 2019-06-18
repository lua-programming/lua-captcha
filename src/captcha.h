#ifndef CAPTCHA_H
#define LUA_CAPTCHA_VERSION "v0.1-3"
#define CAPTCHA_H
int t_w[49] = {
	101, 141, 102, 142, 103, 143, 104, 144, 105, 145,
	106, 146, 107, 147, 110, 150, 151, 112, 152, 113,
	153, 114, 154, 115, 155, 116, 156, 117, 157, 120,
	160, 121, 161, 122, 162, 124, 164, 125, 165, 126,
	166, 127, 167, 130, 170, 131, 171, 132, 172
};
float Pow(float x, int y) {
	float temp;
	if(y==0) return 1;
	temp = Pow(x, y/2);
	if (y%2==0) return temp*temp;
	else {
		if(y>0) return x*temp*temp;
		else return (temp*temp)/x;
	}
}
static int troc(int o) {
	int d = 0, i = 0;
	do { d += (o % 10) * Pow(8, i); ++i; o /= 10; }
	while (o != 0);
	return d;
}
#endif
