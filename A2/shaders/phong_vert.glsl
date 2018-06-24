attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.
  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
    
    //my solution
    //first define vertPos in camera space
    vertPos=vec3(vertPos4/vertPos4.w); //vertPos4.w should be 1.0
    //second find normalInterp in camera space
    normalInterp = vec3(normalMat*vec4(normal,1.0));
    
    //third define viewVec in camera space, it is (0.0,0.0,0.0) - vertPos
    viewVec = -vertPos; 
}
