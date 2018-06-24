// Fragment shader template for the bonus question

precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
// NOTE: You may need to edit this section to add additional variables
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
// NOTE: You may need to edit this section to add additional variables
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

uniform sampler2D uSampler;	// 2D sampler for the earth texture

void main() {
    // Your solution should go here.
    // Only the ambient colour calculations have been provided as an example.
   
    //my solution
    //first normalize the normal
    vec3 N = normalize(normalInterp);
    
    //get light direction
    vec3 Non_norm_l = lightPos - vertPos;
    vec3 L = normalize(Non_norm_l);
    //find the diffuse term based on the dot product
    float lambertian =  max(0.0,dot(L,N));// 1.0 L = N, 0.0 L perpendicular to N
    //we use lambertian property to find threshold, the naming is entertaining.
    //we use these threshold to determine the intensity of line
    float rare = 0.00001;
    float medium_rare = 0.25;
    float medium = 0.5;
    float medium_well = 0.7;
    float well = 0.9;
    
    
    //find the vertex position on the screen
    //use gl_FragCoord
    vec2 st = gl_FragCoord.xy;

    // define pen and light colour
    vec4 black= vec4(ambientColor,1.0);
    vec4 white = vec4(specularColor,1.0);
   
    //lambertian range is 0 to 1, so use is a control variable to the size of dot
    // for the strength of light
    // y = -x + offset -> x + y mod offset == 0
    // y = x - offset -> x - y mod offset == 0
    float lineoffset = 8.0; // control the intensity of the line
    vec4 tempcolour = white;
    float smalloffset = 4.0; // more offset of the pixel,make the line more equally distributed
    float smallestoffset = 3.0;
    // if lambient > 0.9, completely white,
    // if not, start to increase line intensity
    if (lambertian < well) {
        if (mod(st.x - st.y,lineoffset)== 0.0) {
            tempcolour = black;
        }
    }
     if (lambertian < medium_well) {
        if (mod(st.x + st.y, lineoffset) == 0.0) {
            tempcolour = black;
        }
    }
     if (lambertian < medium) {
        if (mod(st.x - st.y - smalloffset, lineoffset) == 0.0) {
            tempcolour = black;
        }
    }
    
     if (lambertian < medium_rare) {
        if (mod(st.x + st.y - smalloffset, lineoffset) == 0.0) {
            tempcolour = black;
        }
    }
    if (lambertian < rare) {
        //make the shadow black, this is absolutely the most presentable way
        tempcolour = black;
        
    }
    gl_FragColor =  tempcolour;
    
}
