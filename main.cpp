/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| This demo uses the Assimp library to load a mesh from a file, and supports   |
| many formats. The library is VERY big and complex. It's much easier to write |
| a simple Wavefront .obj loader. I have code for this in other demos. However,|
| Assimp will load animated meshes, which will we need to use later, so this   |
| demo is a starting point before doing skinning animation                     |
\******************************************************************************/
#include "maths_funcs.h"
#include "gl_utils.h"
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "stb_image.h" //Sean image loader from 09
#include "bvec.h"
#include "unit.h"
#include "enemy.h"
#include "player.h"
#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"
#define VERTEX_SHADER_FILE "test_vs.glsl"
#define FRAGMENT_SHADER_FILE "test_fs.glsl"
#define MESH_FILE "mergedjet3.obj"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

bool load_texture (const char* file_name, GLuint* tex) {
	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf (stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	glGenTextures (1, tex);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, *tex);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap (GL_TEXTURE_2D);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	return true;
}

/* load a mesh using the assimp library */
bool load_mesh (const char* file_name, GLuint* vao, int* point_count) {
	const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate);
	if (!scene) {
		fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}
	printf ("  %i animations\n", scene->mNumAnimations);
	printf ("  %i cameras\n", scene->mNumCameras);
	printf ("  %i lights\n", scene->mNumLights);
	printf ("  %i materials\n", scene->mNumMaterials);
	printf ("  %i meshes\n", scene->mNumMeshes);
	printf ("  %i textures\n", scene->mNumTextures);
	
	/* get first mesh in file only */
	const aiMesh* mesh = scene->mMeshes[0];
	printf ("    %i vertices in mesh[0]\n", mesh->mNumVertices);
	
	/* pass back number of vertex points in mesh */
	*point_count = mesh->mNumVertices;
	
	/* generate a VAO, using the pass-by-reference parameter that we give to the
	function */
	glGenVertexArrays (1, vao);
	glBindVertexArray (*vao);
	
	/* we really need to copy out all the data from AssImp's funny little data
	structures into pure contiguous arrays before we copy it into data buffers
	because assimp's texture coordinates are not really contiguous in memory.
	i allocate some dynamic memory to do this. */
	GLfloat* points = NULL; // array of vertex points
	GLfloat* normals = NULL; // array of vertex normals
	GLfloat* texcoords = NULL; // array of texture coordinates
	if (mesh->HasPositions ()) {

		int numbers = 0;
		float xnumber = 0.0f;
		float ynumber = 0.0f;
		float znumber = 0.0f;
		points = (GLfloat*)malloc (*point_count * 3 * sizeof (GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
			
			numbers++;
			xnumber += points[i * 3];
			ynumber += points[i * 3 + 1];
			znumber += points[i * 3 + 2];
		}

	printf("xmiddle: %f\n",xnumber/numbers);

	printf("ymiddle: %f\n",ynumber/numbers);

	printf("zmiddle: %f\n",znumber/numbers);
	}
	if (mesh->HasNormals ()) {
		normals = (GLfloat*)malloc (*point_count * 3 * sizeof (GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords (0)) {
		texcoords = (GLfloat*)malloc (*point_count * 2 * sizeof (GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}
	
	/* copy mesh data into VBOs */
	if (mesh->HasPositions ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			points,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		free (points);
	}
	if (mesh->HasNormals ()) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			3 * *point_count * sizeof (GLfloat),
			normals,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);
		free (normals);
	}
	if (mesh->HasTextureCoords (0)) {
		GLuint vbo;
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (
			GL_ARRAY_BUFFER,
			2 * *point_count * sizeof (GLfloat),
			texcoords,
			GL_STATIC_DRAW
		);
		glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
		free (texcoords);
	}
	if (mesh->HasTangentsAndBitangents ()) {
		// NB: could store/print tangents here
	}
	
	aiReleaseImport (scene);
	printf ("mesh loaded\n");
	
	return true;
}

/**
	look at mouse airplane mode
	should have a unit class passed
*/
mat4 lookAtMouse(float *unit_pos) {
	double xpos, ypos;
	glfwGetCursorPos(g_window, &xpos, &ypos);
	printf ("MOUSE: x: %f y: %f\n", xpos, ypos);
	float xrel, yrel;
	xrel = xpos/g_gl_width;
	yrel = ypos/g_gl_height;
	printf ("MOUSE_REL: x: %f y: %f\n", xrel, yrel);
	float xmid, ymid;
	xmid = 0.5f;
	ymid = 0.5f;

	bvec2<float> pos = bvec2<float>(xmid, ymid);
	bvec2<float> point = bvec2<float>(xrel, yrel);
	bvec2<float> delta = pos - point;
	float angle;
	angle = atan2(delta.x, delta.y);
	printf ("ANGLE: %f\n", angle);
	bvec2<float> dir = bvec2<float>(cos(angle), sin(angle));
	printf ("DIR: x: %f y: %f\n", dir.x, dir.y);

	float unit_yaw = dir.x*90;
	float unit_roll = -90+dir.y*90;
	float unit_pitch = 0.0f;//dir.y*100 

	//unit_pos[2] += cam_speed * elapsed_seconds; 
	mat4 T = translate (identity_mat4 (), vec3 (unit_pos[0], unit_pos[1], unit_pos[2])); // unit translation

	mat4 R1 = rotate_x_deg (identity_mat4 (), unit_yaw); // 
	mat4 R2 = rotate_y_deg (identity_mat4 (), unit_roll); // 
	mat4 R3 = rotate_z_deg (identity_mat4 (), unit_pitch); // 
	return R1 * R2 * R3 * T;
}

//return models rotation
mat4 planeAngle(float angle, float *unit_pos){
	angle = angle;// * 3.33f;
	float unit_yaw = angle;///-45+(angle*33.0f);
	float unit_roll = 180.0f;//(angle*33.0f);
	float unit_pitch = 180.0f;//dir.y*100 ;
	printf ("ANGLE: %f\n", angle);
	//printf ("DIR: x: %f y: %f\n", dir.x, dir.y);
	printf ("UNIT_YAW: %f\n", unit_yaw);

	//unit_pos[2] += cam_speed * elapsed_seconds; 
	mat4 T = translate (identity_mat4 (), vec3 (-5.481503f/2, 0.251155f/2, 0.004155f/2));//unit_pos[0], unit_pos[1], unit_pos[2])); // unit translation

	mat4 R1 = rotate_y_deg (identity_mat4 (), unit_yaw); // 
	mat4 R2 = rotate_x_deg (identity_mat4 (), unit_roll); // 
	mat4 R3 = rotate_z_deg (identity_mat4 (), unit_pitch); // 
	//return rotate_x_deg(T, angle * 20.f);
	return rotate_z_deg(rotate_x_deg(R2 * R3 * T, angle), unit_yaw);
}
bvec2<float> planeDirection(float angle){
	return bvec2<float>(cos(angle), sin(angle));
}

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
	glViewport (0, 0, g_gl_width, g_gl_height);

	/* load the mesh using assimp */
	GLuint monkey_vao;
	int monkey_point_count = 0;
	assert (load_mesh (MESH_FILE, &monkey_vao, &monkey_point_count));

	
/*-------------------------------CREATE SHADERS-------------------------------*/
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);
	
	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
	// input variables
	float near = 0.1f; // clipping plane
	float far = 1200.0f; // clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan (fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};

	/* shader background */
	background bg = background("background_vs.glsl", "background_fs.glsl");
	
	/* tex test */
	int tex_location = glGetUniformLocation (shader_programme, "basic_texture");
	assert (tex_location > -1);

	/* tricky bit here - remember to set second sampler to use slot 1 */
	glUniform1i (tex_location, 0);
	
	GLuint monkey_tex;
	glActiveTexture (GL_TEXTURE0);
	assert (load_texture("AudioEquipment0039_2_S.jpg", &monkey_tex));
	glBindTexture (GL_TEXTURE_2D, monkey_tex);

	float cam_speed = 25.0f; // 1 unit per second
	float cam_yaw_speed = 33.0f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.0f, 100.0f}; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	//vec3 view_dir = view_mat * cam_pos;

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);

	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

	bg.setViewMatrix(view_mat);
	
	int unit_mat_location = glGetUniformLocation (shader_programme, "unit");
	/*glUseProgram (shader_programme);
	glUniformMatrix4fv (unit_mat_location, 1, GL_FALSE, unit_mat.m);*/

	int cam_vec3_location = glGetUniformLocation (shader_programme, "cam");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (cam_vec3_location, 1, GL_FALSE, cam_pos);
	
	//test unit class
	unit *Units = (unit*) malloc(20 * sizeof(unit));

	for(int i=0;i<20;i++){
		Units[i] = unit(i*10.0f,fmod(i*10.0f, 50.f),0.0f);
		Units[i].setTex(monkey_tex);
		Units[i].setVao(monkey_vao, monkey_point_count);
		Units[i].set_mat_location(unit_mat_location);
	}

	Units[1].setRotationSpeed(180.0f);
	enemy test = enemy(0, &Units[1]);
	test.setTarget(&Units[0]);

	player playertest = player(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, &Units[0]);


	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		
		/*mat4 unit_mat = lookAtMouse(unit_pos);
		glUniformMatrix4fv (unit_mat_location, 1, GL_FALSE, unit_mat.m);*/
	
		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);
		
		bg.draw();

		glUseProgram (shader_programme);
		/*glBindVertexArray (monkey_vao);
		glDrawArrays (GL_TRIANGLES, 0, monkey_point_count);*/
		for(int i = 0; i < 20; i++)
			Units[i].draw();
		
		// update other events like input handling 
		glfwPollEvents ();
		
		// control keys
		bool cam_moved = false;
		if (glfwGetKey (g_window, GLFW_KEY_A)) {
			cam_pos[0] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_D)) {
			cam_pos[0] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)) {
			cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)) {
			cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_W)) {
			cam_pos[2] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_S)) {
			cam_pos[2] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}

		cam_pos[0] = Units[0].get2Dpos().x;
		cam_pos[1] = Units[0].get2Dpos().y;
		cam_moved = true;
		/*if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
			cam_yaw += cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
			cam_yaw -= cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}*/

		//Test angle
		/*if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
			unit_angle += 360.0f  * elapsed_seconds;
			//cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
			unit_angle -= 360.0f * elapsed_seconds;
			//cam_moved = true;
		}
		unit_angle = fmod(unit_angle, 360);
		//unit_angle = fmod(unit_angle, 360.0f);
		if (glfwGetKey(g_window, GLFW_KEY_UP)){
			unit_velocity.x += 0.20f * cos(0.0174532925 * unit_angle) * elapsed_seconds;
			unit_velocity.y += 0.20f * sin(0.0174532925 * unit_angle) * elapsed_seconds;
			
			if (unit_velocity.x > 2.0f) unit_velocity.x = 2.0f;
			if (unit_velocity.x < -2.0f) unit_velocity.x = -2.0f;
			if (unit_velocity.y > 2.0f) unit_velocity.y = 2.0f;
			if (unit_velocity.y < -2.0f) unit_velocity.y = -2.0f;
			
		}*/
		for(int i = 0; i < 20; i++)
			if(i != 1 && i != 0)
				Units[i].setAngle(1.0f*i);

		test.update(elapsed_seconds);
		playertest.update(elapsed_seconds);
		
		/*unit_pos[0] += unit_velocity.x;
		unit_pos[1] += unit_velocity.y;
		unit_velocity /= 1.01f;*/

		/*mat4 unit_mat = translate (identity_mat4 (), vec3 (unit_pos[0], unit_pos[1], unit_pos[2])) * planeAngle(unit_angle, unit_pos);
		glUniformMatrix4fv (unit_mat_location, 1, GL_FALSE, unit_mat.m);*/
		
		// update view matrix
		if (cam_moved) {
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw); // 
			mat4 view_mat = R * T;
			glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
			bg.setViewMatrix(view_mat);
		}
		
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
