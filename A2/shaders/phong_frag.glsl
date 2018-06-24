precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

void main() {
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.
  //gl_FragColor = vec4(ambientColor, 1.0);
    
    //my solution
    //first normalize the normal
    vec3 N = normalize(normalInterp);
    
    //get light direction, from the slides, the direction is from vertex to light
    vec3 Non_norm_l = lightPos - vertPos;
    vec3 L = normalize(Non_norm_l); // normalize light vector
    //find the diffuse term
    float lambertian = max(0.0,dot(L,N)); // by definition
    
    // get specular term
    //find reflected light, I understand how reflect works, I look up the function on
    // openGL guide and the explanation on it is the same as lecture notes
    
    vec3 R = normalize(reflect(-Non_norm_l,N)); //incident light should be from light to vertex
    vec3 V = normalize(viewVec); //normalize viev vector
    //specular intensity
    float temp = max(0.0,dot(R,V));
    float specular = pow(temp,shininessVal); // by definition
    
    //modified version of gl_FragColor
    gl_FragColor = vec4(Ka*ambientColor,1.0) + vec4(Kd*lambertian*diffuseColor,1.0)
    + vec4(Ks*specular*specularColor,1.0);
}
