#ifndef __SPRITE_CLASS_H__
#define __SPRITE_CLASS_H__

#include "texturehelper.h"
#include "bufferHelper.h"

class sprite {
	public:
	
	bufferhelper bufHelp;
	texturehelper texHelp;

	float scale;

	sprite(){

	}



	sprite(texturehelper texBuf){
		GLfloat quad[] = {
             -1.0f,  -1.0f,   0.0f,
             1.0f,  -1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             -1.0f,  1.0f,   0.0f,
             -1.0f,  -1.0f,   0.0f
        };

        GLfloat texcoords[] = {
        	0.0f, 0.0f,
        	1.0f, 0.0f,
        	1.0f, 1.0f,
        	1.0f, 1.0f,
        	0.0f, 1.0f,
        	0.0f, 0.0f
        };

		texHelp = texBuf;
		
		bufHelp = bufferhelper(2);
		bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);
        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);
	}

	//scales
	sprite(texturehelper texBuf, float s){
		GLfloat quad[] = {
             -1.0f,  -1.0f,   0.0f,
             1.0f,  -1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             -1.0f,  1.0f,   0.0f,
             -1.0f,  -1.0f,   0.0f
        };

        GLfloat texcoords[] = {
        	0.0f, 0.0f,
        	1.0f, 0.0f,
        	1.0f, 1.0f,
        	1.0f, 1.0f,
        	0.0f, 1.0f,
        	0.0f, 0.0f
        };

		texHelp = texBuf;
		

		for(int i = 0; i < 18; i++)
			quad[i] *= s;

		bufHelp = bufferhelper(2);
		bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);
        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);

	}

	void draw(){
		texHelp.bind();
		bufHelp.bindAll();

		glDrawArrays (GL_TRIANGLES, 0, 6);
	}
};


#endif