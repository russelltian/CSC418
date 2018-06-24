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
  
  // The model is currently rendered in black
    
    //my solution
    //first normalize the normal
    vec3 N = normalize(normalInterp);
    
    //get light direction
    vec3 Non_norm_l = lightPos - vertPos;
    vec3 L = normalize(Non_norm_l); //normalize it
    
    //find the diffuse term
    float lambertian = max(0.0,dot(L,N));
    
    //normalize the view vector, we need it to compute the rim effect
    vec3 V = normalize(viewVec);
    
    // to achieve the effect of opacity or transparent,
    // the closer viewdirection to normal, the more transparent it is
    // the larger the dot product, the less diffuse color we have
    
    float rim = abs(dot(V,N));
    // the rim effect isn't good enough, so we need a power term to
    //make the rim effect stronger
    //float rimeffect = 10.0;
    gl_FragColor = vec4(diffuseColor,1.0-rim);
}
