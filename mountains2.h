#ifndef __MOUNTAIN_CLASS_H__
#define __MOUNTAIN_CLASS_H__

#include "glm/glm.hpp"
#include "bufferHelper.h"
#include "shaderhelper.h"
#include "texturehelper.h"

class mountain {
	public:
	int x, y, triangleCount;

	float pos_x, pos_y, pos_z;

	float *map;

	GLfloat *mesh;
	int mesh_indices;

	GLfloat *vertices;
	GLfloat *normals;
	GLfloat *uvarray;

	//buffers
	GLuint points_vbo, normals_vbo, vao, tex, tex_vbo;

	//shader
	//	shaderhelper shader;

	bufferhelper bufHelp;
	texturehelper texHelp;

	//cam and view matrix
	//shader x,y,z location
	int mountain_location, view_mat_location, proj_mat_location, tex_location;

	mat4 mountain_mat, view;

	mountain(){}

	mountain(int x, int y) : x(x), y(y) {
		triangleCount = x * y * 2 * 3;
		map = (float*) malloc(sizeof(float) * x * y);

		srand(time(0));

		//generate random heightmap
		/*
			Don't create heights on borders - set them to zero
		*/
		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
				if(i > 0 and i < x - 1 and j > 0 and j < y-1){
				map[i + (j*x)] = 50.0f * (y/(j+5)) *static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
				}
				else
				map[i + (j*x)] = 0.0f;

		BoxFilterHeightMap(x,y,map,true);

		//generate coordinate data
		glm::vec3* vertData = (glm::vec3*) malloc(sizeof(glm::vec3) * x * y);
		glm::vec2* uvData = (glm::vec2*) malloc(sizeof(glm::vec2) * x * y);
		
		//normally x*0.1f
		float texU = x*(1.0f/(float)x);//1.0f/(float)x;
		float texV = y*(1.0f/(float)y);//1.0f/(float)y;


		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
			{
				//triangle mapping
				float xScale = (float) i / float (x - 1);
				float zScale = (float) j / float (y - 1);
				
				vertData[j + (i*x)] = glm::vec3(-0.5f+ xScale, map[i + (j*x)], -0.5f+ zScale);
				
				//texture mapping
				float scaleCol = ((float)i)/(x-1) *texU;
				float scaleRow = ((float)j)/(y-1) *texV;

				uvData[j + (i*x)] = glm::vec2(scaleRow*texU, scaleCol*texV);
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
                jf++;
             }

        mesh_indices = count;

        vertices = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);
		normals = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 3);
		uvarray = (GLfloat *) malloc(sizeof(GLfloat) * mesh_indices * 2);

        for(int i = 0; i < mesh_indices; i++){
    		vertices[i*3]   = vertData[(int)mesh[i]].x; 
        	vertices[i*3+1] = vertData[(int)mesh[i]].y;//mesh[i+1] * map[i];
        	vertices[i*3+2] = vertData[(int)mesh[i]].z;//mesh[i] * normy;//mesh[i+2] * normy;
        	
        	normals[i*3]    = vertices[i*3];
        	normals[i*3+1]  = vertices[i*3+1];
        	normals[i*3+2]  = vertices[i*3+2];

        	uvarray[i*2] = uvData[(int)mesh[i]].x;
        	uvarray[i*2+1] = uvData[(int)mesh[i]].y;
        }

        pos_y = 0.0f;

        //texHelp = texturehelper("mountain.png");

        bufHelp = bufferhelper(3);
        bufHelp.bind(0, mesh_indices * sizeof (GLfloat) * 3, vertices, 3);
        bufHelp.bind(1, mesh_indices * sizeof (GLfloat) * 3, normals, 3);
        bufHelp.bind(2, mesh_indices * sizeof (GLfloat) * 2, uvarray, 2);
	}

	void update(mat4 view_mat, GLfloat *proj_mat){
		/*shader.use();
		shader.bindLocationFloatarray(proj_mat, 2);
		shader.bindLocationMat(view_mat, 0);*/
	}

	void draw(float delta, mat4 view_mat) {
		mat4 mountain_m = translate (identity_mat4 (), vec3 (pos_x, pos_y, pos_z -400.0f));
		
		shaderhelper shader = SC->getShader(MOUNTAIN_SHADER);
		shader.use();
		shader.bindLocationMat(mountain_m, 1);
		
		AC->bindTexture(MOUNTAIN_TEXTURE);
		bufHelp.bindAll();

		bufHelp.drawTriangles();
	}


	//debug functions
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

