/*
	Basic unit class, this is like an entity but with 
	rotations, models and textures always attached
*/
#ifndef __UNIT_CLASS_H__
#define __UNIT_CLASS_H__

#include <cmath>
#include "bvec.h"

#define DEG_TO_RAD 0.0174532925
#define MAX_SPEED 4.0f

#define WATER_BOUNCE_FORCE 0.85

class unit {
public:
	//positions and rotations and velocity
	float pos[3]; // has 3 variables, x = 0, y = 1, z = 2
	float pitch, roll, yaw;
	float angle; //current angle
	bvec2<float> velocity;
	
	//game factors
	float rotation_speed;
	float velocity_speed;

	//models and textures
	GLuint model_vao;
	int model_point_count;

	GLuint texture;

	//bind location
	int unit_mat_location;

	unit() {
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;
		rotation_speed = 360.0f;
		velocity_speed = 0.50f;
		angle = 0.0f;
	}

	unit(float x, float y, float z) {
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		rotation_speed = 360.0f;
		velocity_speed = 0.50f;
		angle = 0.0f;
	}

	void set_mat_location(int loc) {
		unit_mat_location = loc;
	}

	void setVao(GLuint vao, int model_points){
		model_vao = vao;
		model_point_count = model_points;
	}

	void setTex(GLuint tex){
		texture = tex;
	}

	mat4 rotateAngle(){
		yaw = angle;
		roll = 180.0f;
		pitch = 180.0f;

		//should use unit_pos instead of static floats
		mat4 T = translate (identity_mat4 (), vec3 (-5.481503f/2, 0.251155f/2, 0.004155f/2));//unit_pos[0], unit_pos[1], unit_pos[2])); // unit translation

		mat4 R2 = rotate_x_deg (identity_mat4 (), roll); // 
		mat4 R3 = rotate_z_deg (identity_mat4 (), pitch); // 
		
		return rotate_z_deg(rotate_x_deg(R2 * R3 * T, angle), yaw);
	}

	void setAngle(float newa){
		angle = newa;
	}

	float getRotationSpeed(){
		return rotation_speed;
	}

	void setRotationSpeed(float speed){
		rotation_speed = speed;
	}

	/*
		Allign current angle towards a target angle
		based on deltatime
	*/
	void alignAngle(float delta, float targetangle) {
		float deltaangle = targetangle - angle;

		while (deltaangle < 0.0f)
			deltaangle += 360.0f;

		float use = std::min(deltaangle, rotation_speed * delta);
		if(deltaangle > 180.0f){
			addAngle(-use);
		}else{
			addAngle(use);
		}
		if (angle < 0)
			angle += 360;
	}

	void addAngle(float delta) {
		angle += delta;
		angle = fmod(angle, 360);
		if (angle < 0)
			angle += 360;
	}

	float getAngle() {
		return angle; 
	}

	bvec2<float>* get2Dpos(){
		return (new bvec2<float> (pos[0], pos[1]));
	}

	void addVelocity(float delta){
		velocity.x += velocity_speed * cos(DEG_TO_RAD * angle);
		velocity.y += velocity_speed * sin(DEG_TO_RAD * angle);

		if (velocity.x > MAX_SPEED) velocity.x = MAX_SPEED;
		if (velocity.x < -MAX_SPEED) velocity.x = -MAX_SPEED;
		if (velocity.y > MAX_SPEED) velocity.y = MAX_SPEED;
		if (velocity.y < -MAX_SPEED) velocity.y = -MAX_SPEED;
	}

	void velocityFade(){
		velocity /= 1.01f;
	}

	void addGravity(){
		velocity.y -= 0.07f;
	}

	void applyVelocity(){
		pos[0] += velocity.x;
		pos[1] += velocity.y;
	}

	void draw(){
		//get rotations
		mat4 unit_rot = rotateAngle();
		mat4 unit_mat = translate (identity_mat4 (), vec3 (pos[0], pos[1], pos[2])) * unit_rot;
		
		SC->updateUnit(UNIT_SHADER, unit_mat);
	}

	void update(){
		//only add gravity if the plane is moving slowly
		if(velocity.x < MAX_SPEED*0.75)
			addGravity();
		velocityFade();
		applyVelocity();
		bounceOnWater();
	}

	void bounceOnWater(){
		//check water surface collision
		if(pos[1] < 0){
			pos[1] = -pos[1];
			velocity.y = -velocity.y * WATER_BOUNCE_FORCE;
			setAngle(atan2(velocity.y, velocity.x)*(180/3.14));
		}
	}


	
	//debug
	void printUnitMat(){
		mat4 unit_rot = rotateAngle();
		mat4 unit_mat = translate (identity_mat4 (), vec3 (pos[0], pos[1], pos[2])) * unit_rot;
		
		GLfloat *pointer = unit_mat.m;
		for(int i = 0; i < 16; i++)
			printf("[%i] %f\n", i, pointer[i]); 
	}
	
};


#endif