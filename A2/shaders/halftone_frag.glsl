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

// HINT: Use the built-in variable gl_FragCoord to get the screen-space coordinates

void main() {
  // Your solution should go here.
  // Only the background color calculations have been provided as an example.
  //gl_FragColor = vec4(diffuseColor, 1.0);
    
    //my solution
    //first normalize the normal
    vec3 N = normalize(normalInterp);
    
    //get light direction
    vec3 Non_norm_l = lightPos - vertPos;
    vec3 L = normalize(Non_norm_l);
    //find the diffuse term based on the dot product
    float lambertian = max(0.0,dot(L,N));
    
    
    //find the vertex position on the screen
    //use gl_FragCoord
    vec2 st = gl_FragCoord.xy;
    //radius of circle, every 10 pixel a circle, r = 5.0
    float radius = 5.0;
    
    vec4 black= vec4(ambientColor,1.0); // use ambient color as dot
    //draw a circle for every 10 pixels, start from pixel 5
    vec2 origin = vec2(floor(st.x/10.0)*10.0 + 5.0,floor(st.y/10.0)*10.0 + 5.0);
    //lambertian range is 0 to 1, so use is a control variable to the size of dot
    // for the strength of light
    if(distance(origin,st)>5.0-5.0*lambertian){
        gl_FragColor = vec4(diffuseColor,1.0); //out of radius, diffuse color
    }else{
        gl_FragColor=black; //dot
    }
}
