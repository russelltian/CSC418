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
            //if the material is transparent
            if(node->mat->eta>0){
                double eta=1.0/node->mat->eta;
                Point3D origin=ray.intersection.point+0.000001*ray.dir;
                Vector3D normal=ray.intersection.normal;
                normal.normalize();
                Vector3D in_ray=ray.dir;
                in_ray.normalize();
                
                double c1=normal.dot(-in_ray);
                double c2=sqrt(1.0-eta*eta*(1.0-c1*c1));
                Vector3D newDir=eta*(in_ray+c1*normal)-c2*normal;
                newDir.normalize();
                Ray3D newRay(origin,newDir);
                traverseScene(scene, newRay);
                ray=newRay;
                
            }else{
                ray.intersection.mat = node->mat;
                ray.intersection.none = false;
            }
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
        //point light
        if(light->get_type() == 0){
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
        }else if (light->get_type() == 1){
            //area light
            //need to approximate the light source
            int row = 4; //how many rows of point light
            int col = 4; //how many cols of point light
            for(int s = 0;s < row; s++){
                for(int z=0;z< col; z++){
                    Point3D lightPos = light->get_many_position(s,z); // light position
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
            }
            Color scale(1.0/(row*col),1.0/(row*col),1.0/(row*col));
            ray.col = ray.col * scale;
        }
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
        col.clamp();
    }
    return col;
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
    computeTransforms(scene);
    
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
    viewToWorld = camera.initInvViewMatrix();
    
    //added by us to do anti-aliasing
    int num_per_pixel_row = 2; // pow(0.5) to the num of random ray per pixel
    
    
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            
            //added by us, compute average pixel value
            //initialize each pixel's color to 0
            //use Jitter Method, with simple random sample

            Color col(0.0,0.0,0.0);
       
            for(int k = 0; k < num_per_pixel_row; k++){
                for(int m = 0; m < num_per_pixel_row; m++){
                    Point3D origin(0, 0, 0);
                    Point3D imagePlane;
                    //get the random position between 0 and 1
                    double location = rand()/(RAND_MAX + 1.);
                    double location_x = location/num_per_pixel_row*(k+1);//devide them to different block
                    double location_y = location/num_per_pixel_row*(m+1);
                    imagePlane[0] = (-double(image.width)/2 + location_x + j)/factor;
                    imagePlane[1] = (-double(image.height)/2 + location_y + i)/factor;
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
            }
            double num_per_pixel = pow(num_per_pixel_row,2.0);
            Color scale(1.0/num_per_pixel,1.0/num_per_pixel,1.0/num_per_pixel);
            col = scale*col;  //scale the color to find avg color per pixel
            
            image.setColorAtPixel(i, j, col);
        }
    }
}

//added to do depth of field render
//we give the camera a thin lens instead of a pin hole
void Raytracer::render_dof(Camera& camera, Scene& scene, LightList& light_list, Image& image, double focus_index) {
    computeTransforms(scene);
    
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
    viewToWorld = camera.initInvViewMatrix();
    
    //added by us to do anti-aliasing
    //in DoF, we are averaging the color, so we abandon anti-aliasing,
    //otherwise the algo would be O(N^4), now is O(N^3)
    int num_per_pixel_row = 1; // pow(0.5) to the num of random ray per pixel
    double interval = 0.01; // lens postion interval
    double low_high = 0.1; //lower and upper bound
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            
            //added by us, compute average pixel value
            //initialize each pixel's color to 0
            //use Jitter Method, with simple random sample
            
            Color col(0.0,0.0,0.0);
            Point3D origin(0, 0, 0);
            Point3D imagePlane;
            //get the random position between 0 and 1
            //double location = rand()/(RAND_MAX + 1.);
            // double location_x = location/num_per_pixel_row*(k+1);//devide them to different block
            //double location_y = location/num_per_pixel_row*(m+1);
            imagePlane[0] = (-double(image.width)/2 + 0.5 + j)/factor;
            imagePlane[1] = (-double(image.height)/2 + 0.5 + i)/factor;
            imagePlane[2] = -1;
            
            for(int k = 0; k < num_per_pixel_row; k++){
                for(int m = 0; m < num_per_pixel_row; m++){
                 
                    //use secondary ray to compute depth of field
                    //first give primary ray a direction and focus point
                    //then use vector manipulation to find second ray direction
                    Vector3D dir = imagePlane - origin;
                    Vector3D focal_point = focus_index*dir;
                    for( double len_position = -low_high; len_position <= low_high; len_position+=interval){
                        //find secondary ray origin and direction
                        Vector3D len_vec = Vector3D(0,0,0)+ Vector3D(0,len_position,0);
                        Point3D sec_origin = Point3D(len_vec);
                        //Vector3D temp(imagePlane[0],imagePlane[1],imagePlane[2]);
                        Vector3D sec_dir = focal_point - len_vec;
                        //Convert direction and origin to world-space
                        sec_dir = viewToWorld * sec_dir;
                        sec_dir.normalize();
                        sec_origin = viewToWorld * sec_origin;
                        //Build the ray
                        Ray3D ray;
                        ray = Ray3D(sec_origin,sec_dir);
                        int depth = 1; //define depth to achieve reflection
                        int count = 0;
                        col = col + shadeRay(ray, scene, light_list,depth,count); //sum up color, include depth
                    }
                }
            }
            //Computed color for each pixel, need to find the average
            //double num_per_pixel = pow(num_per_pixel_row,2.0);
            //Color scale(1.0/num_per_pixel,1.0/num_per_pixel,1.0/num_per_pixel);
            int num_per_lens = floor((2*low_high)/interval)+1;
            Color lens_scale(1.0/num_per_lens,1.0/num_per_lens,1.0/num_per_lens);
            // col = scale*col;  //scale the color to find avg color per pixel
            col = lens_scale * col; 
            image.setColorAtPixel(i, j, col);
        }
    }
}
