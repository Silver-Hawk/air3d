#ifndef __ASSETS_CONTROLLER_CLASS_H__
#define __ASSETS_CONTROLLER_CLASS_H__

#include "texturehelper.h"
#include "bufferHelper.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#define UNIT_TEXTURE 0
#define UNIT_PATH "AudioEquipment0039_2_S.jpg"
#define MOUNTAIN_TEXTURE 1
#define MOUNTAIN_PATH "mountain.png"
#define BULLET_TEXTURE 2
#define BULLET_PATH "test1.png"


#define POW_TEXTURE 3
#define POW_PATH "powScaled.png"

#define EXPLOSION_TEXTURE 4
#define EXPLOSION_PATH "explosion5x5.png"

#define NUM_TEXTURES 5

#define MODEL_PLANE 0
#define MODEL_PLANE_PATH "test1.obj"
#define MODEL_JET 1
#define MODEL_JET_PATH "test1.obj"

#define NUM_MODELS 2

class assetscontroller {
	public:
	texturehelper *textures;
	bufferhelper *plane;
	bufferhelper *jet;

	assetscontroller(){
			textures = (texturehelper*) malloc(sizeof(texturehelper) * NUM_TEXTURES);
			
			initTexture(UNIT_TEXTURE, UNIT_PATH);
			initTexture(MOUNTAIN_TEXTURE, MOUNTAIN_PATH);
			initTexture(BULLET_TEXTURE, BULLET_PATH);
			initTexture(POW_TEXTURE, POW_PATH);
			initTexture(EXPLOSION_TEXTURE, EXPLOSION_PATH);

	
			/*plane = new bufferhelper(0);
			plane->loadModel(MODEL_PLANE_PATH);*/
	}

	void initTexture(int index, const char* path){
		textures[index] = texturehelper (path);
	}

	float getXscale(int index){
		return textures[index].xyscale;
	}

	float getYscale(int index){
		return 1/textures[index].xyscale;
	}

	void bindTexture(int index){
		textures[index].bind();
	}

};


#endif