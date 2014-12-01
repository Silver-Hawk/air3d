#ifndef __CAM_CLASS_H__
#define __CAM_CLASS_H__

#include <vector>
#define GLM_FORCE_RADIANS
#include "glm/gtc/type_ptr.hpp"

class cam {
public:
	float cam_speed; // 1 unit per second
	float cam_yaw_speed; // 10 degrees per second
	float cam_pos[3]; // don't start at zero, or we will be too close
	float cam_yaw; // y-rotation in degrees
	GLfloat *proj_mat;

	unit *following[4];
	bool followEnabled[4];


	//frustum culling
	float frustum[6][4];

	cam(){
		//Camera vector
		cam_speed = 25.0f; // 1 unit per second
		cam_yaw_speed = 33.0f; // 10 degrees per second
		
		// don't start at zero, or we will be too close
		cam_pos[0] = 0.0f;
		cam_pos[1] = 0.0f;
		cam_pos[2] = 120.0f;

		cam_yaw = 0.0f; // y-rotation in degrees

		//Projection matrix
		// input variables
		float near = 0.1f; // clipping plane
		float far = 2400.0f; // clipping plane
		float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
		float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
		// matrix components
		float range = tan (fov * 0.5f) * near;
		float Sx = (2.0f * near) / (range * aspect + range * aspect);
		float Sy = near / range;
		float Sz = -(far + near) / (far - near);
		float Pz = -(2.0f * far * near) / (far - near);
		GLfloat proj_matrix[] = {
			Sx, 0.0f, 0.0f, 0.0f,
			0.0f, Sy, 0.0f, 0.0f,
			0.0f, 0.0f, Sz, -1.0f,
			0.0f, 0.0f, Pz, 0.0f
		};

		//silly asignment
		proj_mat = (GLfloat *)malloc(sizeof(GLfloat) * 16);
		for(int i=0; i < 16; i ++)
			proj_mat[i] = proj_matrix[i];
	}

	void setPos(float x, float y, float z){
		cam_pos[0] = x;
		cam_pos[1] = y;
		cam_pos[2] = z;
	}

	void setX(float n){
		cam_pos[0] = n;
	}
	void setY(float n){
		cam_pos[1] = n;
	}
	void setZ(float n){
		cam_pos[2] = n;
	}

	void setDeltaZ(float n){
		cam_pos[2] += cam_speed * n;
	}

	void disableFollow(int disable){

	}

	void setFollow(int follow, unit *pointer){
		followEnabled[follow] = true;
		following[follow] = pointer;
	}

	void updateFollow(){
		int followers = 0;
		float followx = 0.0f, followy = 0.0f;
		for(int i = 0; i < 4; i++){
			if(followEnabled[i]){
				followers++;
				followx += following[i]->get2Dpos()->x;
				followy += following[i]->get2Dpos()->y;

				printf("pos x = %f\n", following[i]->pos[0]);

				printf("[%i] followx %f \n",i, followx);
				printf("[%i] followy %f \n",i, followy);
			}
		}

		printf("followers %i\n", followers);

		followx /= followers;
		followy /= followers;
		setX(followx);
		setY(followy);


		float maxDistance = 0;
		bvec2<float>* last = NULL;
		if(followers > 1){
			for(int i = 0; i < 4; i++){
				if(followEnabled[i]){
					if(maxDistance == 0){
						last = (following[i]->get2Dpos());
						maxDistance = -1;
						continue;
					}
					else
					{
						printf("dist : %f\n",  following[i]->get2Dpos()->dist(*last));
						maxDistance = std::max(maxDistance, following[i]->get2Dpos()->dist(*last));
					}
				}
			}
		}

		printf("maxDistance %f\n", maxDistance/0.75f);
		setZ(std::max((maxDistance/0.75f),(120.0f)));
	}


	mat4 getViewMat(){
		mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
		mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
		//mat4 view_mat = R * T;

		return R * T;
	}

	GLfloat* getProjMat(){
		return proj_mat;
	}

