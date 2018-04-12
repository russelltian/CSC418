
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
void object_init(){
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
    
    Material* waveMat=createMat("Textures/wave03.bmp");
    
    Material* gridMat =createMat("Textures/grid.bmp");
    
    Material *woodMat =createMat("Textures/wood.bmp");
    
    Material *auroraMat=createMat("Textures/aurora_texture.bmp");
    
    Material *rainbowMat=createMat("Textures/rainbow.bmp");
    
    
    //load obj file here
    std::string inputfile = "objs/David2.obj";
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
                SceneNode* thisT = new SceneNode(new Triangle(thisTri[0],thisTri[1],thisTri[2]),gold);
                //order:TRS
                
                thisT->rotate('z',90);
                thisT->translate(Vector3D(-2,-3,-0.6));
                double Tfactor[3] = { 0.008, 0.008, 0.008 };
                thisT->scale(Point3D (0,0,0), Tfactor);
                scene.push_back(thisT);
                
                
                SceneNode* thisT2 = new SceneNode(new Triangle(thisTri[0],thisTri[1],thisTri[2]),gold);
                //order:TRS
                
                thisT2->rotate('z',-90);
                thisT2->translate(Vector3D(2,-2.7,-0.6));
                double Tfactor2[3] = { 0.008, 0.008, 0.008 };
                thisT2->scale(Point3D (0,0,0), Tfactor2);
                scene.push_back(thisT2);
                
                SceneNode* thisT3 = new SceneNode(new Triangle(thisTri[0],thisTri[1],thisTri[2]),gold);
                //order:TRS
                
                thisT3->translate(Vector3D(0,-8,-1));
                double Tfactor3[3] = { 0.008, 0.008, 0.008 };
                thisT3->scale(Point3D (0,0,0), Tfactor3);
                scene.push_back(thisT3);
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
    SceneNode* plane = new SceneNode(new UnitSquare(), jade);
    scene.push_back(plane);
    
    //added a cylinder
    SceneNode* cylinder = new SceneNode(new UnitCylinder(),jade);
    //    scene.push_back(cylinder);
    
    
    
    // Apply some transformations to the sphere and unit square.
    //RTS
    
    
    //all temporary, can delete at any time
    
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), gold);
    //     scene.push_back(sphere2);
    double sphere2F[3]={1.0,1.0,1.0};
    sphere2->translate(Vector3D(0, 1,-5));
    sphere2->scale(Point3D(0,0,0),sphere2F);
    
    
    double factor1[3] = { 1.0, 1.0, 1.0 };
    sphere->translate(Vector3D(0, -1, -5));
    //    sphere->rotate('x', 0);
    //    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -4));
    lens->scale(Point3D(0,0,0), lensfactor);
    double factor2[3] = { 7, 7, 1 };
    //    plane->translate(Vector3D(0, 0, -10));
    //    plane->rotate('z', 45);
    
    plane->scale(Point3D(0,0,0), factor2);
    
    double cyl_factor[3] = {0.5,0.5,2};
    //    cylinder->translate(Vector3D(0, 0, -4));
    cylinder->rotate('y',45);
    //    cylinder->rotate('x',60 );
    cylinder->scale(Point3D(0,0,0), cyl_factor);
    
}

void object_init2(){
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
    
    
    
    //load obj file here
    std::string inputfile = "objs/bulbasaur.obj";
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
                SceneNode* thisT = new SceneNode(new Triangle(thisTri[0],thisTri[1],thisTri[2]),jade);
                //order:TRS
                thisT->rotate('x',-90);
                thisT->rotate('y',180);
                thisT->rotate('z',180);
                thisT->rotate('x',45);
                thisT->rotate('y',45);
                thisT->translate(Vector3D(0,0,0));
                double Tfactor[3] = { 2, 2, 2 };
                thisT->scale(Point3D (0,0,0), Tfactor);
                scene.push_back(thisT);
                
            }
            index_offset += fv;
            
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    
    
    
    SceneNode* plane = new SceneNode(new UnitSquare(), gold);
    //scene.push_back(plane);
    double factor2[3]={6,6,1};
    plane->scale(Point3D(0,0,0), factor2);
    plane->translate(Vector3D(0,-2,0));
}

