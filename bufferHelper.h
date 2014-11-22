/*
	Basic unit class, this is like an entity but with 
	rotations, models and textures always attached
*/
#ifndef __BUFFER_HELPER_CLASS_H__
#define __BUFFER_HELPER_CLASS_H__

class bufferhelper {
	public:
	GLuint vao;
	GLuint *buffers;
	int bufCount;
	int *vecType; //contains information about what type of vectors is used

	int firstIndexSize;

	bufferhelper(){

	}

	bufferhelper(int buffer_count){
		bufCount = buffer_count;
		//create an extra buffer for vertex array
		glGenVertexArrays (1, &vao);

		buffers = (GLuint *) malloc(sizeof(GLuint) * buffer_count);
		vecType = (int *) malloc(sizeof(int) * buffer_count);

		for(int i = 0; i < buffer_count; i++){
			glGenBuffers (1, &buffers[i]); 
		}
	}

	void bind(int buffer, int size, GLfloat* array, int type){
		glBindBuffer (GL_ARRAY_BUFFER, buffers[buffer]);
        glBufferData (GL_ARRAY_BUFFER, size, array, GL_STATIC_DRAW);
        vecType[buffer] = type;
        if(buffer == 0)
        	firstIndexSize = size;
	}

	void bindAll(){
		glBindVertexArray (vao);
		for(int i = 0; i < bufCount; i++){
			glBindBuffer (GL_ARRAY_BUFFER, buffers[i]);
        	glVertexAttribPointer (i, vecType[i], GL_FLOAT, GL_FALSE, 0, NULL);
        	glEnableVertexAttribArray (i); //enable them in shaders
		}
	}

	void drawTriangles(){
		glDrawArrays (GL_TRIANGLES, 0, firstIndexSize);
	}
	
	void send(){
		glBindVertexArray(vao);
	}
};


#endif