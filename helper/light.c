#include "light.h"
#include <time.h>

// returns color: red | green | blue | white
void get_random_color(struct rgb* light_color){
	srand(time(NULL));
		
	int rnd_col = rand() % 4;
	
	switch(rnd_col){
		// red
		case 0:	light_color->r = 0.8;
				light_color->g = 0.0;
				light_color->b = 0.0;
		break;

		// green
		case 1:	light_color->r = 0.0;
				light_color->g = 0.8;
				light_color->b = 0.0;
		break;

		// blue
		case 2:	light_color->r = 0.0;
				light_color->g = 0.0;
				light_color->b = 0.8;
		break;
		
		// white
		case 3:	light_color->r = 0.5;
				light_color->g = 0.5;
				light_color->b = 0.5;
		break;
		
		default: break;
	}
}
