/*#version 400

in vec2 texture_cord;
in vec3 normal;
uniform sampler2D basic_texture;
in vec2 st;
out vec4 frag_colour;

in vec3 dir;
in vec3 position_eye, normal_eye;

void main() {
	vec4 specular = vec4(0.6,0.4,0.3,0.5);
	vec4 ambient = vec4(0.05,0.05,0.05,1);
	float shininess = 0.2;

	vec4 texel_a = texture (basic_texture, st);
	vec4 texa = texel_a;

	vec4 spec = vec4(0.0);
	vec3 n = normalize(normal_eye);
	vec3 e = normalize(position_eye);

	float intensity = max(dot(n,dir), 0.0);

	if (intensity > 0.0) {
		vec3 h = normalize(dir + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = specular * pow(intSpec,shininess);

		//vec3 h = normalize(l_dir + e);  
        // compute the specular term into spec
        //float intSpec = max(dot(h,n), 0.0);
        //spec = specular * pow(intSpec,shininess);
	}
	//old texa texa.rgb *= normal.xyz / 2.0 + 0.5;
	frag_colour = max(intensity * texel_a + spec, ambient);
}*/
#version 400

in vec3 position_eye, normal_eye;

uniform mat4 view_mat;

uniform sampler2D basic_texture;
in vec2 st;

// fixed point light properties
vec3 light_position_world  = vec3 (0.0, 0.0, 2.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 La = vec3 (0.2, 0.2, 0.2); // grey ambient colour
  
// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (0.0, 1.0, 0.0); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 50.0; // specular 'power'

out vec4 fragment_colour; // final colour of surface

void main () {
	// ambient intensity
	vec3 Ia = La * Ka;

	// diffuse intensity
	// raise light position to eye space
	vec3 light_position_eye = vec3 (view_mat * vec4 (light_position_world, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);
	float dot_prod = dot (direction_to_light_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = Ld * Kd * dot_prod; // final diffuse intensity
	
	// specular intensity
	vec3 surface_to_viewer_eye = normalize (-position_eye);
	
	//vec3 reflection_eye = reflect (-direction_to_light_eye, normal_eye);
	//float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
	//dot_prod_specular = max (dot_prod_specular, 0.0);
	//float specular_factor = pow (dot_prod_specular, specular_exponent);
	
	// blinn
	vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
	float dot_prod_specular = max (dot (half_way_eye, normal_eye), 0.0);
	float specular_factor = pow (dot_prod_specular, specular_exponent);
	
	vec3 Is = Ls * Ks * specular_factor; // final specular intensity
	
	// final colour
	vec4 texel_a = texture (basic_texture, st);
	fragment_colour = texel_a * vec4 (Is + Id + Ia, 1.0);
}