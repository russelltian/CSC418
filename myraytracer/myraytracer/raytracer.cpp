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
KDNode* ROOT;

bool created=false;
void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
//    for (size_t i = 0; i < scene.size(); ++i) {
//        SceneNode* node = scene[i];
//
//        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
//            //if the material is transparent
//            if(node->mat->eta>0){
//                double eta=1.0/node->mat->eta;
//                Point3D origin=ray.intersection.point+0.000001*ray.dir;
//                Vector3D normal=ray.intersection.normal;
//                normal.normalize();
//                Vector3D in_ray=ray.dir;
//                in_ray.normalize();
//
//                double c1=normal.dot(-in_ray);
//                double c2=sqrt(1.0-eta*eta*(1.0-c1*c1));
//                Vector3D newDir=eta*(in_ray+c1*normal)-c2*normal;
//                newDir.normalize();
//                Ray3D newRay(origin,newDir);
//                traverseScene(scene, newRay);
//                ray=newRay;
//
//            }else{
//                ray.intersection.mat = node->mat;
//                ray.intersection.none = false;
//            }
//        }
//    }
    if(KDHit(ROOT,ROOT, ray)){
//        cout<<"hits!"<<endl;
    }
}

void Raytracer::computeTransforms(Scene& scene) {
    // right now this method might seem redundant. But if you decide to implement
    // scene graph this is where you would propagate transformations to child nodes
    
    for (size_t i = 0; i < scene.size(); ++i) {
        SceneNode* node = scene[i];
        
        if(!node->ismesh){
            node->modelToWorld = node->trans;
            node->worldToModel = node->invtrans;
        }else if(node->firstTouch){
            node->modelToWorld = node->trans*node->modelToWorld;
            node->worldToModel = node->worldToModel*node->invtrans;
            node->firstTouch = false;
        }
        
    }
}

//modified computeshading to find shadows
void Raytracer::computeShading(Ray3D& ray, LightList& light_list,Scene& scene) {
    Ray3D inter_to_light; //from intersection to light
    for (size_t  i = 0; i < light_list.size(); ++i) {
        LightSource* light = light_list[i];
        //if it is point light
        if(light->get_type() == 0){
            //compute shadows here
            //if has intersection with an object on the way to the light,
            //it is in that shadow
            Point3D lightPos = light->get_position(); // light position
            Point3D intersection = ray.intersection.point; //intersection position (new ray origin)
            Vector3D dir = lightPos - intersection;   //direction
            dir.normalize(); //normalization
            //shoot the ray
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
            int row = 1; //how many rows of point light
            int col = 1; //how many cols of point light
            for(int s = 0;s < row; s++){
                for(int z=0;z< col; z++){
                    Point3D lightPos = light->get_many_position(s,z); //specified point light position
                    Point3D intersection = ray.intersection.point; //intersection position (new ray origin)
                    Vector3D dir = lightPos - intersection;     //direction
                    dir.normalize(); //normalization
                    inter_to_light = Ray3D(intersection, dir); //shoot the ray
                    
                    traverseScene(scene, inter_to_light);//find intersection
                    
                    //if intersect,compute next light, this light is not visable
                    if(!inter_to_light.intersection.none) {
                        continue;
                    }
                    light->shade(ray);
                }
            }
            //averaging the color to visualize soft shadow
            Color scale(1.0/(row*col),1.0/(row*col),1.0/(row*col));
            ray.col = ray.col * scale;
        }
    }
}


