#ifndef __SPRITE_CLASS_H__
#define __SPRITE_CLASS_H__

#include "texturehelper.h"
#include "bufferHelper.h"

class sprite {
	public:
	
	bufferhelper bufHelp;
	int texture;

	float scale;

    float xscale, yscale;

    int animation;
    int animationIndex;

	sprite(){

	}

	sprite(int texindex){
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

		texture = texindex;
		
		bufHelp = bufferhelper(2);
		bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);
        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);

        animation = 0;
        animationIndex = 0;
	}

	//scales uniformly
	sprite(int texindex, float s){
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

		texture = texindex;
		
        scale = s;

		for(int i = 0; i < 18; i++)
			quad[i] *= s;

		bufHelp = bufferhelper(2);
		bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);
        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);

        animation = 0;
        
        animationIndex = 0;
	}

    //scales differently
    sprite(int texindex, float xs, float ys, float s){
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

        texture = texindex;

        scale = s;

        xs *=s;
        ys *=s;

        //scale on x
        for(int i = 0; i < 18; i+=3)
            quad[i] *= xs;

        //scale on y
        for(int i = 1; i < 18; i+=3)
            quad[i] *= ys;
        
        bufHelp = bufferhelper(2);
        bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);
        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);

        animation = 0;
        animationIndex = 0;

    }

    void bindTexCords5x5Animation(int index){
        int x = index % 5;
        int y = index / 5;

        GLfloat texcoords[] = {
            0.0f+((float)x/5.0f), 0.0f+((float)y/5.0f),
            0.2f+((float)x/5.0f), 0.0f+((float)y/5.0f),
            0.2f+((float)x/5.0f), 0.2f+((float)y/5.0f),
            0.2f+((float)x/5.0f), 0.2f+((float)y/5.0f),
            0.0f+((float)x/5.0f), 0.2f+((float)y/5.0f),
            0.0f+((float)x/5.0f), 0.0f+((float)y/5.0f)
        };

        bufHelp.bind(1, 12 * sizeof (GLfloat), texcoords, 2);
    }

    void startAnimation(int size){
        animation = size;
        animationIndex = 0;
    }

    bool animationDone(){
        printf("check animationDone\n");

        if(animation && animationIndex >= animation)
            return true;

        return false;        
    }

    void bind(){
        AC->bindTexture(texture);
        bufHelp.bindAll();
    }

	void draw(){
        bind();
        if(animation == 25)
            bindTexCords5x5Animation(animationIndex++);
		glDrawArrays (GL_TRIANGLES, 0, 6);
	}

    float getScale(){
        return scale;
    }
};


#endif