void init_texture(){
    Material* gold = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                  Color(0.628281, 0.555802, 0.366065),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.7,0.7,0.7),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    
    Material *red = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                 Color(0.628281, 0.555802, 0.366065),
                                 12.8,0.0,1.0,NULL,NULL,NULL);
    
    Material *red_g = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                   Color(0.628281, 0.555802, 0.366065),
                                   12.8,0.0,0.05,NULL,NULL,NULL);
    
    
    Material *blue = new Material(Color(0, 0, 0),                                  Color(0, 0, 0.8),
                                  Color(0.690196,0.316228,0.121569),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material* yellow = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    Material* yellow_g = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                      Color(0.628281, 0.555802, 0.366065),
                                      36,0.0,0.1,NULL,NULL,NULL);
    Material* green = new Material(Color(0.3, 0.3, 0.3), Color(0, 0.8, 0),
                                   Color(0.628281, 0.555802, 0.366065),
                                   36,0.0,1.0,NULL,NULL,NULL);
    Material *glass = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                   Color(0.316228,0.316228,0.316228),
                                   12.8,1.1,1.0,NULL,NULL,NULL);
    
    Material* gridMat=createMat("Textures/grid.bmp");
    Material *woodMat =createMat("Textures/wood.bmp");
    Material *rainBowMat =createMat("Textures/rainbow.bmp");
    Material *auroraMat =createMat("Textures/aurora_texture.bmp");
    Material *waveMat =createMat("Textures/wave03.bmp");
    // Add a unit square into the scene with material mat.
    SceneNode* cylinder = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder);
    
    SceneNode* plane = new SceneNode(new UnitSquare(), woodMat);
    scene.push_back(plane);
    SceneNode* plane1 = new SceneNode(new UnitSquare(), rainBowMat);
    scene.push_back(plane1);
    SceneNode* box = new SceneNode(new UnitCube(), auroraMat);
    scene.push_back(box);
    
    
    SceneNode* cylinder2 = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder2);
    SceneNode* cylinder3 = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder3);
    SceneNode* sphere = new SceneNode(new UnitSphere(), woodMat);
    scene.push_back(sphere);
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), gridMat);
    scene.push_back(sphere2);
    SceneNode* sphere3 = new SceneNode(new UnitSphere(), waveMat);
    scene.push_back(sphere3);
    // Apply some transformations to the sphere and unit square.
    
    double factor1[3] = { 0.5, 0.5, 2.0 };
    cylinder->translate(Vector3D(-2, -1, -6));
    cylinder->rotate('y', 90);
    cylinder->rotate('x', 90);
    cylinder->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 12.0, 12.0, 1.0 };
    //double factor3[3] = {12, 8, 1 };
    double boxfactor[3]={2.0,2.0,2.0};
    box->translate(Vector3D(0, -1, -3));
    box->scale(Point3D(0, 0, 0), boxfactor);
    plane->translate(Vector3D(0, -2, -7));
    plane->rotate('x', 90);
    plane->scale(Point3D(0, 0, 0), factor2);
    plane1->translate(Vector3D(0, 0, -12));
    plane1->scale(Point3D(0, 0, 0), factor2);
    //    plane2->translate(Vector3D(-5, 2, -6));
    //    plane2->rotate('y', 90);
    //    plane2->scale(Point3D(0, 0, 0), factor3);
    
    cylinder2->translate(Vector3D(0.5, -1, -6));
    cylinder2->rotate('y',90);
    cylinder2->rotate('x',90);
    cylinder2->scale(Point3D(0, 0, 0), factor1);
    cylinder3->translate(Vector3D(3, -1, -6));
    cylinder3->rotate('y',90);
    cylinder3->rotate('x',90);
    cylinder3->scale(Point3D(0, 0, 0), factor1);
    
    sphere->translate(Vector3D(-2, 2, -6));
    sphere2->translate(Vector3D(0.5, 2, -6));
    sphere3->translate(Vector3D(3, 2, -6));
}

