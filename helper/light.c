#include "light.h"
#include <time.h>


float C;
float X;
float m;

void calc_rgb_tmp(struct hsv* hsv, int h){
	
	printf("before rgb_tmp\n h: %f  s: %f  v: %f\n",hsv->h,hsv->s,hsv->v);
	
	if( (h >= 0) && (h < 60) ){
		hsv->h = C;
		hsv->s = X;
		hsv->v = 0;
	}
	if( (h >= 60) && (h < 120) ){
		hsv->h = X;
		hsv->s = C;
		hsv->v = 0;
	}
	if( (h >= 120) && (h < 180) ){
		hsv->h = 0;
		hsv->s = C;
		hsv->v = X;
	}
	if( (h >= 180) && (h < 240) ){
		hsv->h = 0;
		hsv->s = X;
		hsv->v = C;
	}
	if( (h >= 240) && (h < 300) ){
		hsv->h = X;
		hsv->s = 0;
		hsv->v = C;
	}
	if( (h >= 300) && (h < 360) ){
		hsv->h = C;
		hsv->s = 0;
		hsv->v = X;
	}
	
	printf("after rgb_tmp\n h: %f  s: %f  v: %f\n",hsv->h,hsv->s,hsv->v);
	
}

void calc_rgb(struct hsv* hsv, float m){
	
	calc_rgb_tmp(hsv,hsv->h);
	
	hsv->h = ((hsv->h) + m);
	hsv->s = ((hsv->s) + m);
	hsv->v = ((hsv->v) + m);
}

void hsv_to_rgb(struct hsv* hsv, float h, float s, float v){
	
	s = s/100;
	v = v/100;
			
	C = v*s;
	float tmp_angle = (h/60);
	float modulo = fmodf(tmp_angle,2);		// (h/2) % 2
	float tmp = modulo - 1; 
	if(tmp < 0){
		tmp *= -1;
	}
	
	X = C * (1.0 - tmp);
	m = v-C;	
	
	calc_rgb(hsv,m);
	C = 0;
	X = 0;
	m = 0;
	//~ printf(" h: %f  s: %f  v: %f\n",hsv->h,hsv->s,hsv->v);
	
}


// returns color: red | green | cyan | white
void get_random_color(struct hsv* light_color){
	srand(time(NULL));
		
	int rnd_col = rand() % 4;
	
	switch(rnd_col){
		// red
		case 0:	puts("RED"); hsv_to_rgb(light_color,0,100,100);
		break;

		// green
		case 1:	puts("GREEN"); hsv_to_rgb(light_color,120,100,100);
		break;

		// cyan
		case 2:	puts("CYAN"); hsv_to_rgb(light_color,180,100,100);
		break;
		
		// white
		case 3:	puts("WHITE"); hsv_to_rgb(light_color,0,0,100);
		break;
		
		default: break;
	}
}
