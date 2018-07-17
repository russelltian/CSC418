/***********************************************************
 
 Starter code for Assignment 3
 
 Light source classes
 
 ***********************************************************/
#pragma once

#include "util.h"
#include <vector>

// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray
// differently.
class LightSource {
    public:
    virtual void shade(Ray3D&) = 0;
    virtual Point3D get_position() const = 0;
    virtual ~LightSource() {}
   
    //our new function
    //the value will not be zero for area light
    virtual int get_type() const = 0; //added to return type of light
    virtual Point3D get_many_position(int i,int j) const =0; //used for area light to get uniform distribution
};

// List of all light sources in your scene
typedef std::vector<LightSource*> LightList;

// A point light is defined by its position in world space and its
// color.
class PointLight : public LightSource {
    public:
    PointLight(Point3D pos, Color col)
    :
    pos(pos), col_ambient(col), col_diffuse(col), col_specular(col) {type = 0;}
    
    PointLight(Point3D pos, Color ambient, Color diffuse, Color specular)
    :
    pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular) {type = 0;}
    
    void shade(Ray3D& ray);
    
    Point3D get_position() const { return pos; }
    
    //our functions
    int get_type() const{return type;} //point light: 0
    Point3D get_many_position(int i,int j) const { return pos; }//no need in pointlight, only one position
    private:
    Point3D pos;
    Color col_ambient;
    Color col_diffuse;
    Color col_specular;
    int type;//equals to 0, point light
};

//added by us, area lightsource to produce softshadow
class AreaLight : public LightSource {
public:
    // add length and width to make it as a square
    AreaLight(Point3D pos, Color col, Vector3D l, Vector3D w)
    :
    pos(pos), col_ambient(col), col_diffuse(col), col_specular(col),
    length(l),width(w){type=1;}
    
    AreaLight(Point3D pos, Color ambient, Color diffuse, Color specular,Vector3D l, Vector3D w)
    :
    pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular),
    length(l),width(w) {type=1;}
    
    void shade(Ray3D& ray);
    
    Point3D get_position()const { return pos; } //no need in area light
    Point3D get_many_position(int i,int j) const;// filled in light_source.cpp
    int get_type() const{return type;} 
private:
    Point3D pos;
    Color col_ambient;
    Color col_diffuse;
    Color col_specular;
    //actually length and width defines the light plane
    // i, j is used to locate the random point light position
    Vector3D length;
    Vector3D width;
   
    int type; //equals to 1, area light
};