//added by us, use bounce to count the number of reflection
//recurisive call itself to find reflection
Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list,int bounce,int count) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);
    
    //if intersection, compute the color at that intersection
    if(!ray.intersection.none ){
        computeShading(ray,light_list,scene);
        double scale_factor = pow((count+1),2);
        //approximation: scale the color by depth because the light strength decrease while traveling
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

void createBoxes(Scene& scene){
    for(int i=0;i<scene.size();i++){
        scene[i]->bbox.transForm(scene[i]->trans);
    }
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
    if(created){
        createBoxes(scene);
        created=true;
    }
    
    
    ROOT=build(scene, 0);
    
    computeTransforms(scene);
    
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

    viewToWorld = camera.initInvViewMatrix();
    
    //added by us to do anti-aliasing
    int num_per_pixel_row = 1; // pow(0.5) to the num of random ray per pixel
    
    
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
            
            //added by us, compute average pixel value (anti-aliasing)
            //initialize each pixel's color to 0
            //use Jitter Method, with simple random sample

            Color col(0.0,0.0,0.0);
       
            for(int k = 0; k < num_per_pixel_row; k++){
                for(int m = 0; m < num_per_pixel_row; m++){
                    Point3D origin(0, 0, 0);
                    Point3D imagePlane;
                    //get the random position between 0 and 1
                    double location = rand()/(RAND_MAX + 1.);
                    //devide them to different blocks on the pixel
                    double location_x = location/num_per_pixel_row*(k+1);
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
                
                    int depth = 1; //define depth for reflection, we compute one reflection
                    int count = 0;//counter to calculate light decay
                    col = col + shadeRay(ray, scene, light_list,depth,count); //sum up color, include depth
                }
            }
            double num_per_pixel = pow(num_per_pixel_row,2.0);//samples on the pixel
            Color scale(1.0/num_per_pixel,1.0/num_per_pixel,1.0/num_per_pixel);
            col = scale*col;  //scale the color to find avg color per pixel
            
            image.setColorAtPixel(i, j, col);
        }
    }
    cout << numRaySquareTests <<endl;

}

//added to do depth of field render
//we give the camera a thin lens instead of a pin hole
void Raytracer::render_dof(Camera& camera, Scene& scene, LightList& light_list, Image& image, double focus_index) {
    if(created){
        createBoxes(scene);
        created=true;
    }
    
    computeTransforms(scene);
    
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    
    viewToWorld = camera.initInvViewMatrix();
    
    //in DoF, we are averaging the color, so we do not use anti-aliasing,
    //otherwise the algo would be O(N^4), now is O(N^3)
    double interval = 0.03; // lens postion interval
    double low_high = 0.03; //lower and upper bound
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {
            // Sets up ray origin and direction in view space,
            // image plane is at z = -1.
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
            
            
            //use secondary ray to compute depth of field
            //first give primary ray a direction and focus point
            //then use vector manipulation to find second ray direction
            Vector3D dir = imagePlane - origin;
            Vector3D focal_point = focus_index*dir;
            for( double len_position = -low_high; len_position <= low_high; len_position+=interval){
                for( double len_position1 = -low_high; len_position1 <= low_high; len_position1+=interval){
                    //find secondary ray origin and direction
                    Vector3D len_vec = Vector3D(0,0,0)+ Vector3D(len_position,len_position1,0);
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
            int num_per_lens = floor((2*low_high)/interval)+1;//computation per row per pixel
            num_per_lens = pow(num_per_lens,2);             //computation per pixel
            Color lens_scale(1.0/num_per_lens,1.0/num_per_lens,1.0/num_per_lens);
            col = lens_scale * col; 
            image.setColorAtPixel(i, j, col);
        }
    }
}

//the following are defined for environment mapping, since right now we don't
//want to risk ruining the previous effort, so we write them seperatelly by far
void Raytracer::render_env(Camera& camera, Scene& scene, LightList& light_list, Image& image,Material*Mat) {
    
    computeTransforms(scene);
    Matrix4x4 viewToWorld;
    double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);
    viewToWorld = camera.initInvViewMatrix();
    //added by us to do anti-aliasing
    int num_per_pixel_row = 2; // pow(0.5) to the num of random ray per pixel
    // Construct a ray for each pixel.
    for (int i = 0; i < image.height; i++) {
        for (int j = 0; j < image.width; j++) {

            Color col(0.0,0.0,0.0);
            
            for(int k = 0; k < num_per_pixel_row; k++){
                for(int m = 0; m < num_per_pixel_row; m++){
                    Point3D origin(0, 0, 0);
                    Point3D imagePlane;
                    //get the random position between 0 and 1
                    double location = rand()/(RAND_MAX + 1.);
                    //devide them to different blocks on the pixel
                    double location_x = location/num_per_pixel_row*(k+1);
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
                    col = col + shadeRay_Env(ray, scene, light_list,Mat); //sum up color
                }
            }
            double num_per_pixel = pow(num_per_pixel_row,2.0);//samples on the pixel
            Color scale(1.0/num_per_pixel,1.0/num_per_pixel,1.0/num_per_pixel);
            col = scale*col;  //scale the color to find avg color per pixel
            
            image.setColorAtPixel(i, j, col);
        }
    }
}

