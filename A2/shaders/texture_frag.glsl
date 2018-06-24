precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector
varying highp vec2 texCoordInterp;

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shinines

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

// uniform values are the same across the scene
uniform sampler2D uSampler;	// A GLSL sampler represents a single texture. A sampler2D can be used to sample a 2D texture.

void main() {
  // Your solution should go here.
  // The model is currently rendered in black
 
    
    // my solution
    // use sampler to sample the texture, we need to flip the y coordinate which
    // is 1.0 - coordinate.y
    gl_FragColor = texture2D(uSampler,vec2(texCoordInterp.x,1.0-texCoordInterp.y));
}
