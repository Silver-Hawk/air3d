#version 400

in vec2 texture_cord;
in vec3 normal;
uniform sampler2D basic_texture;
in vec4 pos;
in vec2 st;
out vec4 frag_colour;
in mat4 invTran;
in vec3 eyePos;
#define shininess 20.0
void main (void)
{

vec3 vNormal = normal;
vec3 vVertex = pos.xyz;
// Material Color:
vec4 color0 = texture (basic_texture, st);//vec4(0.8, 0.0, 0.0, 1.0);
         
// Silhouette Color:
vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
         
// Specular Color:
vec4 color2 = vec4(0.8, 0.0, 0.0, 0.1);
            
// Lighting
//vec3 eyePos = ;//vec3(0.0,0.0,5.0);
vec3 lightPos = pos.xyz;//vec3(0.0,5.0,5.0);
vec3 Normal = normalize((invTran * vec4(vNormal, 0.0)).xyz);
vec3 EyeVert = normalize(eyePos - vVertex);
vec3 LightVert = normalize(lightPos - vVertex);
vec3 EyeLight = normalize(LightVert+EyeVert);
// Simple Silhouette
float sil = max(dot(Normal,EyeVert), 0.0);
if (sil < 0.3) frag_colour = color1;
else 
  {
   frag_colour = color0;
   // Specular part
   float spec = pow(max(dot(Normal,EyeLight),0.0), shininess);
   if (spec < 0.2) frag_colour *= 0.8;
   else frag_colour = color2;
   // Diffuse part
   float diffuse = max(dot(Normal,LightVert),0.0);
   if (diffuse < 0.5) frag_colour *=0.8;
   }
}