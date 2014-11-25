#ifndef __ANGULAR_CLASS_H__
#define __ANGULAR_CLASS_H__

#include "shaderhelper.h"

class angular {
	public:
	float pos[3]; // has 3 variables, x = 0, y = 1, z = 2
	float middle_pos[3];
	float pitch, roll, yaw;
	float angle; //current angle


	angular(){

		//normal middles for a sprite
		middle_pos[0] = 0.5f;
		middle_pos[1] = 0.5f;
		middle_pos[2] = 0.0f;
	}

	void setAngle (float angle) { this->angle = angle; }
	float getAngle() { return angle; }

	void setPos (float x, float y, float z) { this->pos[0] = x; this->pos[1] = y; this->pos[2] = z; }

	void setPitch(float pitch) { this->pitch = pitch; }
	void setRoll(float roll) { this->roll = roll; }
	void setYaw(float yaw) { this->yaw = yaw; }

	void addX(float diff) { pos[0] += diff; }
	void addY(float diff) { pos[1] += diff; }

	float getX(){ return pos[0]; };
	float getY(){ return pos[1]; };

	mat4 bulletMat(){
		mat4 rotate = rotate_z_deg(translate (identity_mat4 (), vec3 (middle_pos[0], middle_pos[1], middle_pos[2])), angle - 90);
		return translate (identity_mat4 (), vec3 (pos[0], pos[1], pos[2])) * rotate;
	}

	void bindBulletMat(){
		GLint shader;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

		int unit_location = glGetUniformLocation (shader, "unit");
		glUniformMatrix4fv (unit_location, 1, GL_FALSE, bulletMat().m);
	}
};


#endif