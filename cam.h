#ifndef __CAM_CLASS_H__
#define __CAM_CLASS_H__

class cam {
public:
	float cam_speed; // 1 unit per second
	float cam_yaw_speed; // 10 degrees per second
	float cam_pos[3]; // don't start at zero, or we will be too close
	float cam_yaw; // y-rotation in degrees

	cam(){
		cam_speed = 25.0f; // 1 unit per second
		cam_yaw_speed = 33.0f; // 10 degrees per second
		
		// don't start at zero, or we will be too close
		cam_pos[0] = 0.0f;
		cam_pos[1] = 0.0f;
		cam_pos[2] = 120.0f;


		printf("CAM START %f %f %f\n", cam_pos[0], cam_pos[1], cam_pos[2]);

		cam_yaw = 0.0f; // y-rotation in degrees
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
		printf("Cam pos[2] = %f, and n is %i\n", cam_pos[2]);
	}


	mat4 getViewMat(){
		mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
		mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
		//mat4 view_mat = R * T;

		return R * T;
	}
	
};

#endif

