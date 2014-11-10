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

	GLuint vbo;

	int back_view_location;

	background(const char* vs, const char* fs) {
		background_shader = create_programme_from_files (
			vs, fs
		);

		GLfloat bg[24] =
		{
		   -1000000.000000f, 0.000000f, 1.000000f,   //V1
		    1.000000f, 0.000000f, 1.000000f,    //N1
		    1.000000f, 0.000000f, 1.000000f,    //V2
		    1.000000f, 0.000000f, 1.000000f,    //N2
		    1.000000f, 0.000000f, -1.000000f,   //V3
		    1.000000f, 0.000000f, 1.000000f,    //N3
		    -1.000000f, 0.000000f, -1.000000f,  //V4
		    1.000000f, 0.000000f, 1000000.000000f     //N4
		};

    	background_array = bg;

		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * 4 * sizeof (GLfloat),
			bg,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
	}

	void setViewMatrix(mat4 view_mat) {
		int back_view_location = glGetUniformLocation (background_shader, "proj");
		glUseProgram (background_shader);
		glUniformMatrix4fv (back_view_location, 1, GL_FALSE, view_mat.m);
	}
	void draw() {
		glUseProgram (background_shader);
		
		//bind vertex array
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer (3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		glBindVertexArray (vbo);
		glDrawArrays (GL_TRIANGLES, 0, 12);
	}
	
};


#endif