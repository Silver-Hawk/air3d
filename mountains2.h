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
	GLuint points_vbo, normals_vbo, vao, tex, tex_vbo;

	//shader
	GLuint shader;

	//cam and view matrix
	//shader x,y,z location
	int mountain_location, view_mat_location, proj_mat_location, tex_location;

	mat4 mountain_mat, view;

	mountain(int x, int y) : x(x), y(y) {
		triangleCount = x * y * 2 * 3;
		map = (float*) malloc(sizeof(float) * x * y);

		srand(time(0));

		shader = create_programme_from_files (
			"mountain_vs.glsl", "mountain_fs.glsl"
		);
		
		glUseProgram(shader);

		view_mat_location = glGetUniformLocation (shader, "view");
		mountain_location = glGetUniformLocation (shader, "unit");
		proj_mat_location = glGetUniformLocation (shader, "proj");

		//generate random heightmap
		/*
			Don't create heights on borders - set them to zero
		*/
		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
				(i > 0 and i < x - 1 and j > 0 and j < y-1)?
				map[i + (j*x)] = 50.0f * (y/(j+5)) *static_cast <float> (rand()) / (static_cast <float> (RAND_MAX))
				:
				map[i + (j*x)] = 0.0f;

		//BoxFilterHeightMap(x,y,map,false);

		//generate coordinate data
		glm::vec3* vertData = (glm::vec3*) malloc(sizeof(glm::vec3) * x * y);
		GLfloat* uvData = (GLfloat*) malloc(sizeof(GLfloat) * x * y * 2);
		float texU = x*0.1f;
		float texV = y*0.1f;


		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
			{
				float xScale = (float) i / float (x - 1);
				float zScale = (float) j / float (y - 1);

				//texture mapping
				float scaleCol = ((float)i)/(x-1);
				float scaleRow = ((float)j)/(y-1);


				vertData[j + (i*x)] = glm::vec3(-0.5f+ xScale, map[i + (j*x)], -0.5f+ zScale);
				uvData[j*2 + (i*x)] = scaleRow*texU;
				//printf("scale %f tex %f\n", scaleRow, texU);
				//printf("uvData[%i] = %f\n", j*2 + (i*x),(float)scaleRow*texU);
				uvData[j*2 + (i*x) + 1] = scaleCol*texV; 
				//printf("uvData[%i] = %f\n", j*2 + (i*x) + 1,scaleCol*texV);


			}


		//bind texture
		tex_location = glGetUniformLocation (shader, "basic_texture");
		glUniform1i (tex_location, 0);
	
		glActiveTexture (GL_TEXTURE0);
		assert (load_texture("mountain.jpg", &tex));
		glBindTexture (GL_TEXTURE_2D, tex);

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
                jf++;
             }

        mesh_indices = count;

        vertices = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);
		normals = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);

        for(int i = 0; i < mesh_indices; i++){
    		vertices[i*3]   = vertData[(int)mesh[i]].x; 
        	vertices[i*3+1] = vertData[(int)mesh[i]].y;//mesh[i+1] * map[i];
        	vertices[i*3+2] = vertData[(int)mesh[i]].z;//mesh[i] * normy;//mesh[i+2] * normy;
        	
        	normals[i*3]    = vertices[i*3];
        	normals[i*3+1]  = vertices[i*3+1];
        	normals[i*3+2]  = vertices[i*3+2];
        }

        glGenBuffers (1, &points_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 3, vertices, GL_STATIC_DRAW);

        glGenBuffers (1, &normals_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 3, normals, GL_STATIC_DRAW);

        glGenBuffers (1, &tex_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, tex_vbo);
        glBufferData (GL_ARRAY_BUFFER, mesh_indices * sizeof (GLfloat) * 2, uvData, GL_STATIC_DRAW);

        glGenVertexArrays (1, &vao);
        glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        /*glBindBuffer (GL_ARRAY_BUFFER, tex_vbo);
        glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/


        glBindTexture (GL_TEXTURE_2D, tex);


        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
        glEnableVertexAttribArray (2);
	}

	void update(mat4 view_mat, GLfloat *proj_mat){
		glUseProgram(shader);
		glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
		
		glUseProgram(shader);
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
		
		/*glUseProgram(shader);
		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_mat.m);*/
	}

	void draw(float delta, mat4 view_mat) {
		glUseProgram(shader);
		//mountain_location = glGetUniformLocation(shader, "unit");
		//pos_z += delta;
		//mat4 T = translate (identity_mat4 (), vec3 (0.5f, 0.5f, 0.5f));
		mat4 mountain_m = translate (identity_mat4 (), vec3 (pos_x, pos_y, pos_z -400.0f));


		//const float *pSource = (const float*)glm::value_ptr(pMat4);


		glUniformMatrix4fv (mountain_location, 1, GL_FALSE, mountain_m.m);		
		glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);

		glBindTexture (GL_TEXTURE_2D, tex);

		glBindVertexArray (vao);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
        glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (1);
        glBindBuffer (GL_ARRAY_BUFFER, tex_vbo);
        glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (2);

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

	static void BoxFilterHeightMap(unsigned long width, unsigned long height,
                        float*& heightMap, bool smoothEdges)
	{
	  //     width: Width of the height map in bytes
	  //    height: Height of the height map in bytes
	  // heightMap: Pointer to your height map data
	  
	  // Temporary values for traversing single dimensional arrays
	  long x = 0;
	  long z = 0;
	  
	  long  widthClamp = (smoothEdges) ?  width : width  - 1;
	  long heightClamp = (smoothEdges) ? height : height - 1;
	  
	  // [Optimization] Calculate bounds ahead of time
	  unsigned int bounds = width * height;
	  
	  // Validate requirements
	  if (!heightMap)
	    return;
	  
	  // Allocate the result
	  float* result = new float[bounds];
	  
	  // Make sure memory was allocated
	  if (!result)
	    return;
	  
	  for (z = (smoothEdges) ? 0 : 1; z < heightClamp; ++z)
	  {
	    for (x = (smoothEdges) ? 0 : 1; x < widthClamp; ++x)
	    {
	      // Sample a 3x3 filtering grid based on surrounding neighbors
	      
	      float value = 0.0f;
	      float cellAverage = 1.0f;
	      
	      // Sample top row
	      
	      if (((x - 1) + (z - 1) * width) >= 0 &&
	          ((x - 1) + (z - 1) * width) < bounds)
	      {
	        value += heightMap[(x - 1) + (z - 1) * width];
	        ++cellAverage;
	      }
	      
	      if (((x - 0) + (z - 1) * width) >= 0 &&
	          ((x - 0) + (z - 1) * width) < bounds)
	      {
	        value += heightMap[(x    ) + (z - 1) * width];
	        ++cellAverage;
	      }
	      
	      if (((x + 1) + (z - 1) * width) >= 0 &&
	          ((x + 1) + (z - 1) * width) < bounds)
	      {
	        value += heightMap[(x + 1) + (z - 1) * width];
	        ++cellAverage;
	      }
	      
	      // Sample middle row
	      
	      if (((x - 1) + (z - 0) * width) >= 0 &&
	          ((x - 1) + (z - 0) * width) < bounds)
	      {
	        value += heightMap[(x - 1) + (z    ) * width];
	        ++cellAverage;
	      }
	      
	      // Sample center point (will always be in bounds)
	      value += heightMap[x + z * width];
	      
	      if (((x + 1) + (z - 0) * width) >= 0 &&
	          ((x + 1) + (z - 0) * width) < bounds)
	      {
	        value += heightMap[(x + 1) + (z    ) * width];
	        ++cellAverage;
	      }
	      
	      // Sample bottom row
	      
	      if (((x - 1) + (z + 1) * width) >= 0 &&
	          ((x - 1) + (z + 1) * width) < bounds)
	      {
	        value += heightMap[(x - 1) + (z + 1) * width];
	        ++cellAverage;
	      }
	      
	      if (((x - 0) + (z + 1) * width) >= 0 &&
	          ((x - 0) + (z + 1) * width) < bounds)
	      {
	        value += heightMap[(x    ) + (z + 1) * width];
	        ++cellAverage;
	      }
	      
	      if (((x + 1) + (z + 1) * width) >= 0 &&
	          ((x + 1) + (z + 1) * width) < bounds)
	      {
	        value += heightMap[(x + 1) + (z + 1) * width];
	        ++cellAverage;
	      }
	      
	      // Store the result
	      result[x + z * width] = value / cellAverage;
	    }
	  }
	  
	  // Release the old array
	  delete [] heightMap;
	  
	  // Store the new one
	  heightMap = result;
	}

};

#endif

