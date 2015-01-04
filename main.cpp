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
#include <string>
#include <sstream>
#include <string>
#include <iostream>

#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"
#define VERTEX_SHADER_FILE "test_vs.glsl"
#define FRAGMENT_SHADER_FILE "test_fs.glsl"
#define MESH_FILE "test1.obj"
#define MESH_FILE2 "mergedjet3.obj"

//Game classes
#include "helpers.h"
#include "worldController.h"

//world controller needs to be visible for all
worldController WC;

int* getWorldBounds(){
	return WC.getBoundsArray();
}

//make controllers visible
#include "shadercontroller.h"
shadercontroller* SC;

#include "assetsController.h"
assetscontroller* AC;

#include "particlecontroller.h"
particlecontroller *PC;

#include "bulletController.h"
bulletController* BC;

#include "unitcontroller.h"
unitcontroller* UC;

#include "collisionDetection.h"
collision *CD;

#include "enemyspawner.h"
enemyspawner *ES;


#include "weaponcontroller.h"
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
#include "water.h"
#include "font.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;

//scores
float score_seconds = 0;

int main () {
	assert (restart_gl_log ());
	assert (start_gl ());

	//game controllers
	//set world bounds
	WC = worldController(-2000, 0, 2000, 5000);
	BC = new bulletController();
	SC = new shadercontroller();
	AC = new assetscontroller();
	UC = new unitcontroller();
	CD = new collision();
	PC = new particlecontroller();

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
	
	/* shader background */
	background bg = background("background_vs.glsl", "background_fs.glsl");
	
	/* tex test */
	/* load the mesh using assimp */
	SC->use(UNIT_SHADER);
	
	bufferhelper *unitBuf = new bufferhelper(0);
	unitBuf->loadModel(MESH_FILE);
	bufferhelper *unitBuf2 = new bufferhelper(0);
	unitBuf2->loadModel(MESH_FILE);

	texturehelper* unitTex = new texturehelper("AudioEquipment0039_2_S.jpg");

	cam camera = cam();
	mountain Mo = mountain(128,128);

	UC->addPlayer(new player(GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_SPACE));
	UC->last_player->getUnit()->setBuffers(unitBuf2);
	UC->last_player->getUnit()->setTex(unitTex);
	

	ES = new enemyspawner(unitBuf, unitTex);
	/*for(int i=0;i<20;i++){
		UC->addEnemy(new enemy(MOV_SEEKING));
		UC->last_enemy->setTarget(UC->getPlayerUnit(0));
		UC->last_enemy->getUnit()->setBuffers(unitBuf);//unitBuf);
		UC->last_enemy->getUnit()->setTex(unitTex);
	}*/

	

	water W = water();

	W.update(camera.getViewMat(), camera.getProjMat());

	camera.setFollow(0, UC->getPlayerUnit(0));
	//camera.setFollow(1, UC->getEnemyUnit(0));


	//load data enabling font print
	loadFontData();

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		score_seconds += elapsed_seconds;

		_update_fps_counter (g_window);
		
		//calculate actions
		// update other events like input handling 
		glfwPollEvents ();


		UC->update(elapsed_seconds);

		camera.updateFollow();


		SC->updateShaders(camera.getViewMat(), camera.getProjMat(), true);
		
		BC->update();
		PC->update();

		//do collision detection
		CD->checkAllAll();

		ES->update(elapsed_seconds);

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
			
			if(i%2 == 1){
				glFrontFace (GL_CW);
				glDisable(GL_STENCIL_TEST);
				glEnable(GL_DEPTH_TEST);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable( GL_BLEND );

				W.update(camera.getViewMat(), camera.getProjMat());
			    W.draw();

			   /* glDisable( GL_BLEND );
			    glBlendFunc(GL_ONE, GL_ZERO);

			    glEnable( GL_MULTISAMPLE );
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);*/

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
			
			SC->use(UNIT_SHADER);
			//unitBuf->bindModel();
			//unitTex->bind();
			UC->draw();
			//unitBuf->drawArrays();
			/*for(int j = 0; j < 20; j++){
				UC->draw();
				Units[j].draw();
				unitTex.bind();
				unitBuf->drawArrays();
			}*/
			
			SC->use(SPRITE_SHADER);
			glFrontFace (GL_CCW);
			SC->getShader(SPRITE_SHADER).bindLocationFloat(1.0f, 4);
			if(i%2 == 1){
				BC->draw(camera);
				PC->draw(camera);
			}

			SC->use(UNIT_SHADER);
			if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose (g_window, 1);
			}

		}


		updateFont(std::to_string((int)score_seconds+UC->players->score), std::to_string((int)UC->last_player->health));
		drawScoresAndLife();
		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
		glFinish();
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}