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
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include <cstdlib>
#include "raytracer.h"
#include "light_source.h"
#include "bmp_io.h"
#include "scene_object.h"
#include "util.h"
#include "tiny_obj_loader.h"

using namespace std;

// Define materials for shading.

Scene scene;


//for environment mapping
Material* init_env(){
    Material *gold=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                Color(0.628281, 0.555802, 0.366065),
                                51.2,0.0,1.0,NULL,NULL,NULL);
    
    unsigned long int* twidth= new unsigned long int();;
    long int* theight=new long int();
    
    //canyon scene
    unsigned char* canyon[3];
    for(unsigned i=0;i<3;i++){
        canyon[i]=new unsigned char();
    }
    bool read=bmp_read ( "canyon.bmp", twidth, theight,&canyon[0], &canyon[1], &canyon[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    Material *canyonMat = new Material(Color(0, 0, 0), Color(0, 0, 0),
                                       Color(0, 0, 0),
                                       51.2,0.0,1.0,canyon,*twidth,*theight);
    
    //skysea scene
    unsigned char* skysea[3];
    for(unsigned i=0;i<3;i++){
        skysea[i]=new unsigned char();
    }
    read=bmp_read ("cloudySea.bmp", twidth, theight,&skysea[0], &skysea[1], &skysea[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    Material *skyseaMat = new Material(Color(0, 0, 0), Color(0, 0, 0),
                                       Color(0, 0, 0),
                                       51.2,0.0,1.0,skysea,*twidth,*theight);
    
    //wood mateiral
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
                                     51.2,0.0,1.0,wood,*twidth,*theight);
    //standard objects in the scene by default
    double factor1[3] = { 6.0, 6.0, 1.0 };
    double factor2[3] = { 2.0, 2.0, 2.0 };
    //SceneNode* sphere2 = new SceneNode(new UnitSphere(), gold);
    //scene.push_back(sphere2);
    //   sphere2->translate(Vector3D(-2, 0, -4));
    //   sphere2->scale(Point3D(0,0,0),factor2);
    SceneNode* box = new SceneNode(new UnitCube(), gold);
    scene.push_back(box);
    box->translate(Vector3D(-2, 0, -4));
    box->scale(Point3D(0,0,0),factor2);
    SceneNode* plane = new SceneNode(new UnitSquare(), woodMat);
    scene.push_back(plane);
    plane->translate(Vector3D(0, 0, -7));
    plane->scale(Point3D(0,0,0),factor1);
    return skyseaMat;
}

Material* createMat(char const * s){
    unsigned long int* twidth= new unsigned long int();;
    long int* theight=new long int();
    unsigned char** rgb=new unsigned char*[3];
    for(unsigned i=0;i<3;i++){
        rgb[i]=new unsigned char();
    }
    bool read=bmp_read ( s, twidth, theight,&rgb[0], &rgb[1], &rgb[2]);
    if(read){
        std::cout<<"error loading texture"<<std::endl;
    }
    
    return new Material(Color(0, 0, 0), Color(0, 0, 0),
                                        Color(0, 0, 0),
                                        51.2,0.0,1.0,rgb,*twidth,*theight);
}


void init(){
    Material *gold=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                Color(0.628281, 0.555802, 0.366065),
                                51.2,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.9,0.9,0.9),
                                    Color(0.628281, 0.555802, 0.366065),
                                    51.2,0.0,1.0,NULL,NULL,NULL);
    Material *jade = new Material(Color(0, 0, 0), Color(0.54,0.89,0.63),
                                Color(0.316228,0.316228,0.316228),
                                12.8,0.0,1.0,NULL,NULL,NULL);
    Material *glass = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                 Color(0.316228,0.316228,0.316228),
                                 12.8,1.1,1.0,NULL,NULL,NULL);
    
    // russell's edit, redefine mateiral for glossy effect
    Material *gold_glossy=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                Color(0.628281, 0.555802, 0.366065),
                                51.2,0.0,0.05,NULL,NULL,NULL);
    
    Material *jade_glossy=new Material(Color(0, 0, 0), Color(0.54,0.89,0.63),
                                Color(0.316228,0.316228,0.316228),
                                12.8,0.0,0.15,NULL,NULL,NULL);
    
    Material* waveMat=createMat("wave03.bmp");

    Material* gridMat =createMat("grid.bmp");
    
    Material *woodMat =createMat("wood.bmp");
    
    Material *auroraMat=createMat("aurora_texture.bmp");
    
    Material *rainbowMat=createMat("rainbow.bmp");
    
    
    //load obj file here
    std::string inputfile = "David2.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
    
    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }
    
    if (!ret) {
        exit(1);
    }
    
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv;) {
                Point3D thisTri[3];
                for(int i=0;i<3;i++){
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    double vx = attrib.vertices[3*idx.vertex_index+0];
                    double vy = attrib.vertices[3*idx.vertex_index+1];
                    double vz = attrib.vertices[3*idx.vertex_index+2];
                    
                    thisTri[i]=Point3D(vx,vy,vz);
                    v++;
                }
                SceneNode* thisT = new SceneNode(new Triangle(thisTri[0],thisTri[1],thisTri[2]),jade_glossy);
                //order:TRS
                thisT->translate(Vector3D(0,-3,20));
//                thisT->rotate('y',45);
                thisT->rotate('x',-90);
                double Tfactor[3] = { 0.07, 0.07, 0.07 };
                thisT->scale(Point3D (0,0,0), Tfactor);
                scene.push_back(thisT);
            }
            index_offset += fv;
            
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    
    
    //standard objects in the scene by default
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), gold);
//    scene.push_back(sphere);
    SceneNode* lens = new SceneNode(new UnitSphere(), glass);
