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
void hard_shadow(Raytracer& raytracer,LightList& light_list,Scene& scene,int width,int height){
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2,0.0,1.0,NULL);
    Material ranColor(Color(0.3, 0.3, 0.3), Color(0.5,0.5,0.5),
                      Color(0.628281, 0.555802, 0.366065),
                      51.2,0.0,1.0,NULL);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8,0.0,1.0,NULL);
    Material glass(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                   Color(0.316228,0.316228,0.316228),
                   12.8,1.1,1.0,NULL);
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    
    SceneNode* lens = new SceneNode(new UnitSphere(), &glass);
    scene.push_back(lens);
    
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    //RTS
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -3));
    lens->scale(Point3D(0,0,0), lensfactor);
    
    
    
    double factor2[3] = { 6.0, 6.0, 1.0 };
    //    plane->translate(Vector3D(0, 0, -7));
    //    plane->rotate('z', 45);
    plane->translate(Vector3D(0, 0, -7));
    plane->scale(Point3D(0,0,0), factor2);
    
    
    
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

void soft_shadow(Raytracer& raytracer,LightList& light_list,Scene& scene,int width,int height){
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2,0.0,1.0,NULL);
    Material ranColor(Color(0.3, 0.3, 0.3), Color(0.5,0.5,0.5),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2,0.0,1.0,NULL);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8,0.0,1.0,NULL);
    Material glass(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
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
    
    Material woodMat(Color(0, 0, 0), Color(0.75164,0.60648,0.22648),
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
    
    Material rainbowMat(Color(0, 0, 0), Color(0, 0, 0),
                       Color(0, 0, 0),
                       51.2,0.0,1.0,rainbow);
    
    
    
    
    //added by us, define a extended area light source to make soft shadowing
    AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.1,0,0),Vector3D(0,0.1,0));
    light_list.push_back(aLight);
    
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &rainbowMat);
    scene.push_back(sphere);
    
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), &auroraMat);
    scene.push_back(sphere2);
    
    SceneNode* lens = new SceneNode(new UnitSphere(), &glass);
    scene.push_back(lens);
    
    SceneNode* plane = new SceneNode(new UnitSquare(), &woodMat);
    scene.push_back(plane);
    
    
    // Apply some transformations to the sphere and unit square.
    //RTS
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, -1.5, -6));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor3[3] = { 2.0, 1.0, 1.0 };
    sphere2->translate(Vector3D(0, 1.5, -7));
    sphere2->rotate('x', -45);
    sphere2->rotate('z', 45);
    sphere2->scale(Point3D(0, 0, 0), factor3);
    
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(1,1, -3));
    lens->scale(Point3D(0,0,0), lensfactor);
    
    
    
    double factor2[3] = { 6.0, 6.0, 1.0 };
    //    plane->translate(Vector3D(0, 0, -7));
    //    plane->rotate('z', 45);
    plane->translate(Vector3D(0, 0, -9));
    plane->scale(Point3D(0,0,0), factor2);
    
    
    
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
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2,0.0,1.0,NULL);
    
    
    Material ranColor(Color(0.3, 0.3, 0.3), Color(0.5,0.5,0.5),
                      Color(0.628281, 0.555802, 0.366065),
                      51.2,0.0,1.0,NULL);
    
    
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8,0.0,1.0,NULL);
    Material glass(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                   Color(0.316228,0.316228,0.316228),
                   12.8,1.1,1.0,NULL);
    
    //added by us, define a extended area light source to make soft shadowing
    AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.1,0,0),Vector3D(0,0.1,0));
    light_list.push_back(aLight);
    
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    //    scene.push_back(sphere);
    
    SceneNode* lens = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(lens);
    
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    SceneNode* chro = new SceneNode(new UnitSphere(), &ranColor);
    scene.push_back(chro);
    
    // Apply some transformations to the sphere and unit square.
    //RTS
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -3));
    lens->scale(Point3D(0,0,0), lensfactor);
    
    double grayfactor[3] = { 1.0, 1.0, 1.0 };
    chro->translate(Vector3D(0, 1, -1));
    chro->scale(Point3D(0,0,0), grayfactor);
    
    double factor2[3] = { 6.0, 6.0, 1.0 };
    //    plane->translate(Vector3D(0, 0, -7));
    //    plane->rotate('z', 45);
    plane->translate(Vector3D(0, 0, -7));
    plane->scale(Point3D(0,0,0), factor2);
    
    
    
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
    Scene scene;
    
    //int width = 320;
    //int height = 240;
    
    //for testing purpose
    int width = 640;
    int height = 480;
    
    
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    soft_shadow(raytracer,light_list,scene,width,height);

    return 0;
}

