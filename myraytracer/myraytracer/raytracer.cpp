/***********************************************************
 
 Starter code for Assignment 3
 
 Implementations of functions in raytracer.h,
 and the main function which specifies the scene to be rendered.
 
 ***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode* node = scene[i];
        
        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
            ray.intersection.mat = node->mat;
        }

    }
}

void Raytracer::computeTransforms(Scene& scene) {
    // right now this method might seem redundant. But if you decide to implement
    // scene graph this is where you would propagate transformations to child nodes
    
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode* node = scene[i];
        
        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans;
    }
}

//modified computeshading to find shadows
void Raytracer::computeShading(Ray3D& ray, LightList& light_list,Scene& scene) {
    
    Ray3D inter_to_light; //from intersection to light
    for (size_t  i = 0; i < light_list.size(); ++i) {
        LightSource* light = light_list[i];
        
        //compute shadows here
        //if has intersection with an object on the way to the light,
        //it is in that shadow
        Point3D lightPos = light->get_position(); // light position
        Point3D intersection = ray.intersection.point; //intersection position
        Vector3D dir = lightPos - intersection;     //direction
        dir.normalize(); //normalization
        inter_to_light = Ray3D(intersection, dir);
        
        traverseScene(scene, inter_to_light);//find intersection
        
        //if intersect,compute next light, this light is not visable
        if(!inter_to_light.intersection.none) {
            continue;
        }
        light->shade(ray);
    }
    //TODO: averaging the color if there are multiple light source
    //note some light source hit, some not, so  need to think
    //may use a counter
    //Color scale(1.0/light_list.size(),1.0/light_list.size(),1.0/light_list.size());
    //ray.col = ray.col * scale;
}


//added by us, use bounce to count the number of reflection

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list,int bounce,int count) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);
    
    //if intersection, compute the color at that intersection
    if(!ray.intersection.none ){
        computeShading(ray,light_list,scene);
        double scale_factor = pow((count+1),2);
        Color scale(1.0/scale_factor,1.0/scale_factor,1.0/scale_factor);
        col = ray.col*scale;
    }
    //if need more reflect and have intersection,
    //define the reflect ray, and recursive call
    if(bounce > 0 && !ray.intersection.none){
        //define reflect ray
        Point3D intersect_p = ray.intersection.point; // intersection point
        Vector3D intersect_n(ray.intersection.normal);
        intersect_n.normalize(); //normalize
        Vector3D intersect_r = -1*(2*(intersect_n.dot(ray.dir))*intersect_n-ray.dir);
        intersect_r.normalize();
        
        Ray3D second_r;
        second_r = Ray3D(intersect_p,intersect_r); //intersection point is its origin
        bounce--; //one more reflection
        //recursive call, count is used for decide scale factor to the light
        col = col + shadeRay(second_r, scene, light_list,bounce,++count);
    }
    return col;
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
    computeTransforms(scene);
    
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
    viewToWorld = camera.initInvViewMatrix();
    
    //added by us to do anti-aliasing
    int num_per_pixel = 25; // num of random ray per pixel
    
    
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            
            //added by us, compute average pixel value
            //initialize each pixel's color to 0
            //use Jitter Method, with simple random

            Color col(0.0,0.0,0.0);
       
            for(int k = 0; k < num_per_pixel; k++){
                Point3D origin(0, 0, 0);
                Point3D imagePlane;
                //get the random position between 0 and 1
                double location = rand()/(RAND_MAX + 1.);
                location = location/num_per_pixel*(k+1);//devide them to different block
                imagePlane[0] = (-double(image.width)/2 + location + j)/factor;
                imagePlane[1] = (-double(image.height)/2 + location + i)/factor;
                imagePlane[2] = -1;
                
                
                Ray3D ray;
                //Convert ray to world space
                //Ray Direction
                Vector3D dir = imagePlane - origin;
                //Convert direction and origin to world-space
                dir = viewToWorld * dir;
                dir.normalize();
                origin = viewToWorld * origin;
                //construct the ray
                ray = Ray3D(origin,dir);
                
                int depth = 1; //define depth to achieve reflection
                int count = 0;
                col = col + shadeRay(ray, scene, light_list,depth,count); //sum up color, include depth
            }
            Color scale(1.0/num_per_pixel,1.0/num_per_pixel,1.0/num_per_pixel);
            col = col*scale;  //scale the color to find avg color per pixel
            
            image.setColorAtPixel(i, j, col);
        }
    }
}
