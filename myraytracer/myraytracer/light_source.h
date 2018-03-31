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
};

// List of all light sources in your scene
typedef std::vector<LightSource*> LightList;

// A point light is defined by its position in world space and its
// color.
class PointLight : public LightSource {
    public:
    PointLight(Point3D pos, Color col)
    :
    pos(pos), col_ambient(col), col_diffuse(col), col_specular(col) {}
    
    PointLight(Point3D pos, Color ambient, Color diffuse, Color specular)
    :
    pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular) {}
    
    void shade(Ray3D& ray);
    
    Point3D get_position() const { return pos; }
    
    private:
    Point3D pos;
    Color col_ambient;
    Color col_diffuse;
    Color col_specular;
};

//added by us, extended lightsource to produce softshadow

class ExtendedLight : public LightSource {
public:
    // add length and width to make it as a square
    ExtendedLight(Point3D pos, Color col, Vector3D l, Vector3D w)
    :
    pos(pos), col_ambient(col), col_diffuse(col), col_specular(col),
    length(l),width(w){}
    
    ExtendedLight(Point3D pos, Color ambient, Color diffuse, Color specular,Vector3D l, Vector3D w)
    :
    pos(pos), col_ambient(ambient), col_diffuse(diffuse), col_specular(specular),
    length(l),width(w) {}
    
    void shade(Ray3D& ray);
    
    Point3D get_position() const { return pos; }
    
private:
    Point3D pos;
    Color col_ambient;
    Color col_diffuse;
    Color col_specular;
    Vector3D length;
    Vector3D width;
};


