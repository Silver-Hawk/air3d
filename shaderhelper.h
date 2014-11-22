/*
	Basic unit class, this is like an entity but with 
	rotations, models and textures always attached
*/
#ifndef __SHADER_HELPER_CLASS_H__
#define __SHADER_HELPER_CLASS_H__

class shaderhelper {
	public:
	int Nlocations;
	int *locations;
	GLint shader;

	shaderhelper(){

	}

	shaderhelper(const char* vs, const char* fs, int location_number){
		Nlocations = location_number;

		shader = create_programme_from_files (
			vs, fs
		);

		locations = (int*) malloc(sizeof(int) * location_number);

	}

	int setLocation(const char* name, int index){
		int loc = glGetUniformLocation (shader, name);
		locations[index] = loc;

		return loc; 
	}

	void bindLocationMat(mat4 mat, int index){
		glUniformMatrix4fv (locations[index], 1, GL_FALSE, mat.m);
	}

	void bindLocationFloatarray(GLfloat* array, int index){
		glUniformMatrix4fv (locations[index], 1, GL_FALSE, array);
	}

	void use(){
		glUseProgram(shader);
	}
};


#endif