void init_refraction(){
    Material* gold = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                  Color(0.628281, 0.555802, 0.366065),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.7,0.7,0.7),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    
    Material *red = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                 Color(0.628281, 0.555802, 0.366065),
                                 12.8,0.0,1.0,NULL,NULL,NULL);
    
    Material *red_g = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                   Color(0.628281, 0.555802, 0.366065),
                                   12.8,0.0,0.05,NULL,NULL,NULL);
    
    
    Material *blue = new Material(Color(0, 0, 0),                                  Color(0, 0, 0.8),
                                  Color(0.690196,0.316228,0.121569),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material* yellow = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    Material* yellow_g = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                      Color(0.628281, 0.555802, 0.366065),
                                      36,0.0,0.1,NULL,NULL,NULL);
    Material* green = new Material(Color(0.3, 0.3, 0.3), Color(0, 0.8, 0),
                                   Color(0.628281, 0.555802, 0.366065),
                                   36,0.0,1.0,NULL,NULL,NULL);
    Material *glass = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                   Color(0.316228,0.316228,0.316228),
                                   12.8,1.1,1.0,NULL,NULL,NULL);
    
    Material* gridMat=createMat("Textures/grid.bmp");
    Material *woodMat =createMat("Textures/wood.bmp");
    Material *rainBowMat =createMat("Textures/rainbow.bmp");
    Material *auroraMat =createMat("Textures/aurora_texture.bmp");
    Material *waveMat =createMat("Textures/wave03.bmp");
    // Add a unit square into the scene with material mat.
    
    SceneNode* plane = new SceneNode(new UnitSquare(), gridMat);
    scene.push_back(plane);
    SceneNode* plane1 = new SceneNode(new UnitSquare(), woodMat);
    scene.push_back(plane1);
    
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), glass);
    scene.push_back(sphere);
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), glass);
    scene.push_back(sphere2);
    SceneNode* sphere3 = new SceneNode(new UnitSphere(), glass);
    scene.push_back(sphere3);
    // Apply some transformations to the sphere and unit square.
    
    
    
    double factor2[3] = { 12.0, 12.0, 1.0 };
    //double factor3[3] = {12, 8, 1 };
    plane->translate(Vector3D(0, -2, -7));
    plane->rotate('x', 90);
    plane->scale(Point3D(0, 0, 0), factor2);
    plane1->translate(Vector3D(0, 0, -12));
    plane1->scale(Point3D(0, 0, 0), factor2);
    //    plane2->translate(Vector3D(-5, 2, -6));
    //    plane2->rotate('y', 90);
    //    plane2->scale(Point3D(0, 0, 0), factor3);
    
    
    sphere->translate(Vector3D(-2, -1, -6));
    sphere2->translate(Vector3D(0.5, 0, -6));
    sphere3->translate(Vector3D(3, 1, -6));
}


