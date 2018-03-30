/***********************************************************
 
 Starter code for Assignment 3
 
 Implements light_source.h
 
 ***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
    // TODO: implement this function to fill in values for ray.col
    // using phong shading.  Make sure your vectors are normalized, and
    // clamp colour values to 1.0.
    //
    // It is assumed at this point that the intersection information in ray
    // is available.  So be sure that traverseScene() is called on the ray
    // before this function.
    
    //we need to have incident light, reflect light, and normal vector
    //incident light
    Vector3D inci_ray = this->pos - ray.intersection.point; //light pos - intersection pos
    inci_ray.normalize(); //normalization
    
    //normalize the intersection normal vector
    Vector3D normal = ray.intersection.normal;
    normal.normalize();
    
    //find intersection material color term
    Material* mat = ray.intersection.mat;
    Color diffuse = mat->diffuse;           //diffuse color
    Color ambient = mat->ambient;           //ambient color
    Color specular = mat->specular;         //specular color
    double specular_term = mat->specular_exp;   //specular exp
    
    
   //calculate diffuse color
    Color diff=fmax(normal.dot(inci_ray),0.0)*diffuse;
    
    //find reflection ray equals to 2(R.N)N - L
    Vector3D ref_ray=2*(normal.dot(inci_ray)*normal)-inci_ray;
    ref_ray.normalize(); //double make sure it is normalized
    
    Vector3D V=-ray.dir;  //view vector
   // Color ref=powf(fmax(ref_ray.dot(V),0.0),specular_term)*specular;
    Color ref = fmax(0.0,powf(ref_ray.dot(V),specular_term))*specular; // specular color
    
    // final color should also times the light color
    // for example, if light is red, only red should be left
    Color out=ambient*this->col_ambient+diff*this->col_diffuse+ref*this->col_specular;
    out.clamp();
    ray.col=out;
}
