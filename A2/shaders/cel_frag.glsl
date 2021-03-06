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
  // Your solution should go here
  // Only the ambient colour calculations have been provided as an example.
  //gl_FragColor = vec4(ambientColor, 1.0);
    

    //my solution
    //first normalize the normal
    vec3 N = normalize(normalInterp);
    
    //get light direction
    vec3 Non_norm_l = lightPos - vertPos;
    vec3 L = normalize(Non_norm_l); //normalize it
    //find the diffuse term, divide it to 4 different levels to posterlize it
    // here I define diffrent color layer based on the level of light intensity
    float layer = 4.0;
    float lambertian = floor(layer*max(0.0,dot(L,N)))/layer;
    
    // get specular term
    //find reflected light
    vec3 R = normalize(reflect(-Non_norm_l,N)); // negative direction,from light to vertex
    vec3 V = normalize(viewVec);
    
    //specular intensity, only need a little specular point area,
    //same idea as diffuse color layer
    float area_control = 1.02;//slightly larger than 1.0 to shrink the area of specular light
    float specular = pow(floor(area_control*max(dot(R,V),0.0))/area_control,shininessVal);
    
    //modified version of gl_FragColor
    gl_FragColor = vec4(Ka*ambientColor,1.0) + vec4(Kd*lambertian*diffuseColor,1.0)
    + vec4(Ks*specular*specularColor,1.0);
}
