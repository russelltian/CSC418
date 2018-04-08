/***********************************************************
 
 Starter code for Assignment 3
 
 Implements light_source.h
 
 ***********************************************************/

#include <cmath>
#include "light_source.h"
#include <random>
using namespace std;
double random_double(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
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
    Point3D localpos=ray.intersection.localPos;
    int x=3*(localpos[0]+0.5)*255;
    x%=255;
    int y=3*(localpos[1]+0.5)*255;
    y%=255;
    
    
    
    Material* mat = ray.intersection.mat;
    
    unsigned char* texture[3];
    texture[0]=mat->texture[0];
    texture[1]=mat->texture[1];
    texture[2]=mat->texture[2];
    
//    unsigned char** texture=(unsigned char**)mat->texture;
    
    Color diffuse = mat->diffuse;           //diffuse color
    Color ambient = mat->ambient;           //ambient color
    Color specular = mat->specular;         //specular color
    double specular_term = mat->specular_exp;   //specular exp
    
    //if use texture mapping
    if(texture[0]){
        Color texColor(texture[0][x+y*256]/255.0,texture[1][x+y*256]/255.0,texture[2][x+y*256]/255.0);
        diffuse = texColor;//diffuse color
        ambient =Color(0,0,0);//ambient color
        specular = texColor;         //specular color
        specular_term = mat->specular_exp;   //specular exp
    }else{
//        std::cout<<"???"<<std::endl;
    }

    
    
   //calculate diffuse color
    Color diff=fmax(normal.dot(inci_ray),0.0)*diffuse;
    
    //find reflection ray equals to 2(R.N)N - L
    Vector3D ref_ray=2*(normal.dot(inci_ray)*normal)-inci_ray;
    ref_ray.normalize(); //double make sure it is normalized
    
    Vector3D V=-ray.dir;  //view vector
    if(ray.intersection.mat->glossy_idx < 1){
        Vector3D u = ref_ray.cross(normal);
        u.normalize();
        Vector3D v = ref_ray.cross(u);
        v.normalize();
        //temp
        double roughness = ray.intersection.mat->glossy_idx;
        double theta = 2*M_PI*(random_double(0.0,1.0)*roughness);
        double phi = 2*M_PI*(random_double(0.0,1.0)*roughness);
        double x = sin(theta) * cos(phi);
        double y = sin(theta) * sin(phi);
        double z = cos(theta);
        ref_ray = x*u + y*v + z*ref_ray;
        ref_ray.normalize();
    }
    
    
    
    
   // Color ref=powf(fmax(ref_ray.dot(V),0.0),specular_term)*specular;
    Color ref = fmax(0.0,powf(ref_ray.dot(V),specular_term))*specular; // specular color
    
    
    // final color should also times the light color
    // for example, if light is red, only red should be left
    Color out=ambient*this->col_ambient+diff*this->col_diffuse+ref*this->col_specular;
    out.clamp();
    
    ray.col = ray.col + out;
}

// acturally identical with pointlight::shade, since it's an approximation of
// multiple point lights
void AreaLight::shade(Ray3D& ray){
    
    //we need to have incident light, reflect light, and normal vector
    //incident light
    Vector3D inci_ray = this->pos - ray.intersection.point; //light pos - intersection pos
    inci_ray.normalize(); //normalization
    
    //normalize the intersection normal vector
    Vector3D normal = ray.intersection.normal;
    normal.normalize();
    
    //find intersection material color term
    Point3D localpos=ray.intersection.localPos;
    int x=3*(localpos[0]+0.5)*255;
    x%=255;
    int y=3*(localpos[1]+0.5)*255;
    y%=255;
    
    
    
    Material* mat = ray.intersection.mat;
    
    unsigned char* texture[3];
    texture[0]=mat->texture[0];
    texture[1]=mat->texture[1];
    texture[2]=mat->texture[2];
    
    //    unsigned char** texture=(unsigned char**)mat->texture;
    
    Color diffuse = mat->diffuse;           //diffuse color
    Color ambient = mat->ambient;           //ambient color
    Color specular = mat->specular;         //specular color
    double specular_term = mat->specular_exp;   //specular exp
    
    //if use texture mapping
    if(texture[0]){
        Color texColor(texture[0][x+y*256]/255.0,texture[1][x+y*256]/255.0,texture[2][x+y*256]/255.0);
        diffuse = texColor;//diffuse color
        ambient =Color(0,0,0);//ambient color
        specular = texColor;         //specular color
        specular_term = mat->specular_exp;   //specular exp
    }else{
        //        std::cout<<"???"<<std::endl;
    }
    
    
    
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
    
    ray.col = ray.col + out;
}

//uniformly distributed area light
Point3D AreaLight::get_many_position(int i,int j)const{
    // double location = rand()/(RAND_MAX + 1.); //0-1 random location
    //use random location in the specific range times the light plane
    //return (this->pos + location*(i+1)*this->length+ location*(j+1)*this->width);
    double radius = 0.125;
    double random = random_double(0.0,1.0);
    double random1 = random_double(0.0,1.0);
    double random2 = random_double(0.0,1.0);
    double r = radius* random;
    double theta = 2 * M_PI *random1;
    double phi = 2 * M_PI * random2;
    return (this->pos + (i+1)*this->length+ (j+1)*this->width +
    Vector3D(r*cos(theta)*sin(phi),r*sin(theta)*sin(phi),r*cos(phi)));

    //acturally use uniform distribution instead of random distribution
    //return (this->pos+(i+1)*this->length+ (j+1)*this->width);
}