//for environment mapping
Material* init_env(){
    Material *gold=new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                Color(0.628281, 0.555802, 0.366065),
                                51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.9,0.9,0.9),
                                    Color(0.628281, 0.555802, 0.366065),
                                    51.2,0.0,1.0,NULL,NULL,NULL);
    Material *jade = new Material(Color(0, 0, 0), Color(0.54,0.89,0.63),
                                  Color(0.316228,0.316228,0.316228),
                                  12.8,0.0,1.0,NULL,NULL,NULL);
    //canyon scene
    Material *canyonMat =  createMat("canyon.bmp");
    
    //skysea scene
    // Material *skyseaMat = createMat("Textures/cloudySea.bmp");
    
    
    double factor3[3] = { 1.0, 1.0, 1.0 };
    
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), gold);
    scene.push_back(sphere);
    sphere->translate(Vector3D(0, 0, -3));
    sphere->scale(Point3D(0,0,0),factor3);
    
    return canyonMat;
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
    
    Material* waveMat=createMat("Textures/wave03.bmp");
    
    Material* gridMat =createMat("Textures/grid.bmp");
    
    Material *woodMat =createMat("Textures/wood.bmp");
    
    Material *auroraMat=createMat("Textures/aurora_texture.bmp");
    
    Material *rainbowMat=createMat("Textures/rainbow.bmp");
    
    
    //load obj file here
    std::string inputfile = "objs/cube.obj";
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
                //                scene.push_back(thisT);
            }
            index_offset += fv;
            
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    
    
    //standard objects in the scene by default
    
    SceneNode* sphere = new SceneNode(new UnitSphere(), gold);
    scene.push_back(sphere);
    SceneNode* lens = new SceneNode(new UnitSphere(), glass);
    //    scene.push_back(lens);
    SceneNode* plane = new SceneNode(new UnitSquare(), jade);
    scene.push_back(plane);
    
    //added a cylinder
    SceneNode* cylinder = new SceneNode(new UnitCylinder(),jade);
    //scene.push_back(cylinder);
    
    
    
    // Apply some transformations to the sphere and unit square.
    //RTS
    //all temporary, can delete at any time
    
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), gold);
    scene.push_back(sphere2);
    double sphere2F[3]={1.0,1.0,1.0};
    sphere2->translate(Vector3D(0, 1,-5));
    sphere2->scale(Point3D(0,0,0),sphere2F);
    
    
    double factor1[3] = { 1.0, 1.0, 1.0 };
    sphere->translate(Vector3D(0, -1, -5));
    //    sphere->rotate('x', 0);
    //    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double lensfactor[3] = { 1.0, 1.0, 1.0 };
    lens->translate(Vector3D(0, 0, -4));
    lens->scale(Point3D(0,0,0), lensfactor);
    double factor2[3] = { 6, 6, 1 };
    plane->translate(Vector3D(0, 0, -10));
    //plane->rotate('z', 45);
    
    // plane->scale(Point3D(0,0,0), factor2);
    
    double cyl_factor[3] = {0.5,0.5,2.0};
    cylinder->translate(Vector3D(0, 0, -4));
    //    cylinder->rotate('y',45);
    //    cylinder->rotate('x',60 );
    //    cylinder->scale(Point3D(0,0,0), cyl_factor);
    
}

