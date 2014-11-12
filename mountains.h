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
	int cam, view, projection;

	mountain(int x, int y) : x(x), y(y) {
		triangleCount = x * y * 2 * 3;
		map = (float*) malloc(sizeof(int) * x * y);

		srand(time(0));

		shader = create_programme_from_files (
			"mountain_vs.glsl", "mountain_fs.glsl"
		);

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

        float normx = (1.0f/(x-1));
        float normy = (1.0f/(y-1));

        printf("\n");
        for(int i = 0; i < mesh_indices; i++){
    		printf("[%i]: %i %i %f\n", i, convertMeshDataToX(i), convertMeshDataToY(i), mesh[i]);
        	vertices[i*3]   = -0.5 + convertMeshDataToX(i)*normx; 
        	vertices[i*3+1] = -0.5 + convertMeshDataToY(i)*normy;//mesh[i+1] * map[i];
        	vertices[i*3+2] = map[(int) mesh[i]];//mesh[i] * normy;//mesh[i+2] * normy;
        	printf("Created vertice x:%f y:%f z:%f\n", vertices[i], vertices[i+1], vertices[i+2]);	
        }

        printVertices();

        GLfloat points[] = {
                 -1.0f,  -1.0f,   0.0f,
                 1.0f,  -1.0f,   0.0f,
                 1.0f,  1.0f,   0.0f,
                 1.0f,  1.0f,   0.0f,
                 -1.0f,  1.0f,   0.0f,
                 -1.0f,  -1.0f,   0.0f
        };

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

	void update(int cam_loc, int view_loc, int proj_loc, mat4 view_mat, GLfloat *proj_mat){
		int view_mat_location = glGetUniformLocation (shader, "view");
		glUseProgram (shader);
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);

		int proj_mat_location = glGetUniformLocation (shader, "proj");
		glUseProgram (shader);
		glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
	}

	void draw() {
		glUseProgram (shader);
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