	void calculateFrustum(){
		//calculate view * proj
		glm::mat4 clipping1 = glm::make_mat4(getProjMat());// * getViewMat();
		glm::mat4 clipping2 = glm::make_mat4(getViewMat().m);
		glm::mat4 clipping = clipping1 * clipping2;
		const float *pSource = (const float*)glm::value_ptr(clipping);
		float clip[16];
		for(int i = 0; i < 16; i++)
			clip[i] = pSource[i];

		//calculate planes
		float t;

		 /* Extract the numbers for the RIGHT plane */
		   frustum[0][0] = clip[ 3] - clip[ 0];
		   frustum[0][1] = clip[ 7] - clip[ 4];
		   frustum[0][2] = clip[11] - clip[ 8];
		   frustum[0][3] = clip[15] - clip[12];
		 /* Normalize the result */
		   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2]    * frustum[0][2] );
		   frustum[0][0] /= t;
		   frustum[0][1] /= t;
		   frustum[0][2] /= t;
		   frustum[0][3] /= t;
		 /* Extract the numbers for the LEFT plane */
		   frustum[1][0] = clip[ 3] + clip[ 0];
		   frustum[1][1] = clip[ 7] + clip[ 4];
		   frustum[1][2] = clip[11] + clip[ 8];
		   frustum[1][3] = clip[15] + clip[12];
		 /* Normalize the result */
		   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2]    * frustum[1][2] );
		   frustum[1][0] /= t;
		   frustum[1][1] /= t;
		   frustum[1][2] /= t;
		   frustum[1][3] /= t;
		 /* Extract the BOTTOM plane */
		   frustum[2][0] = clip[ 3] + clip[ 1];
		   frustum[2][1] = clip[ 7] + clip[ 5];
		   frustum[2][2] = clip[11] + clip[ 9];
		   frustum[2][3] = clip[15] + clip[13];
		 /* Normalize the result */
		   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2]    * frustum[2][2] );
		   frustum[2][0] /= t;
		   frustum[2][1] /= t;
		   frustum[2][2] /= t;
		   frustum[2][3] /= t;
		 /* Extract the TOP plane */
		   frustum[3][0] = clip[ 3] - clip[ 1];
		   frustum[3][1] = clip[ 7] - clip[ 5];
		   frustum[3][2] = clip[11] - clip[ 9];
		   frustum[3][3] = clip[15] - clip[13];
		 /* Normalize the result */
		   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2]    * frustum[3][2] );
		   frustum[3][0] /= t;
		   frustum[3][1] /= t;
		   frustum[3][2] /= t;
		   frustum[3][3] /= t;
		 /* Extract the FAR plane */
		   frustum[4][0] = clip[ 3] - clip[ 2];
		   frustum[4][1] = clip[ 7] - clip[ 6];
		   frustum[4][2] = clip[11] - clip[10];
		   frustum[4][3] = clip[15] - clip[14];
		 /* Normalize the result */
		   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2]    * frustum[4][2] );
		   frustum[4][0] /= t;
		   frustum[4][1] /= t;
		   frustum[4][2] /= t;
		   frustum[4][3] /= t;
		 /* Extract the NEAR plane */
		   frustum[5][0] = clip[ 3] + clip[ 2];
		   frustum[5][1] = clip[ 7] + clip[ 6];
		   frustum[5][2] = clip[11] + clip[10];
		   frustum[5][3] = clip[15] + clip[14];
		 /* Normalize the result */
		   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2]    * frustum[5][2] );
		   frustum[5][0] /= t;
		   frustum[5][1] /= t;
		   frustum[5][2] /= t;
		   frustum[5][3] /= t;

	}

	bool PointInFrustum( float x, float y, float z )
	{
	  int p;
	  for( p = 0; p < 6; p++ )
	    if( frustum[p][0] * x + frustum[p][1] * y + frustum[p][2] * z + frustum[p][3]    <= 0 )
	      return false;
	  return true;
	}
	
};

#endif

