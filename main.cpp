#include "maths_funcs.h"
#include "gl_utils.h"
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "stb_image.h" //Sean image loader from 09
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"
#define VERTEX_SHADER_FILE "test_vs.glsl"
#define FRAGMENT_SHADER_FILE "test_fs.glsl"
#define MESH_FILE "mergedjet3.obj"

//Game classes
#include "helpers.h"
#include "bufferHelper.h"
#include "texturehelper.h"
#include "bvec.h"
#include "unit.h"
#include "enemy.h"
#include "player.h"
#include "background.h"
#include "mountains2.h"
#include "cam.h"
#include "sprite.h"
#include "angular.h"
#include "bullet.h"
#include "bulletController.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());

	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	
	//Enable blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable( GL_BLEND );
	
	glEnable( GL_MULTISAMPLE );

	//use alpha testing instead for depth buffering of alpha objects
	//glEnable(GL10.GL_ALPHA_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

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
	
	/* shader background */
	background bg = background("background_vs.glsl", "background_fs.glsl");
	
	/* tex test */
	int tex_location = glGetUniformLocation (shader_programme, "basic_texture");
	assert (tex_location > -1);

	/* tricky bit here - remember to set second sampler to use slot 1 */
	
	GLuint monkey_tex;
	glActiveTexture (GL_TEXTURE0);
	assert (load_texture("AudioEquipment0039_2_S.jpg", &monkey_tex));
	glBindTexture (GL_TEXTURE_2D, monkey_tex);

	cam camera = cam();

	//vec3 view_dir = view_mat * cam_pos;

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, camera.getViewMat().m);

	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);


	//proj_mat = camera.getProjMat();
	for(int i = 0; i < 16 ; i ++){
		printf("Cam get[%i] %f\n", i, camera.getProjMat()[i]);		
	}
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, camera.getProjMat());
	
	int unit_mat_location = glGetUniformLocation (shader_programme, "unit");
	
	//test unit class
	unit *Units = (unit*) malloc(20 * sizeof(unit));
	
	for(int i=0;i<20;i++){
		Units[i] = unit(i*10.0f,fmod(i*10.0f, 50.f),0.0f);
		Units[i].setTex(monkey_tex);
		Units[i].setVao(monkey_vao, monkey_point_count);
		Units[i].set_mat_location(unit_mat_location);
	}

	int numbullets = 0;
	int maxBullets = 5000000;
	bullet* bullets = (bullet*) malloc(maxBullets * sizeof(bullet));

	mountain Mo = mountain(128,128);

	Units[1].setRotationSpeed(180.0f);

	enemy *enemytest = (enemy*) malloc(19 * sizeof(enemy));

	for(int i=0;i<19;i++){
		enemytest[i] = enemy(0, &Units[i+1]);
		enemytest[i].setTarget(&Units[0]);
	}

	player playertest = player(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, &Units[0]);


	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);
		
		bg.draw();
		glClear (GL_DEPTH_BUFFER_BIT);

		Mo.draw(elapsed_seconds, camera.getViewMat());
		glClear (GL_DEPTH_BUFFER_BIT);

		
		shaderhelper shader = shaderhelper ("shaders/bullet_vs.glsl", "shaders/bullet_fs.glsl", 2);
		shader.use();
		shader.setLocation("view", 0);
		shader.setLocation("proj", 1);
		shader.bindLocationFloatarray(camera.getViewMat().m, 0);
		shader.bindLocationFloatarray(camera.getProjMat(), 1);

		texturehelper bullettest = texturehelper("bullet.png");
		bullettest.bind();

		glUseProgram(shader_programme);
		for(int i = 0; i < 20; i++)
			Units[i].draw();
		

		shader.use();
		sprite spritetest = sprite(bullettest, 3.0f);
		spritetest.draw();

		if(numbullets > 0)
			bullets[0].s.bind();

		camera.calculateFrustum();
		for(int i = 0; i < numbullets; i++){
			bullets[i].update();
			if(camera.PointInFrustum(bullets[i].a.getX(), bullets[i].a.getY(), 0.0f)){
				bullets[i].draw();
				//glDrawArrays (GL_TRIANGLES, 0, 6);
			}
		}

		glUseProgram(shader_programme);		
		// update other events like input handling 
		glfwPollEvents ();
		
		// control keys
		bool cam_moved = false;
		if (glfwGetKey (g_window, GLFW_KEY_A)) {
			//cam_pos[0] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_D)) {
			//cam_pos[0] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)) {
			//cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)) {
			//cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_W)) {
			camera.setDeltaZ(-elapsed_seconds);
			cam_moved = true;
		}
		if (glfwGetKey (g_window, GLFW_KEY_S)) {
			camera.setDeltaZ(elapsed_seconds);
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_SPACE)) {
			float displacement = 5.481503f;
			float dx = cos(ONE_DEG_IN_RAD * Units[0].getAngle());
			float dy = sin(ONE_DEG_IN_RAD * Units[0].getAngle());
			for(int i=0;i<150;i++){
				bullets[numbullets] = bullet(spritetest, -30.0f + Units[0].getAngle() + i*(15.0f), Units[0].pos[0] + (displacement * dx) + Units[0].velocity.x, Units[0].pos[1] + (displacement * dy) + Units[0].velocity.y, 15.0f+(i/10.0f));
				bullets[numbullets].addPlaneSpeed(Units[0].velocity.x, Units[0].velocity.y);
				numbullets++;
			}
			if(numbullets > maxBullets)
				numbullets = 0;

			printf("numbullets %i\n", numbullets);
		}


		cam_moved = true;
		camera.getViewMat();
	
		for(int i=0; i<19; i++)
			enemytest[i].update(elapsed_seconds);
		playertest.update(elapsed_seconds);
		
		camera.setX(Units[0].get2Dpos().x);
		camera.setY(Units[0].get2Dpos().y);

		// update view matrix
		if (cam_moved) {
			mat4 view_mat = camera.getViewMat();
			glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
			bg.setViewMatrix(view_mat);
			Mo.update(view_mat, camera.getProjMat());
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