void hard_shadow(Raytracer& raytracer,int width,int height){
    
    // Defines a point light source.
    LightList light_list;
    //  PointLight* pLight = new PointLight(Point3D(2,-2,4), Color(0.9,0.9,0.9));
    PointLight* pLight = new PointLight(Point3D(0,0,5), Color(0.9,0.9,0.9));
    //  PointLight* pLight = new PointLight(Point3D(-5,-5,10), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    //    Camera camera1(Point3D(-15,-15,15), Vector3D(15, 15, -15), Vector3D(0, 1, 0), 60.0);
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    std::cout << "finished View1" << std::endl;
    // Render it from a different point of view.
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    //        Camera camera2(Point3D(0, -3, 3), Vector3D(0, 3, -3), Vector3D(0, 1, 0), 60.0);
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

void hard_shadow_for_obj(Raytracer& raytracer,int width,int height){
    
    // Defines a point light source.
    LightList light_list;
    //  PointLight* pLight = new PointLight(Point3D(2,-2,4), Color(0.9,0.9,0.9));
    PointLight* pLight = new PointLight(Point3D(0,-5,5), Color(0.9,0.9,0.9));
    //  PointLight* pLight = new PointLight(Point3D(-5,-5,10), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    //    Camera camera1(Point3D(-15,-15,15), Vector3D(15, 15, -15), Vector3D(0, 1, 0), 60.0);
    Camera camera1(Point3D(0, -8, 3), Vector3D(0, 8, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    std::cout << "finished View1" << std::endl;
    
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
    //AreaLight* aLight = new AreaLight(Point3D(3,0,5), Color(0.9,0.9,0.9),Vector3D(0.1,0,0),Vector3D(0,0.1,0));
    AreaLight* aLight = new AreaLight(Point3D(0,3,5), Color(0.9,0.9,0.9),Vector3D(0.1,0,0),Vector3D(0,0.1,0));
    light_list.push_back(aLight);
    
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    Image image1(width, height);
    raytracer.render(camera1, scene, light_list, image1); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    std::cout << "finished View1" << std::endl;
    
    //   Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
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


void DOF(Raytracer& raytracer,int width,int height){
    //added by us, define a extended area light source to make soft shadowing
    LightList light_list;
    //    AreaLight* aLight = new AreaLight(Point3D(0,0,5), Color(0.9,0.9,0.9),Vector3D(0.1,0,0),Vector3D(0,0.1,0));
    //    light_list.push_back(aLight);
    PointLight* pLight = new PointLight(Point3D(3,3,5), Color(0.9,0.9,0.9));
    light_list.push_back(pLight);
    
    // Render the scene, feel free to make the image smaller for
    // testing purposes.
    double focus_point = 5.5; //where the lens is focusing
    //    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);
    //    Image image1(width, height);
    //    raytracer.render_dof(camera1, scene, light_list, image1,focus_point); //render 3D scene to image
    //    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    std::cout << "finished View1" << std::endl;
    
    // Render it from a different point of view.
    
    Camera camera2(Point3D(4, 2, 1), Vector3D(-4, -2, -6), Vector3D(0, 1, 0), 60.0);
    Image image2(width, height);
    raytracer.render_dof(camera2, scene, light_list, image2,focus_point);
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

void env_mapping(Raytracer& raytracer,int width,int height,Material* mat){
    LightList light_list;
    
    Camera camera1(Point3D(0, 0, 1), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 60.0);//0,0,-1 init
    Image image1(width, height);
    raytracer.render_env(camera1, scene, light_list, image1,mat); //render 3D scene to image
    image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
    
    
    // Free memory
    for (size_t i = 0; i < scene.size(); ++i) {
        delete scene[i];
    }
    
    for (size_t i = 0; i < light_list.size(); ++i) {
        delete light_list[i];
    }
}

//the first scene we get
void default_init(){
    Material* gold = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                  Color(0.628281, 0.555802, 0.366065),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    Material* jade = new Material(Color(0, 0, 0), Color(0.54,0.89,0.63),
                                  Color(0.316228,0.316228,0.316228),
                                  12.8,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.9,0.9,0.9),
                                    Color(0.628281, 0.555802, 0.366065),
                                    51.2,0.0,1.0,NULL,NULL,NULL);
    
    // Add a unit square into the scene with material mat.
    SceneNode* sphere = new SceneNode(new UnitSphere(), gold);
    //scene.push_back(sphere);
    SceneNode* plane = new SceneNode(new UnitSquare(), jade);
    scene.push_back(plane);
    SceneNode* sphere1 = new SceneNode(new UnitSphere(), ranColor);
    scene.push_back(sphere1);
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), jade);
    scene.push_back(sphere2);
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 1.0, 2.0, 1.0 };
    sphere->translate(Vector3D(0, 0, -5));
    sphere->rotate('x', -45);
    sphere->rotate('z', 45);
    sphere->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 6.0, 6.0, 6.0 };
    plane->translate(Vector3D(0, 0, -10));
    plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    sphere1->translate(Vector3D(3, 0, -5));
    sphere2->translate(Vector3D(2, 0, -1));
    
}

