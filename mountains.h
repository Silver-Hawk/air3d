#ifndef __MOUNTAIN_CLASS_H__
#define __MOUNTAIN_CLASS_H__

struct vertex
{
	GLfloat x, y, z;
};

class mountain {
	public:
	int x, y, triangleCount;

	float *map;

	GLfloat *mesh;
	int mesh_indices;

	GLfloat *vertices;

	//buffers
	GLuint points_vbo, colours_vbo, vao;

	//shader
	GLuint shader;

	//cam and view matrix
	//shader x,y,z location
	int mountain_location, view_mat_location, proj_mat_location;
	mat4 mountain_mat, view;

	mountain(int x, int y) : x(x), y(y) {
		triangleCount = x * y * 2 * 3;
		map = (float*) malloc(sizeof(int) * x * y);

		srand(time(0));

		shader = create_programme_from_files (
			"mountain_vs.glsl", "mountain_fs.glsl"
		);
		
		glUseProgram(shader);
		//bind mountains in the background
		//mountain_location = glGetUniformLocation (shader, "unit");

		view_mat_location = glGetUniformLocation (shader, "view");
		proj_mat_location = glGetUniformLocation (shader, "proj");
		mountain_location = glGetUniformLocation (shader, "unit");
 
		//glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_mat.m);


		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
				map[i + (j*x)] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

		

		//generate triangles
		mesh = (GLfloat *) malloc(triangleCount * sizeof(GLfloat));
		printf("TRIANGLES %i \n", triangleCount);
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
                printf("COUNT %i\n", count);
                jf++;
             }

        mesh_indices = count;

        printMesh();

        vertices = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);

        float normx = 2.0f; //(1.0f/(x-1));
        float normy = 2.0f; //(1.0f/(y-1));

        printf("\n");
        for(int i = 0; i < mesh_indices; i++){
    		printf("[%i]: %i %i %f\n", i, convertMeshDataToX(i), convertMeshDataToY(i), mesh[i]);
        	vertices[i*3]   = -0.5 + convertMeshDataToX(i)*normx; 
        	vertices[i*3+1] = 5.0f * map[(int) mesh[i]];//mesh[i+1] * map[i];
        	vertices[i*3+2] = -0.5 + convertMeshDataToY(i)*normy;//mesh[i] * normy;//mesh[i+2] * normy;
        	printf("Created vertice x:%f y:%f z:%f\n", vertices[i], vertices[i+1], vertices[i+2]);	
        }

        printVertices();

        glGenBuffers (1, &points_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 3, vertices, GL_STATIC_DRAW);

        glGenVertexArrays (1, &vao);
        glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
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
		view = view_mat;
		glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_mat.m);
	}

	void draw(float delta) {
		glUseProgram(shader);
		mountain_location = glGetUniformLocation(shader, "unit");
		mat4 mountain_m = translate (identity_mat4 (), vec3 (0.0f + delta , 0.0f , 50.0f + delta));
		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_m.m);

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

