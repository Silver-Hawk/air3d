/*
	Basic unit class, this is like an entity but with 
	rotations, models and textures always attached
*/
#ifndef __BACKGROUND_CLASS_H__
#define __BACKGROUND_CLASS_H__

#include <cmath>
#include "bvec.h"

class background {
public:
	GLuint background_shader;
	
	GLfloat *background_array;

	int back_view_location;

	background(const char* vs, const char* fs) {
		background_shader = create_programme_from_files (
			vs, fs
		);

		GLfloat bg[] = {
	        -100.0f, -1.0f,
	        1.0f, -1.0f,
	        -1.0f,  1.0f,
	        1.0f,  1.0f,
    	};

    	background_array = bg;
	}

	void setViewMatrix(mat4 view_mat) {
		int back_view_location = glGetUniformLocation (background_shader, "proj");
		glUseProgram (background_shader);
		glUniformMatrix4fv (back_view_location, 1, GL_FALSE, view_mat.m);
	}
	void draw() {
		glUseProgram (background_shader);
		
		GLuint bvbo;
		glGenBuffers (1, &bvbo);
		glBindBuffer (GL_ARRAY_BUFFER, bvbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			4 * 2 * sizeof (GLfloat),
			background_array,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		glVertexAttribPointer(GL_ARRAY_BUFFER, 2, GL_FLOAT, 0, 0, NULL);
		glDrawArrays (GL_QUADS, 0, 4);
	}
	
};


#endif