//the first scene we get
void init_cyl(){
    Material* gold = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                  Color(0.628281, 0.555802, 0.366065),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.7,0.7,0.7),
                                    Color(0.628281, 0.555802, 0.366065),
                                    51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material *red = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                 Color(0.628281, 0.555802, 0.366065),
                                 12.8,0.0,1.0,NULL,NULL,NULL);
    
    Material *blue = new Material(Color(0, 0, 0),                                  Color(0, 0, 0.8),
                                  Color(0.690196,0.316228,0.121569),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material* yellow = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    // Add a unit square into the scene with material mat.
    SceneNode* cylinder = new SceneNode(new UnitCylinder(), gold);
    scene.push_back(cylinder);
    SceneNode* plane = new SceneNode(new UnitSquare(), ranColor);
    scene.push_back(plane);
    SceneNode* plane1 = new SceneNode(new UnitSquare(), ranColor);
    scene.push_back(plane1);
    SceneNode* cylinder1 = new SceneNode(new UnitCylinder(), red);
    scene.push_back(cylinder1);
    SceneNode* cylinder2 = new SceneNode(new UnitCylinder(), blue);
    scene.push_back(cylinder2);
    SceneNode* cylinder3 = new SceneNode(new UnitCylinder(), yellow);
    scene.push_back(cylinder3);
    // Apply some transformations to the sphere and unit square.
    double factor1[3] = { 0.5, 0.5, 1.0 };
    cylinder->translate(Vector3D(-2, 0, -6));
    cylinder->rotate('x', -90);
    cylinder->rotate('z', 45);
    cylinder->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 10.0, 10.0, 1.0 };
    plane->translate(Vector3D(0, -1, -7));
    plane->rotate('x', 90);
    //plane->rotate('z', 45);
    plane->scale(Point3D(0, 0, 0), factor2);
    
    plane1->translate(Vector3D(0, 0, -12));
    //plane->rotate('z', 45);
    plane1->scale(Point3D(0, 0, 0), factor2);
    
    cylinder1->translate(Vector3D(0, 0, -8));
    
    cylinder1->rotate('y',90);
    cylinder1->scale(Point3D(0, 0, 0), factor1);
    cylinder2->translate(Vector3D(2, 0, -8));
    cylinder2->scale(Point3D(0, 0, 0), factor1);
    cylinder3->translate(Vector3D(3, 0, -6));
    cylinder3->rotate('y',90);
    cylinder3->rotate('x',90);
    cylinder3->scale(Point3D(0, 0, 0), factor1);
    
}

