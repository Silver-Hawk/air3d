/*
	Basic unit class, this is like an entity but with 
	rotations, models and textures always attached
*/
#ifndef __TEXTURE_HELPER_CLASS_H__
#define __TEXTURE_HELPER_CLASS_H__

class texturehelper {
	public:
	GLuint tex, tex_location;
	
	GLint shader;

	texturehelper(){

	}

	texturehelper(const char* text_string){
		//get current shader
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

		tex_location = glGetUniformLocation (shader, "basic_texture");

		//might be needed
		//glUniform1i (tex_location, 0);

		glActiveTexture (GL_TEXTURE0);
		assert (load_texture(text_string, &tex));
	}

	void bind(){
		glBindTexture (GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
};


#endif