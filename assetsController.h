#ifndef __ASSETS_CONTROLLER_CLASS_H__
#define __ASSETS_CONTROLLER_CLASS_H__

#include "texturehelper.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#define UNIT_TEXTURE 0
#define UNIT_PATH "AudioEquipment0039_2_S.jpg"
#define MOUNTAIN_TEXTURE 1
#define MOUNTAIN_PATH "mountain.png"
#define BULLET_TEXTURE 2
#define BULLET_PATH "bullet.png"

#define NUM_TEXTURES 3

class assetscontroller {
	public:
	texturehelper *textures;

	assetscontroller(){
			textures = (texturehelper*) malloc(sizeof(texturehelper) * NUM_TEXTURES);

			initTexture(UNIT_TEXTURE, UNIT_PATH);
			initTexture(MOUNTAIN_TEXTURE, MOUNTAIN_PATH);
			initTexture(BULLET_TEXTURE, BULLET_PATH);
	}

	void initTexture(int index, const char* path){
		textures[index] = texturehelper (path);
	}

	void bindTexture(int index){
		textures[index].bind();
	}
};


#endif