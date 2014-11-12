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

	GLuint points_vbo, colours_vbo, vao;

	int back_view_location;

	background(const char* vs, const char* fs) {
		background_shader = create_programme_from_files (
			vs, fs
		);

		GLfloat points[] = {
                 -1.0f,  -1.0f,   0.0f,
                 1.0f,  -1.0f,   0.0f,
                 1.0f,  1.0f,   0.0f,
                 1.0f,  1.0f,   0.0f,
                 -1.0f,  1.0f,   0.0f,
                 -1.0f,  -1.0f,   0.0f
        };

        GLfloat colours[] = {
                1.0f, 0.0f,  0.0f,
                0.0f, 1.0f,  0.0f,
                0.0f, 0.0f,  1.0f,
                0.0f, 0.0f,  1.0f,
                0.0f, 1.0f,  0.0f,
                1.0f, 0.0f,  0.0f
        };


        //generate square and colors
        glGenBuffers (1, &points_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glBufferData (GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), points, GL_STATIC_DRAW);

        glGenBuffers (1, &colours_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
        glBufferData (GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), colours, GL_STATIC_DRAW);

        glGenVertexArrays (1, &vao);
        glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
	}

	void setViewMatrix(mat4 view_mat) {
		int back_view_location = glGetUniformLocation (background_shader, "proj");
		glUseProgram (background_shader);
		glUniformMatrix4fv (back_view_location, 1, GL_FALSE, view_mat.m);
	}

	void draw() {
		glUseProgram (background_shader);
		glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
		glDrawArrays (GL_TRIANGLES, 0, 6);
	}
	
};


#endif