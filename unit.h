/*
	Basic Vec2 functions wrapped in bvec2 object
*/
#ifndef __UNIT_CLASS_H__
#define __UNIT_CLASS_H__

#include <cmath>
#include "bvec.h"

template <class T>
class bvec2 {
public:
	//positions and rotations and velocity
	float* pos; // has 3 variables, x = 0, y = 1, z = 2
	float pitch, roll, yaw;
	float angle; //current angle
	bvec2<float> unit_velocity;
	
	//models and textures
	GLuint model_vao;
	int model_point_count;

	GLuint texture;

	//bind location
	int unit_mat_location;



	unit() : {
		float pos = malloc (*point_count * 3 * sizeof (GLfloat));
	}

	unit(float x, float y, float z) : {
		float pos = malloc (*point_count * 3 * sizeof (GLfloat));
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}

	void set_mat_location(int loc) {
		unit_mat_location = loc;
	}

	mat4 angle(float angle){
		yaw = angle;///-45+(angle*33.0f);
		roll = 180.0f;//(angle*33.0f);
		pitch = 180.0f;//dir.y*100 ;

		//unit_pos[2] += cam_speed * elapsed_seconds; 
		//should use unit_pos instead of static floats
		mat4 T = translate (identity_mat4 (), vec3 (-5.481503f/2, 0.251155f/2, 0.004155f/2));//unit_pos[0], unit_pos[1], unit_pos[2])); // unit translation

		mat4 R1 = rotate_y_deg (identity_mat4 (), unit_yaw); // 
		mat4 R2 = rotate_x_deg (identity_mat4 (), unit_roll); // 
		mat4 R3 = rotate_z_deg (identity_mat4 (), unit_pitch); // 
		//return rotate_x_deg(T, angle * 20.f);
		return rotate_z_deg(rotate_x_deg(R2 * R3 * T, angle), unit_yaw);
	}

	void draw(){
		mat4 unit_mat = translate (identity_mat4 (), vec3 (pos[0], pos[1], pos[2])) * angle(angle);
		glUniformMatrix4fv (unit_mat_location, 1, GL_FALSE, unit_mat.m);
	}
	
};

typedef bvec2<float> bvec2f;
typedef bvec2<double> bvec2d;


#endif