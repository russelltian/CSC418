/***********************************************************
 
 Starter code for Assignment 3
 
 
 Classes defining primitives in the scene
 
 ***********************************************************/
#pragma once

#include "util.h"
#include <vector>
static uint64_t numRaySquareTests = 0;



class Triangle;

// All primitives should provide an intersection function.
// To create more primitives, inherit from SceneObject.
// Namely, you can create, Sphere, Cylinder, etc... classes
// here.
class SceneObject {
    public:
    // Returns true if an intersection occured, false otherwise.
    virtual bool intersect(Ray3D&, const Matrix4x4&, const Matrix4x4&) = 0;
    virtual ~SceneObject(){}
};

// Scene node containing information about an object: geometry, material,
// tranformations.
struct SceneNode {
    
    SceneNode()
    :
    obj(NULL), mat(NULL),ismesh(false){}
    
    SceneNode(bool isMesh)
    :
    obj(NULL), mat(NULL),ismesh(isMesh){}
    
    SceneNode(SceneObject* obj, Material* mat)
    :
    obj(obj), mat(mat),ismesh(false) {}
    
    SceneNode(Triangle* obj, Material* mat);

    
    ~SceneNode() {
        if (obj) delete obj;
    }
    
    // Apply rotation about axis 'x', 'y', 'z' angle degrees to node.
    void rotate(char axis, double angle);
    
    // Apply translation in the direction of trans to node.
    void translate(Vector3D trans);
    
    // Apply scaling about a fixed point origin.
    void scale(Point3D origin, double factor[3]);
    
    // Pointer to geometry primitive, used for intersection.
    SceneObject* obj;
    
    // Pointer to material of the object, used in shading.
    Material* mat;
    
    bool ismesh;
	bool firstTouch;
    
    // Each node maintains a transformation matrix, which maps the
    // geometry from object space to world space and the inverse.
    Matrix4x4 trans;
    Matrix4x4 invtrans;
    Matrix4x4 modelToWorld;
    Matrix4x4 worldToModel;
    Vector3D normal;
};

// Scene is simply implemented as a list of nodes. Doesnt support hierarchy(scene graph).
typedef std::vector<SceneNode*> Scene;

// Example primitive you can create, this is a unit square on
// the xy-plane.
class UnitSquare : public SceneObject {
    public:
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                   const Matrix4x4& modelToWorld);
};

class UnitSphere : public SceneObject {
    public:
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                   const Matrix4x4& modelToWorld);
};

class UnitCylinder : public SceneObject {
    public:
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld);
    
};


class UnitTriangle: public SceneObject{
public:
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                   const Matrix4x4& modelToWorld);
};

class Triangle: public SceneObject{
    public:
    Triangle(Point3D origin,Point3D iu, Point3D iv);
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                    const Matrix4x4& modelToWorld);
    Point3D o;
    Vector3D u;
    Vector3D v;
};

class UnitCube: public SceneObject{
public:
    bool intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                   const Matrix4x4& modelToWorld);
};
