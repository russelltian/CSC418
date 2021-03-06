/***********************************************************
 
 Starter code for Assignment 3
 
 This file contains the interface and datastructures of the raytracer.
 Simple traversal and addition code to the datastructures are given to you.
 
 ***********************************************************/
#pragma once

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

class Raytracer {
    public:
    // Renders 3D scene to an image given camera and lights setup.
    void render(Camera& camera, Scene& scene, LightList& light_list, Image& image);
    
    //added, Depth of Field render
    void render_dof(Camera& camera, Scene& scene, LightList& light_list, Image& image,double
                    focuspoint);
    
    //added for environmemt mapping
    void render_env(Camera& camera, Scene& scene, LightList& light_list, Image& image,Material*Mat);
    Color shadeRay_Env(Ray3D& ray, Scene& scene, LightList& light_list,Material*Mat);
    private:
    
    // Return the color of the ray after intersection and shading, call
    // this function recursively for reflection and refraction.
    Color shadeRay(Ray3D& ray, Scene& scene, LightList& light_list,int bounce,int count);
    
    // Traversal code for the scene, the ray is transformed into
    // the object space of each node where intersection is performed.
    void traverseScene(Scene& scene, Ray3D& ray);
    
    // After intersection, calculate the color of the ray by shading it
    // with all light sources in the scene.
    void computeShading(Ray3D& ray, LightList& light_list,Scene& scene);
    
    // Precompute the modelToWorld and worldToModel transformations for each
    // object in the scene.
    void computeTransforms(Scene& scene);
    
};


double random_double(double fMin, double fMax); //return a random number between fMin and fMax, I put this function in light_source.cpp.. can't remember the reason
void Shade_Env(Ray3D& ray,Material*Mat); // render the image for environemnt mapping

