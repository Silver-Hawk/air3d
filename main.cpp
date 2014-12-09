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
#include "worldController.h"

//world controller needs to be visible for all
worldController WC;

int* getWorldBounds(){
	return WC.getBoundsArray();
}

//make shader controller visible
#include "shadercontroller.h"
shadercontroller* SC;

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
#include "water.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());

	//game controllers
	//set world bounds
	WC = worldController(-2000, 0, 2000, 5000);
	bulletController BC = bulletController();
	SC = new shadercontroller();

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

	/*GLuint monkey_vao;
	int monkey_point_count = 0;
	assert (load_mesh (MESH_FILE, &monkey_vao, &monkey_point_count));*/

	
/*-------------------------------CREATE SHADERS-------------------------------*/
	/*GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);*/
	
	/* shader background */
	background bg = background("background_vs.glsl", "background_fs.glsl");
	
	/* tex test */
	/* load the mesh using assimp */
	SC->use(UNIT_SHADER);
	bufferhelper *unitBuf = new bufferhelper();
	unitBuf->loadModel(MESH_FILE);
	texturehelper unitTex = texturehelper("AudioEquipment0039_2_S.jpg");

	/*int tex_location = glGetUniformLocation (shader_programme, "basic_texture");
	assert (tex_location > -1);

	tricky bit here - remember to set second sampler to use slot 1 
	GLuint monkey_tex;
	glActiveTexture (GL_TEXTURE0);
	assert (load_texture("AudioEquipment0039_2_S.jpg", &monkey_tex));
	glBindTexture (GL_TEXTURE_2D, monkey_tex);*/

	cam camera = cam();

	/*int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, camera.getViewMat().m);

	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);

	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, camera.getProjMat());
	
	int unit_mat_location = glGetUniformLocation (shader_programme, "unit");*/
	
	//test unit class
	unit *Units = (unit*) malloc(20 * sizeof(unit));
	
	for(int i=0;i<20;i++){
		Units[i] = unit(i*10.0f,fmod(i*10.0f, 50.f),0.0f);
		/*Units[i].setTex(monkey_tex);
		Units[i].setVao(monkey_vao, monkey_point_count);
		Units[i].set_mat_location(unit_mat_location);*/
	}


	mountain Mo = mountain(128,128);
	

	Units[1].setRotationSpeed(30.0f);


	enemy *enemytest = (enemy*) malloc(19 * sizeof(enemy));

	for(int i=0;i<19;i++){
		enemytest[i] = enemy(0, &Units[i+1]);
		enemytest[i].setTarget(&Units[0]);
	}

	water W = water();
	W.update(camera.getViewMat(), camera.getProjMat());

	camera.setFollow(0, &Units[0]);
	//camera.setFollow(1, &Units[1]);

	player playertest = player(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, &Units[0]);

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		
		//calculate actions
		// update other events like input handling 
		glfwPollEvents ();

		for(int i=0; i<19; i++)
			enemytest[i].update(elapsed_seconds);
		playertest.update(elapsed_seconds);
		
		camera.updateFollow();

		SC->updateShaders(camera.getViewMat(), camera.getProjMat(), true);
		
		shaderhelper shader = shaderhelper ("shaders/bullet_vs.glsl", "shaders/bullet_fs.glsl", 2);
		shader.use();
		shader.setLocation("view", 0);
		shader.setLocation("proj", 1);
		shader.bindLocationFloatarray(camera.getViewMat().m, 0);
		shader.bindLocationFloatarray(camera.getProjMat(), 1);

		texturehelper bullettest = texturehelper("bullet.png");
		bullettest.bind();

		shader.use();
		sprite spritetest = sprite(bullettest, 3.0f);
		spritetest.draw();


		BC.update();


		if (glfwGetKey (g_window, GLFW_KEY_SPACE)) {
			float displacement = 5.481503f;
			float dx = cos(ONE_DEG_IN_RAD * Units[0].getAngle());
			float dy = sin(ONE_DEG_IN_RAD * Units[0].getAngle());
			int num_bullets = 100;
			for(int i=0;i<num_bullets;i++){
				float splitAngle, diffAngle;
				int extradiff;
				if(num_bullets%2 == 0){
					splitAngle = 30.0f/2;
					diffAngle = splitAngle / ((float) num_bullets);
					extradiff = 0; 
				}
				else
				{
					splitAngle = 30.0f/2.0f;
					diffAngle = splitAngle / ((float) num_bullets);
					extradiff = 1;
				}

				BC.add(new bullet(spritetest, -splitAngle + Units[0].getAngle() + (diffAngle*(i+extradiff)), Units[0].pos[0] + (displacement * dx) + Units[0].velocity.x,
			    Units[0].pos[1] + (displacement * dy) + Units[0].velocity.y, 5.0f, i));
				BC.last->addPlaneSpeed(Units[0].velocity.x, Units[0].velocity.y);
			}
		}

		//DRAWING
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);

		//calculate view frustum
		camera.calculateFrustum();
		
		bg.draw();
		glClear (GL_DEPTH_BUFFER_BIT);

		for(int i = 0; i < 2; i++){
			// update view matrix

			mat4 view_mat = camera.getViewMat();
			//glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
			
			if(i%2 == 1){
				glFrontFace (GL_CW);
				glDisable(GL_STENCIL_TEST);
				glEnable(GL_DEPTH_TEST);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable( GL_BLEND );

				W.update(camera.getViewMat(), camera.getProjMat());
			    W.draw();

			    glDisable( GL_BLEND );
			    glBlendFunc(GL_ONE, GL_ZERO);

			    glEnable( GL_MULTISAMPLE );
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);

				glFrontFace (GL_CCW);
				//camera.inverseProjMatOnY(); //enable when  the floor is made
			}
			else
			{
			    glFrontFace (GL_CW);
				glDisable(GL_DEPTH_TEST);			
				//enable stencil testing
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			    glStencilMask(0xFF); // Write to stencil buffer
			    glDepthMask(GL_FALSE); // Don't write to depth buffer
			    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
				
			    //draw water
			    W.update(camera.getViewMat(), camera.getProjMat());
			    W.draw();

				glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
			    glStencilMask(0x00); // Don't write anything to stencil buffer
			    glDepthMask(GL_TRUE); // Write to depth buffer
				glEnable(GL_DEPTH_TEST);
				//camera.inverseProjMatOnY(); //enable when  the floor is made
			}
			if(i%2 == 1)
				SC->updateShaders(camera.getViewMat(), camera.getProjMat(), false);
			else
				SC->updateShaders(camera.getViewMat(), camera.getProjMat(), true);


			bg.setViewMatrix(view_mat);
			
			Mo.draw(elapsed_seconds, camera.getViewMat());
			//glClear (GL_DEPTH_BUFFER_BIT);
			
			//glUseProgram (shader_programme);
			//glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, camera.getProjMat());
	
			SC->use(UNIT_SHADER);
			unitBuf->bindModel();
			for(int j = 0; j < 20; j++){
				Units[j].draw();
				unitTex.bind();
				unitBuf->drawArrays();
			}
			
			shader.use();
			glFrontFace (GL_CCW);
			if(i%2 == 1){
				BC.draw(camera);
			}

			SC->use(UNIT_SHADER);
			
			if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose (g_window, 1);
			}

		}
		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}