#ifndef __WATER_CLASS_H__
#define __WATER_CLASS_H__


#define WATER_SCALE 10000

class water {
public:
	bufferhelper bufHelp;

	angular a;
	
	shaderhelper shader;

	water(){
		/*GLfloat quad[] = {
             -1.0f,  0.0f,   -1.0f,
             1.0f,  0.0f,   -1.0f,
             1.0f,  0.0f,   1.0f,
             1.0f,  0.0f,   1.0f,
             -1.0f,  0.0f,   1.0f,
             -1.0f,  0.0f,   -1.0f
        };*/
        /*GLfloat quad[] = {
             -1.0f,  -1.0f,   0.0f,
             1.0f,  -1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             1.0f,  1.0f,   0.0f,
             -1.0f,  1.0f,   0.0f,
             -1.0f,  -1.0f,   0.0f
        };*/
        GLfloat quad[] = {
             0.0f, -1.0f,  -1.0f, 
             0.0f, -1.0f,  1.0f,   
             0.0f, 1.0f,  1.0f,   
             0.0f, 1.0f,  1.0f,     
             0.0f, 1.0f,  -1.0f,   
             0.0f, -1.0f,  -1.0f,   
        };

        for(int i = 0; i < 18; i++){
			quad[i] *= WATER_SCALE;
			printf("QUAD[%i] is now %f\n", i, quad[i]);
		}


		bufHelp = bufferhelper(1);
        bufHelp.bind(0, 18 * sizeof (GLfloat), quad, 3);        

        a = angular();
		a.setPos(0.0f, 0.0f, 0.0f);

		shader = shaderhelper("shaders/water_vs.glsl", "shaders/water_fs.glsl", 3);

		shader.setLocation("view", 0);
		shader.setLocation("unit", 1);
		shader.setLocation("proj", 2);
	}

	void update(mat4 view_mat, GLfloat *proj_mat){
		shader.use();
		shader.bindLocationFloatarray(proj_mat, 2);
		shader.bindLocationMat(view_mat, 0);
	}

	void bind(){
		shader.use();
        a.bindBulletMat();
        bufHelp.bindAll();
	}

	void draw(){
        bind();

		glDrawArrays (GL_TRIANGLES, 0, 6);
	}
};

#endif

