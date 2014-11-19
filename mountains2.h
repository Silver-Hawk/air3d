#ifndef __MOUNTAIN_CLASS_H__
#define __MOUNTAIN_CLASS_H__

#include "glm/glm.hpp"

class mountain {
	public:
	int x, y, triangleCount;

	float pos_x, pos_y, pos_z;

	float *map;

	GLfloat *mesh;
	int mesh_indices;

	GLfloat *vertices;
	GLfloat *normals;

	//buffers
	GLuint points_vbo, normals_vbo, vao;

	//shader
	GLuint shader;

	//cam and view matrix
	//shader x,y,z location
	int mountain_location, view_mat_location, proj_mat_location;

	mat4 mountain_mat, view;

	mountain(int x, int y) : x(x), y(y) {
		triangleCount = x * y * 2 * 3;
		map = (float*) malloc(sizeof(float) * x * y);

		srand(time(0));

		shader = create_programme_from_files (
			"mountain_vs.glsl", "mountain_fs.glsl"
		);
		
		glUseProgram(shader);
		//bind mountains in the background
		//mountain_location = glGetUniformLocation (shader, "unit");

		this->view_mat_location = glGetUniformLocation (shader, "view");
		mountain_location = glGetUniformLocation (shader, "unit");
		proj_mat_location = glGetUniformLocation (shader, "proj");
 
		//glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_mat.m);


		//generate random heightmap
		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
				map[i + (j*x)] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));


		//generate coordinate data
		glm::vec3* vertData = (glm::vec3*) malloc(sizeof(glm::vec3) * x * y);

		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
			{
				float xScale = (float) i / float (x - 1);
				float zScale = (float) j / float (y - 1);

				vertData[j + (i*x)] = glm::vec3(-0.5f+ xScale, map[i + (j*x)], -0.5f+ zScale);
				//printf("vertData x: %f, y: %f, z: %f\n", -0.5f+ xScale, map[i + (j*x)], -0.5f+ zScale);
			}


		//generate triangles
		mesh = (GLfloat *) malloc(triangleCount * sizeof(GLfloat));
		int jf = 0;
		int count = 0;
		for (int j = 0; j < y-1; j++)
        	for (int i = 0; i < x; i++)
            {
                if (i - 1 >= 0)
                {
                    mesh[count] = jf - 1;
                    mesh[count+1] = jf;
                    mesh[count+2] = x + jf;

                	count += 3;
                }
                if (i + 1 <= x)
                {
                    mesh[count] = jf;
                    mesh[count+1] = x + 1 + jf;
                    mesh[count+2] = x + jf;

                	count += 3;
                }
                //printf("COUNT %i\n", count);
                jf++;
             }

        mesh_indices = count;
        printf("TRAINGLE COUNT %i, REAL COUNT %i\n", triangleCount, mesh_indices);

        printf("MESH\n");
//        printMesh();

        vertices = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);
		normals = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);

        float normx = 2.0f; //(1.0f/(x-1));
        float normy = 2.0f; //(1.0f/(y-1));

        //printf("\n");
        for(int i = 0; i < mesh_indices; i++){
    		//printf("[%i]: %i %i %f\n", i, convertMeshDataToX(i), convertMeshDataToY(i), mesh[i]);
        	vertices[i*3]   = vertData[(int)mesh[i]].x; 
        	vertices[i*3+1] = vertData[(int)mesh[i]].y;//mesh[i+1] * map[i];
        	vertices[i*3+2] = vertData[(int)mesh[i]].z;//mesh[i] * normy;//mesh[i+2] * normy;
        	//printf("Created vertice x:%f y:%f z:%f\n", vertices[i], vertices[i+1], vertices[i+2]);

        	printf("Using mesh %i\n", (int) mesh[i]);

        	//calculate normals
        	float A=vertices[i*3],B=vertices[i*3+1],C=vertices[i*3+2];

        	normals[i*3]    = vertices[i*3];
        	normals[i*3+1]  = vertices[i*3+1];
        	normals[i*3+2]  = vertices[i*3+2];
        }

        //printVertices();

        glGenBuffers (1, &points_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 3, vertices, GL_STATIC_DRAW);

        glGenBuffers (1, &normals_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 3, normals, GL_STATIC_DRAW);


        glGenVertexArrays (1, &vao);
        glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
	}

	int convertMeshDataToX(int i){
		int counter = 0;
		int test = mesh[i];
		while(test >= y){
			counter++;
			test-=y;
		}

		return (int) mesh[i] % 10;
	}

	int convertMeshDataToY(int i){
		int counter = 0;
		int test = mesh[i];
		while(test >= x){
			counter++;
			test-=x;
		}

		return counter;
	}

	void update(mat4 view_mat, GLfloat *proj_mat){
		glUseProgram(shader);
		glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
		
		glUseProgram(shader);
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
		
		glUseProgram(shader);
		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_mat.m);
	}

	void draw(float delta, mat4 view_mat) {
		glUseProgram(shader);
		//mountain_location = glGetUniformLocation(shader, "unit");
		//pos_z += delta;
		//mat4 T = translate (identity_mat4 (), vec3 (0.5f, 0.5f, 0.5f));
		mat4 mountain_m = translate (identity_mat4 (), vec3 (pos_x, pos_y, pos_z -200.0f));


		//const float *pSource = (const float*)glm::value_ptr(pMat4);


		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_m.m);		
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);

		glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
		glDrawArrays (GL_TRIANGLES, 0, triangleCount);
	}

	void printMap() {
		for(int i = 0; i < x; i++){
			for(int j = 0; j < y; j++)
				printf("%.2f ", map[i + (j*x)]);
			printf("\n");
		}
	}

	void printMesh() {
		printf("SIZE %i\n", mesh_indices);

		for(int i = 0; i < mesh_indices; i++){
			if(i%x == 0)	
				printf("\n");
		printf("%.2f ", mesh[i]);
		}	
	}

	void printVertices() {
		printf("VERTICES::\n");
		printf("SIZE %i\n", mesh_indices);

		for(int i = 0; i < mesh_indices; i++){
			if(i%9 == 0)	
				printf("\n");
		printf("%.2f ", vertices[i]);
		}	
	}
};

#endif