//the first scene we get
void init_glossy(){
    Material* gold = new Material(Color(0.3, 0.3, 0.3), Color(0.75164,0.60648,0.22648),
                                  Color(0.628281, 0.555802, 0.366065),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    Material *ranColor=new Material(Color(0.3, 0.3, 0.3), Color(0.7,0.7,0.7),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    
    Material *red = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                 Color(0.628281, 0.555802, 0.366065),
                                 12.8,0.0,1.0,NULL,NULL,NULL);
    
    Material *red_g = new Material(Color(0.1, 0.1, 0.1), Color(0.690196,0.090196,0.121569),
                                   Color(0.628281, 0.555802, 0.366065),
                                   12.8,0.0,0.05,NULL,NULL,NULL);
    
    
    Material *blue = new Material(Color(0, 0, 0),                                  Color(0, 0, 0.8),
                                  Color(0.690196,0.316228,0.121569),
                                  51.2,0.0,1.0,NULL,NULL,NULL);
    
    Material* yellow = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                    Color(0.628281, 0.555802, 0.366065),
                                    36,0.0,1.0,NULL,NULL,NULL);
    Material* yellow_g = new Material(Color(0.3, 0.3, 0.3), Color(1, 0.6, 0.070),
                                      Color(0.628281, 0.555802, 0.366065),
                                      36,0.0,0.1,NULL,NULL,NULL);
    Material* green = new Material(Color(0.3, 0.3, 0.3), Color(0, 0.8, 0),
                                   Color(0.628281, 0.555802, 0.366065),
                                   36,0.0,1.0,NULL,NULL,NULL);
    
    // Add a unit square into the scene with material mat.
    SceneNode* cylinder = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder);
    
    SceneNode* plane = new SceneNode(new UnitSquare(), ranColor);
    scene.push_back(plane);
    SceneNode* plane1 = new SceneNode(new UnitSquare(), ranColor);
    scene.push_back(plane1);
    SceneNode* plane2 = new SceneNode(new UnitSquare(), ranColor);
    scene.push_back(plane2);
    
    
    
    SceneNode* cylinder2 = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder2);
    SceneNode* cylinder3 = new SceneNode(new UnitCylinder(), ranColor);
    scene.push_back(cylinder3);
    SceneNode* sphere = new SceneNode(new UnitSphere(), green);
    scene.push_back(sphere);
    SceneNode* sphere2 = new SceneNode(new UnitSphere(), red_g);
    scene.push_back(sphere2);
    SceneNode* sphere3 = new SceneNode(new UnitSphere(), yellow_g);
    scene.push_back(sphere3);
    // Apply some transformations to the sphere and unit square.
    
    double factor1[3] = { 0.5, 0.5, 2.0 };
    cylinder->translate(Vector3D(-2, -1, -6));
    cylinder->rotate('y', 90);
    cylinder->rotate('x', 90);
    cylinder->scale(Point3D(0, 0, 0), factor1);
    
    double factor2[3] = { 12.0, 12.0, 1.0 };
    double factor3[3] = {12, 8, 1 };
    
    plane->translate(Vector3D(0, -2, -7));
    plane->rotate('x', 90);
    plane->scale(Point3D(0, 0, 0), factor2);
    plane1->translate(Vector3D(0, 0, -12));
    plane1->scale(Point3D(0, 0, 0), factor2);
    plane2->translate(Vector3D(-5, 2, -6));
    plane2->rotate('y', 90);
    plane2->scale(Point3D(0, 0, 0), factor3);
    
    cylinder2->translate(Vector3D(0.5, -1, -6));
    cylinder2->rotate('y',90);
    cylinder2->rotate('x',90);
    cylinder2->scale(Point3D(0, 0, 0), factor1);
    cylinder3->translate(Vector3D(3, -1, -6));
    cylinder3->rotate('y',90);
    cylinder3->rotate('x',90);
    cylinder3->scale(Point3D(0, 0, 0), factor1);
    
    sphere->translate(Vector3D(-2, 2, -6));
    sphere2->translate(Vector3D(0.5, 2, -6));
    sphere3->translate(Vector3D(3, 2, -6));
    
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
    int width = 1920;
    int height = 1080;
    //    int width = 2480;
    //   int height = 1748;
    //    int width = 1440;
    //    int height = 1080;
    //4K
    //    int width = 4096;
    //    int height = 2160;
    
    //        int width = 3200;
    //        int height = 2400;
    //    int width = 320;
    //    int height =240;
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    
    
    //pick the scene initialization to start
    
    //init();
   // object_init();
    // object_init2();
    //  init_texture();
    //  init_refraction();
    //  default_init();
    //  init_cyl();
    //  init_glossy();
    Material *parse = init_env();
    
    
    
    
    clock_t timeStart = clock();
     env_mapping(raytracer,width,height,parse);
    
    
    //Please use hard_shadow_for_obj for mesh demo
   // hard_shadow_for_obj(raytracer,width,height);
    
    //  soft_shadow(raytracer,width,height);
    //hard_shadow(raytracer,width,height);
    //  DOF((raytracer,width,height);
    clock_t timeEnd = clock();
    printf("render time: %04.2f (sec)\n",(double)(timeEnd - timeStart) / CLOCKS_PER_SEC);//print run time
    return 0;
}


