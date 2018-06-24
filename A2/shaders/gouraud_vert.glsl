attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex
varying vec4 color;

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position in camera space


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.

  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
    
    // my solution
    // first define the three varying vectors in camera space
    
    vertPos = vec3(vertPos4/vertPos4.w);// verPos4.w should be 1.0
    
    normalInterp = vec3(normalMat * vec4(normal,1.0));
    vec3 N = normalize(normalInterp); //normalize normalInterp
    
    //get light direction
    vec3 NotNorm_L = lightPos - vertPos; // by definition
    vec3 L = normalize(NotNorm_L);
    float lambertian = max(0.0,dot(N,L));
    // now do specular lighting
    // first find the reflection light of L, I understand how reflect works, I look up the function on
    // openGL guide and the explanation on it is the same as lecture notes
    vec3 R = normalize(reflect(-NotNorm_L,N)); //incident light is negative direction as L
    
    //Find eye direction
    viewVec = -vertPos;
    vec3 V = normalize(viewVec);
    
    float specular = pow(max(0.0,dot(R,V)),shininessVal);
    //color = vec4(specularColor*Ks*pow(dot(R,viewVec),shininessVal),1.0);
    color = vec4(Ka*ambientColor,1.0) + vec4(diffuseColor*lambertian*Kd,1.0)
    + vec4(Ks*specular*specularColor,1.0);
   
}
