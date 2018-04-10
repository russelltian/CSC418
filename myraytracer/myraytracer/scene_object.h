/***********************************************************
 
 Starter code for Assignment 3
 
 
 Classes defining primitives in the scene
 
 ***********************************************************/
#pragma once

#include "util.h"
#include <vector>
static uint64_t numRaySquareTests = 0;
using namespace std;


class Triangle;
class UnitCylinder;
class UnitSquare;
class UnitSphere;

// All primitives should provide an intersection function.
// To create more primitives, inherit from SceneObject.
// Namely, you can create, Sphere, Cylinder, etc... classes
// here.
class Box{
public:
    Box(){a=Point3D(0,0,0);b=Point3D(0,0,0);}
    void expand(const Box &other){
        Point3D min(999999999,999999999,999999999);
        Point3D max(-999999999,-999999999,-999999999);
        Point3D p[4];
        p[0]=this->a;
        p[1]=this->b;
        p[2]=other.a;
        p[3]=other.b;
        for(int i=0;i<4;i++){
            for(int j=0;j<3;j++){
                min[j]=fmin(p[i][j],min[j]);
                max[j]=fmax(p[i][j],max[j]);
            }
        }
        a=min;
        b=max;
        updateLongestAxis();
    }
    void transForm(Matrix4x4 trans){
        Point3D p[8];
        p[0]=trans*a;
        p[1]=trans*Point3D(a[0],a[1],b[2]);
        p[2]=trans*Point3D(a[0],b[1],a[2]);
        p[3]=trans*Point3D(a[0],b[1],b[2]);
        p[4]=trans*Point3D(b[0],a[1],a[2]);
        p[5]=trans*Point3D(b[0],a[1],b[2]);
        p[6]=trans*Point3D(b[0],b[1],a[2]);
        p[7]=trans*Point3D(b[0],b[1],b[2]);
        Point3D min(999999999,999999999,999999999);
        Point3D max(-999999999,-999999999,-999999999);
        for(int i=0;i<8;i++){
            for(int j=0;j<3;j++){
                min[j]=fmin(p[i][j],min[j]);
                max[j]=fmax(p[i][j],max[j]);
            }
        }
        
        a=min;
        b=max;
        updateLongestAxis();
    }
    void updateLongestAxis(){
        double diff[3];
        
        diff[0]=b[0]-a[0];
        diff[1]=b[1]-a[1];
        diff[2]=b[2]-a[2];
        double longest=fmax(fmax(diff[0],diff[1]),diff[2]);
        for(int i=0;i<3;i++){
            if(longest==diff[i]){
                longestAxis=i;
            }
        }
        mid=0.5*(a+b);
    }
    bool hit(const Ray3D& ray){
        double tx1 = (a[0] - ray.origin[0])*(1.0/ray.dir[0]);
        double tx2 = (b[0] - ray.origin[0])*(1.0/ray.dir[0]);
        
        double tmin = min(tx1, tx2);
        double tmax = max(tx1, tx2);
        
        double ty1 = (a[1] - ray.origin[1])*(1.0/ray.dir[1]);
        double ty2 = (b[1] - ray.origin[1])*(1.0/ray.dir[1]);
        
        tmin = max(tmin, min(ty1, ty2));
        tmax = min(tmax, max(ty1, ty2));
        
        double tz1 = (a[2] - ray.origin[2])*(1.0/ray.dir[2]);
        double tz2 = (b[2] - ray.origin[2])*(1.0/ray.dir[2]);
        
        tmin = max(tmin, min(tz1, tz2));
        tmax = min(tmax, max(tz1, tz2));
        
        return tmax >= tmin;
//        return true;
    }
    Point3D a;//min
    Point3D b;//max
    Point3D mid;
    int longestAxis;
};

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
    
    SceneNode(UnitSphere* inobj, Material* mat);
    
    SceneNode(UnitSquare* inobj, Material* mat);
    
    SceneNode(UnitCylinder* inobj, Material* mat);

    
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
    Box bbox;
    Box getBox(){
        return bbox;
    };
    
    
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



class KDNode{
public:
    KDNode* left;
    KDNode* right;
    vector<SceneNode*> items;
    KDNode(){};
//    bool hit(KDNode* node, Ray3D& ray);
    Box bbox;
    
};

KDNode* build(vector<SceneNode*> &itemsin,int depth);
bool KDHit(KDNode* node,KDNode* root, Ray3D& ray);

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
