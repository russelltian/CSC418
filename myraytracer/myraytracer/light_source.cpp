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
    double ka=0.3;
    double kd=0.3;
    double ks=0.3;
    double alpha=3.0;
    
    
    Vector3D inci_ray=this->pos-ray.intersection.point;
    inci_ray.normalize();
    
    //calculate diffuse color
    Color diff=fmax(ray.intersection.normal.dot(inci_ray),0.0)*this->col_diffuse;
    
    
    //TODO: optimization
//    Vector3D diff=ray.intersection.normal-inci_ray;
    
    
    Vector3D ref_ray=2*ray.intersection.normal-inci_ray;
    Vector3D V=-ray.dir;
    Color ref=powf(fmax(ref_ray.dot(V),0.0),alpha)*this->col_specular;

    
    ray.col=ka*this->col_ambient+kd*diff+ks*ref;
    
    
}
