#ifndef __SHADER_CONTROLLER_CLASS_H__
#define __SHADER_CONTROLLER_CLASS_H__

#include "shaderhelper.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#define UNIT_SHADER 0
#define MOUNTAIN_SHADER 1
#define SPRITE_SHADER 2
#define WATER_SHADER 3

#define NUM_SHADERS 4

class shadercontroller {
	public:
	shaderhelper *shaders;

	shadercontroller(){
		shaders = (shaderhelper*) malloc(sizeof(shaderhelper) * NUM_SHADERS);

		initShader (UNIT_SHADER, "test_vs.glsl", "test_fs.glsl", 0);
		initShader (MOUNTAIN_SHADER, "mountain_vs.glsl", "mountain_fs.glsl", 0);
		initShader (SPRITE_SHADER, "shaders/bullet_vs.glsl", "shaders/bullet_fs.glsl", 0);	
		initShader (WATER_SHADER, "shaders/water_vs.glsl", "shaders/water_fs.glsl", 0);	
	}

	void initShader(int index, const char* vs, const char* fs, int additionalUniforms){
		shaders[index] = shaderhelper (vs, fs, 4 + additionalUniforms);
		shaders[index].use();

		//for sake of generality add all for standard uniforms for all shaders
		shaders[index].setLocation("view", 0);
		shaders[index].setLocation("unit", 1);
		shaders[index].setLocation("proj", 2);
		shaders[index].setLocation("inverse", 3);
	}

	//update all shaders with the basics needed, boolean inverse for flipping y on the models
	void updateShaders(mat4 view_mat, GLfloat *proj_mat, bool inverse){
		glm::vec3 in = glm::vec3(1.0f,1.0f,1.0f);

		if(inverse)
			in.y = -1.0f;
		
		for(int i = 0; i < NUM_SHADERS; i++){
			shaders[i].use();
			shaders[i].bindLocationMat(view_mat, 0);
			shaders[i].bindLocationFloatarray(proj_mat, 2);
			shaders[i].bindLocationVec3Floatarray((float*)glm::value_ptr(in), 3);
		}
	}

	shaderhelper getShader(int index){
		return shaders[index];
	}

	void updateUnit(int shader, mat4 unit){
		shaders[shader].bindLocationMat(unit, 1);
	}

	void use(int index){
		shaders[index].use();
	}
};


#endif