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
    int width = 320;
    int height = 240;
    
    
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    
    // Define materials for shading.
    Material gold(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                  Color(0.628281, 0.555802, 0.366065),
                  51.2);
    Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
                  Color(0.316228,0.316228,0.316228),
                  12.8);
    
    // Defines a point light source.
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), &gold);
    scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), &jade);
    scene.push_back(plane);
    
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -7));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
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
    return 0;
}

