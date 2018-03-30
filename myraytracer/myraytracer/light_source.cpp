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
    Vector3D inci_ray=this->pos-ray.intersection.point;
    inci_ray.normalize(); //normalization
    
    //normalize the normal vector, added by russell, could be deleted
    Vector3D normal = ray.intersection.normal;
    normal.normalize();
    
    //added by russell , find color term
    Material* mat = ray.intersection.mat;
    Color diffuse = mat->diffuse;
    Color ambient = mat->ambient;
    Color specular = mat->specular;
    double specular_term = mat->specular_exp;
    
    
   //calculate diffuse color
    
   
    Color diff=fmax(normal.dot(inci_ray),0.0)*diffuse;

    //TODO: optimization
    Vector3D ref_ray=2*ray.intersection.normal-inci_ray;
    Vector3D V=-ray.dir;  //view vector
    Color ref=powf(fmax(ref_ray.dot(V),0.0),specular_term)*specular;

    Color out=ambient*this->col_ambient+diff*this->col_diffuse+ref*this->col_specular;
    out.clamp();
    ray.col=out;
}