//environment mapping shaderay
Color Raytracer::shadeRay_Env(Ray3D& ray, Scene& scene, LightList& light_list,Material*Mat) {
    Color col(0.0, 0.0, 0.0);
    traverseScene(scene, ray);

    //if intersection, compute the color at that intersection
    if(ray.intersection.none ){
        Shade_Env(ray,Mat);//(ray,light_list,scene);
        col = ray.col;
    }
    //if need more reflect and have intersection,
    //define the reflect ray, and recursive call
    if(!ray.intersection.none){
        //define reflect ray
        Point3D intersect_p = ray.intersection.point; // intersection point
        Vector3D intersect_n(ray.intersection.normal);
        intersect_n.normalize(); //normalize
        Vector3D intersect_r = -1*(2*(intersect_n.dot(ray.dir))*intersect_n-ray.dir);
        intersect_r.normalize();
        
        Ray3D second_r;
        second_r = Ray3D(intersect_p,intersect_r); //intersection point is its origin

        //recursive call, count is used for decide scale factor to the light
        col = col + shadeRay_Env(second_r, scene, light_list,Mat);
        col.clamp();
    }
    return col;
}

//confirm the background pixel
void Shade_Env(Ray3D& ray,Material*Mat){
    double x = ray.dir[0];
    double y = ray.dir[1];
    double z = ray.dir[2];
    double absX = fabs(x);
    double absY = fabs(y);
    double absZ = fabs(z);
    int is_x_pos = x > 0 ? 1 : 0;
    int is_y_pos = y > 0 ? 1 : 0;
    int is_z_pos = z > 0 ? 1 : 0;
    double u,v;
    int index = 0;
    double uc = 0.0; double vc = 0.0;
    double maxAxis = 0.0;
    // positive x
    if(is_x_pos && absX >= absY && absX >= absZ){
    // u (0 to 1) goes from +z to -z
    // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
        index = 0;
    }
    // NEGATIVE X
    if (!is_x_pos && absX >= absY && absX >= absZ) {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
        index = 1;
    }
    // POSITIVE Y
    if (is_y_pos && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
        index = 2;
    }
    // NEGATIVE Y
    if (!is_y_pos && absY >= absX && absY >= absZ) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
        index = 3;
    }
    // POSITIVE Z
    if (is_z_pos && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
        index = 4;
    }
    // NEGATIVE Z
    if (!is_z_pos && absZ >= absX && absZ >= absY) {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
        index = 5;
    }

    //convert range from -1 to 1 to 0 to 1
    u = 0.5f * (uc / maxAxis + 1.0f);
    v = 0.5f * (vc / maxAxis + 1.0f);
//    if(u != 0 && v != 0 && index == 0)
//    printf("%d,%f, %f \n",index,u,v);
    unsigned char** texture=Mat->texture;
//    texture[0] = Mat->texture[0];
//    texture[1] = Mat->texture[1];
//    texture[2] = Mat->texture[2];
    int height=int(Mat->theight);
    int width=int(Mat->twidth);
    int tx = width/4;
    int ty = height/3;
    int tIndex;
    int pos_x = 0;
    int pos_y = 0;
    switch (index)
    {
        case 0: pos_x = (2*tx + u*tx);
                pos_y = (ty + v*ty);
                break;
        case 1: pos_x = (u*tx);
                pos_y = (ty + v*ty);
            
                break;
        case 2: pos_x = (tx + u*tx) ;
                pos_y = (2*ty + v * ty);
           
                break;
        case 3: pos_x = (tx + u*tx );
                pos_y = (v * ty);
                break;
        case 4: pos_x =( tx + u*tx) ;
                pos_y = (ty + v * ty);
                break;
        case 5: pos_x = (3*tx + u*tx);
                pos_y = (ty + v * ty);
                break;
    }
    //tIndex= (pos_y*width+pos_x);
    tIndex= (pos_y)*width + pos_x ;
  //  tIndex = 12186642  -100;
    //tIndex = 12186642+3;
    if(texture[0] /*&& (index == 2 || index == 4 || index == 3 )*/){
        Color texColor(texture[0][tIndex]/255.0,texture[1][tIndex]/255.0,texture[2][tIndex]/255.0);
        ray.col = texColor;
//        if (texColor[0] == 1.0){
//            cout << pos_x << " "<<pos_y<<endl;
//            ray.col[0] = 0;
//        }
    }
}