//    scene.push_back(lens);
    SceneNode* plane = new SceneNode(new UnitSquare(), rainbowMat);
//    scene.push_back(plane);
    
    //added a cylinder
    SceneNode* cylinder = new SceneNode(new UnitCylinder(),glass);
//    scene.push_back(cylinder);
    
    
    
    // Apply some transformations to the sphere and unit square.
    //RTS


    //all temporary, can delete at any time
    
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), gold);
     //scene.push_back(sphere2);
    
    
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, 0));
    sphere->rotate('x', 0);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -4));
    lens->scale(Point3D(0,0,0), lensfactor);
    double factor2[3] = { 100.0, 100.0, 100.0 };
    plane->translate(Vector3D(0, 10, -50));
    //plane->rotate('z', 45);
   // plane->translate(Vector3D(0, 0, -7));
    //plane->rotate('x',180 );

    plane->scale(Point3D(0,0,0), factor2);
    
    double cyl_factor[3] = {0.5,0.5,3.0};
    cylinder->translate(Vector3D(0, 0, -4));
    cylinder->rotate('y',45);
    cylinder->rotate('x',60 );
    cylinder->scale(Point3D(0,0,0), cyl_factor);
    
    
    
    
    
    
}

void hard_shadow(Raytracer& raytracer,int width,int height){
    
    // Defines a point light source.
    LightList light_list;
//  PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    PointLight* pLight = new PointLight(Point3D(-3,-4,5), Color(0.9,0.9,0.9));
  //  PointLight* pLight = new PointLight(Point3D(-5,-5,10), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // temp
    PointLight* pLight1 = new PointLight(Point3D(0,3,-7), Color(0.9,0.9,0.9));
    //light_list.push_back(pLight1);
    
    
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
//    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
 //   Camera camera1(Point3D(0, 0, 5), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
 //   Image image1(width, height);
 //   raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
 //   image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    //std::cout << "finished View1" << std::endl;
    // Render it from a different point of view.
//    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Camera camera2(Point3D(0, -3, 5), Vector3D(0, 3, -5), Vector3D(0, 1, 0), 60.0);
  //  Camera camera2(Point3D(0, -10, 3), Vector3D(0, 10, -3), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render(camera2, scene, light_list, image2);
    image2.flushPixelBuffer("view2.bmp");
    std::cout << "finished View2" << std::endl;
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
    AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.2,0,0),Vector3D(0,0.2,0));
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


void DOF(Raytracer& raytracer,int width,int height){
    //added by us, define a extended area light source to make soft shadowing
    LightList light_list;
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

void env_mapping(Raytracer& raytracer,int width,int height,Material* mat){
    LightList light_list;
    
    Camera camera1(Point3D(0, 0, 1), Vector3D(-4, 2, -6), Vector3D(0, 1, 0), 60.0);//0,0,-1 init
    Image image1(width, height);
    raytracer.render_env(camera1, scene, light_list, image1,mat); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    
    //right forward up
    Camera camera2(Point3D(4, 2, 1), Vector3D(4, 2, -6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
    Image image2(width, height);
    raytracer.render_env(camera2, scene, light_list, image2,mat);
    image2.flushPixelBuffer("view2.bmp");
    
    //right forward bottom
    Camera camera3(Point3D(-4, 2, 1), Vector3D(4, -2, -6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
    Image image3(width, height);
    raytracer.render_env(camera3, scene, light_list, image3,mat);
    image3.flushPixelBuffer("view3.bmp");
    
    //right back top
//    Camera camera4(Point3D(0, 0, 0), Vector3D(4, 2, 6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
//    Image image4(width, height);
//    raytracer.render_env(camera4, scene, light_list, image4,mat);
//    image4.flushPixelBuffer("view4.bmp");
//    //right back bottom
//    Camera camera5(Point3D(0, 0, 0), Vector3D(4, -2, 6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
//    Image image5(width, height);
//    raytracer.render_env(camera5, scene, light_list, image5,mat);
//    image5.flushPixelBuffer("view5.bmp");
//
//    Camera camera6(Point3D(0, 0, 0), Vector3D(-4, -2, 6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
//    Image image6(width, height);
//    raytracer.render_env(camera6, scene, light_list, image6,mat);
//    image6.flushPixelBuffer("view6.bmp");
//
//    Camera camera7(Point3D(0, 0, 0), Vector3D(-4, 2, 6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
//    Image image7(width, height);
//    raytracer.render_env(camera7, scene, light_list, image7,mat);
//    image7.flushPixelBuffer("view7.bmp");
//
//    Camera camera8(Point3D(0, 0, 0), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);//init -4 -2 -6
//    Image image8(width, height);
//    raytracer.render_env(camera8, scene, light_list, image8,mat);
//    image8.flushPixelBuffer("view8.bmp");
    
    
    
    
    
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
    
//    //2K
//    int width = 1920;
//    int height = 1080;
    
    //4K
//    int width = 4096;
//    int height = 2160;
    
//    int width = 3200;
//    int height = 2400;
    int width = 640;
    int height =480;
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    init();
    clock_t timeStart = clock();
//    Material *parse = init_env();
//    clock_t timeStart = clock();
//    env_mapping(raytracer,width,height,parse);
    hard_shadow(raytracer,width,height);
    clock_t timeEnd = clock();
    printf("render time: %04.2f (sec)\n",(double)(timeEnd - timeStart) / CLOCKS_PER_SEC);//print run time
    return 0;
}

