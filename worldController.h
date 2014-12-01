#ifndef __WORLDCONTROLLER_CLASS_H__
#define __WORLDCONTROLLER_CLASS_H__



class worldController {
	public:
	int xstart, ystart, xend, yend;


	worldController(){

	}

	worldController(int xstart, int ystart, int xend, int yend) : xstart(xstart), ystart(ystart), xend(xend), yend(yend){

	}

	int *getBoundsArray(){
		int *arr = (int*) malloc(sizeof(int) * 4);
		arr[0] = xstart;
		arr[1] = ystart;
		arr[2] = xend;
		arr[3] = yend;

		return arr;
	}
};

#endif