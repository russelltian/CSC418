//
//  main.cpp
//  myraytracer
//
//  Created by HongYi Tian on 2018/3/20.
//  Copyright © 2018年 HongYi Tian. All rights reserved.
//

/***********************************************************
 
 Starter code for Assignment 3
 
 ***********************************************************/

#include <cstdlib>
#include "raytracer.h"
#include "light_source.h"
#include "bmp_io.h"
#include "scene_object.h"
#include "util.h"
using namespace std;

// Define materials for shading.

Scene scene;



void init(){
    Material *gold=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                Color(0.628281, 0.555802, 0.366065),
                                51.2,0.0,1.0,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.5,0.5,0.5),
                                    Color(0.628281, 0.555802, 0.366065),
                                    51.2,0.0,1.0,NULL);
    Material *jade=new Material(Color(0, 0, 0), Color(0.54,0.89,0.63),
                                Color(0.316228,0.316228,0.316228),
                                12.8,0.0,1.0,NULL);
    Material *glass=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                 Color(0.316228,0.316228,0.316228),
                                 12.8,1.1,1.0,NULL);
    
    unsigned long int* twidth= new unsigned long int();;
    long int* theight=new long int();
    //    unsigned char*** texture=new unsigned char**[3];
    unsigned char* wave[3];
    for(unsigned i=0;i<3;i++){
        wave[i]=new unsigned char();
    }
    bool read=bmp_read ( "wave03.bmp", twidth, theight,&wave[0], &wave[1], &wave[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material waveMat(Color(0, 0, 0), Color(0.75164,0.60648,0.22648),
                     Color(0.628281, 0.555802, 0.366065),
                     51.2,0.0,1.0,wave);
    
    
    unsigned char* grid[3];
    for(unsigned i=0;i<3;i++){
        grid[i]=new unsigned char();
    }
    read=bmp_read ( "grid.bmp", twidth, theight,&grid[0], &grid[1], &grid[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material gridMat(Color(0, 0, 0), Color(0.75164,0.60648,0.22648),
                     Color(0.628281, 0.555802, 0.366065),
                     51.2,0.0,1.0,grid);
    
    unsigned char* wood[3];
    for(unsigned i=0;i<3;i++){
        wood[i]=new unsigned char();
    }
    read=bmp_read ( "wood.bmp", twidth, theight,&wood[0], &wood[1], &wood[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material *woodMat = new Material(Color(0, 0, 0), Color(0.75164,0.60648,0.22648),
                     Color(0.628281, 0.555802, 0.366065),
                     51.2,0.0,1.0,wood);
    
    
    unsigned char* aurora[3];
    for(unsigned i=0;i<3;i++){
        aurora[i]=new unsigned char();
    }
    read=bmp_read ( "aurora_texture.bmp", twidth, theight,&aurora[0], &aurora[1], &aurora[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material auroraMat(Color(0, 0, 0), Color(0, 0, 0),
                       Color(0, 0, 0),
                       51.2,0.0,1.0,aurora);
    
    unsigned char* rainbow[3];
    for(unsigned i=0;i<3;i++){
        rainbow[i]=new unsigned char();
    }
    read=bmp_read ( "rainbow.bmp", twidth, theight,&rainbow[0], &rainbow[1], &rainbow[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material *rainbowMat = new Material(Color(0, 0, 0), Color(0, 0, 0),
                        Color(0, 0, 0),
                        51.2,0.0,1.0,rainbow);
    
    
    //russell's addition
    unsigned char* universe[3];
    for(unsigned i=0;i<3;i++){
        universe[i]=new unsigned char();
    }
    read=bmp_read ( "universe.bmp", twidth, theight,&universe[0], &universe[1], &universe[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    Material *universeMat = new Material(Color(0, 0, 0), Color(0, 0, 0),
                       Color(0, 0, 0),
                       51.2,0.0,1.0,universe);
    
    // Add a unit square into the scene with material mat.
//    SceneNode* sphere = new SceneNode(new UnitSphere(), &rainbowMat);
//    scene.push_back(sphere);
//
//    SceneNode* sphere2 = new SceneNode(new UnitSphere(), &auroraMat);
//    scene.push_back(sphere2);
//
//    SceneNode* lens = new SceneNode(new UnitSphere(), &glass);
//    scene.push_back(lens);
//
//    SceneNode* plane = new SceneNode(new UnitSquare(), &woodMat);
//    scene.push_back(plane);
//
//    SceneNode* chro = new SceneNode(new UnitSphere(), &ranColor);
//    scene.push_back(chro);
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), gold);
//    scene.push_back(sphere);
    SceneNode* lens = new SceneNode(new UnitSphere(), glass);
//    scene.push_back(lens);
    SceneNode* plane = new SceneNode(new UnitSquare(), jade);
  /// scene.push_back(plane);
    
    //added a cylinder
    SceneNode* cylinder = new SceneNode(new UnitCylinder(),gold);
    //scene.push_back(cylinder);
    // Apply some transformations to the sphere and unit square.
    //RTS
    //add triangle
    SceneNode* tri = new SceneNode(new UnitTriangle(),gold);
    scene.push_back(tri);

    
    
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -4));
    lens->scale(Point3D(0,0,0), lensfactor);
    double factor2[3] = { 6.0, 6.0, 1.0 };
    plane->translate(Vector3D(0, 0, -7));
    //plane->rotate('z', 45);
   // plane->translate(Vector3D(0, 0, -7));
    //plane->rotate('x',180 );

    plane->scale(Point3D(0,0,0), factor2);
    
    double cyl_factor[3] = { 1.0, 1.0, 1.0 };
    cylinder->translate(Vector3D(0, 0, -5));
    cylinder->rotate('y', 90);

}

void hard_shadow(Raytracer& raytracer,int width,int height){
    
    // Defines a point light source.
    LightList light_list;
    PointLight* pLight = new PointLight(Point3D(3,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("view2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
}

void soft_shadow(Raytracer& raytracer,int width,int height){
    LightList light_list;
    //added by us, define a extended area light source to make soft shadowing
    AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.5,0,0),Vector3D(0,0.5,0));
    light_list.push_back(aLight);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("view2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
}


void DOF(Raytracer& raytracer,LightList& light_list,Scene& scene,int width,int height){
    //added by us, define a extended area light source to make soft shadowing
    AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.05,0,0),Vector3D(0,0.05,0));
    light_list.push_back(aLight);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    double focus_point = 4.0; //where the lens is focusing
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render_dof(camera1, scene, light_list, image1,focus_point); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    
    // Render it from a different point of view.
    
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_dof(camera2, scene, light_list, image2,focus_point);
    image2.flushPixelBuffer("view2.bmp");
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
}

int main(int argc, char* argv[])
{
    // Build your scene and setup your camera here, by calling
    // functions from Raytracer.  The code here sets up an example
    // scene and renders it from two different view points, DO NOT
    // change this if you're just implementing part one of the
    // assignment.
    Raytracer raytracer;
    LightList light_list;
    
    //int width = 320;
    //int height = 240;
    
    //for testing purpose
    int width = 640;
    int height = 480;
    
    
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    init();
    soft_shadow(raytracer,width,height);
    
    return 0;
}

