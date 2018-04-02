/***********************************************************
 
 Starter code for Assignment 3
 
 Implements scene_object.h
 
 ***********************************************************/

#include <cmath>
#include "scene_object.h"
#include <math.h>
#define PI 3.14159265

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    // TODO: implement intersection code for UnitSquare, which is
    // defined on the xy-plane, with vertices (0.5, 0.5, 0),
    // (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
    // (0, 0, 1).
    //
    // Your goal here is to fill ray.intersection with correct values
    // should an intersection occur.  This includes intersection.point,
    // intersection.normal, intersection.none, intersection.t_value.
    //
    // HINT: Remember to first transform the ray into object space
    // to simplify the intersection test.
    
    //transform the ray(origin,direction) to object space
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    // direction.normalize();
    double t = -origin[2]/direction[2];
    
    //invalid intersection
    if(t <= 0.0000000001 || direction[2] == 0||(!ray.intersection.none&&t>ray.intersection.t_value)){
        return false;
    }
    
    Point3D p = origin + t*direction;
    Vector3D normal = Vector3D(0,0,1);
    if(normal.dot(direction)>0.0000001)normal = Vector3D(0,0,-1);

    if(p[0] >= -0.5 && p[0] <= 0.5 && p[1] >= -0.5 && p[1] <= 0.5){
        ray.intersection.t_value = t;
        ray.intersection.point = modelToWorld*p;
        ray.intersection.localPos=p;
        ray.intersection.normal = transNorm(worldToModel, normal);
//        ray.intersection.none = false; // there is an intersection
        return true;
    }
    //ray.intersection.none = true;
    return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    // TODO: implement intersection code for UnitSphere, which is centred
    // on the origin.
    //
    // Your goal here is to fill ray.intersection with correct values
    // should an intersection occur.  This includes intersection.point,
    // intersection.normal, intersection.none, intersection.t_value.
    //
    // HINT: Remember to first transform the ray into object space
    // to simplify the intersection test.
    Point3D origin = worldToModel * ray.origin;
    Vector3D originV(origin[0],origin[1],origin[2]);
    Vector3D direction = worldToModel * ray.dir;
    
    direction.normalize();
    
    double t;
    double temp=direction.dot(originV);
    bool exist=(temp*temp-originV.length()*originV.length()+1)>=0;
//    std::cout<<exist<<std::endl;
    t=-temp-sqrt(temp*temp-originV.length()*originV.length()+1);
    //invalid intersection
    
    
    
    if(!exist||(!ray.intersection.none&&t>ray.intersection.t_value)){
        return false;
    }else if(t<-0.00000001){
            return false;
    }
    
    Point3D p = origin + t*direction;
    Vector3D normal = Vector3D(p[0],p[1],p[2]);
    
    
    

    normal.normalize();
    double u=atan2(normal[3],normal[0])/(2*PI);
    double v=-asin(normal[1])/PI;
    
//    std::cout<<"u: "<<u<<" v "<<v<<std::endl;
    
    ray.intersection.localPos = Point3D(u,v,0);
    ray.intersection.t_value =t;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
    ray.intersection.normal.normalize();
//        ray.intersection.none=false;
        return true;
//    }
//    return false;
}

//compute intersection of a cylinder
bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel,const Matrix4x4& modelToWorld){
    //transform the ray(origin,direction) to object space
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    direction.normalize();
    double t0,t1;
    
    
    double a = direction[0]*direction[0]+direction[1]*direction[1];
    double b = 2*origin[0] * direction[0]
    + 2 * origin[1] * direction[1];
    double c = origin[0] * origin[0]
    + origin[1] * origin[1]- 1;
    double root = b*b - 4*a*c; //b^2 - 4ac
    if(root < 0) return false; //no intersection
    
    double root_sq = sqrtf(root);
    t0 = (-b + root_sq)/(2*a);
    t1 = (-b - root_sq)/(2*a);
    if(t0>t1){// swap the sequence, t0 always smaller
        double tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
//    double y0 = origin[1] + t0*direction[1];
//    double y1 = origin[1] + t1*direction[1];
    Point3D p0 = origin + t0*direction;
    Point3D p1 = origin + t1*direction;
    Point3D p;
    double t;
    Vector3D normal;
    double threshold=-0.00000001;
    if(t0>threshold&&p0[2]<1&&p0[2]>-1){
        p=p0;
        t=t0;
        normal= Vector3D(p[0],p[1],0.0);
    }else if(t1>threshold&&p1[2]<1&&p1[2]>-1){
        p=p1;
        t=t1;
        normal= Vector3D(-p[0],-p[1],0.0);
    }else{
        return false;
    }
    
    if(root<0||(!ray.intersection.none&&t0>ray.intersection.t_value)){
        return false;
    }else if(t<-0.00000001){
        return false;
    }
    
    
    normal.normalize();
    double u=atan2(normal[1],normal[0])/(2*PI);
    double v=p[2]/2.0;
    
    ray.intersection.localPos = Point3D(u,v,0);
    ray.intersection.t_value =t;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
    ray.intersection.normal.normalize();
    return true;
//    if(y0<-1){
//        if (y1<-1) return false;
//        else
//        {   // hit the top cap
//            double th = t0 + (t1-t0) * (y0+1) / (y0-y1);
//            if (th<=0) return false;
//            Point3D p(origin + (th*direction));
//            Vector3D normal(0,-1,0);
//            ray.intersection.localPos = p;
//            ray.intersection.t_value = th;
//            ray.intersection.point = modelToWorld*p;
//            ray.intersection.normal = transNorm(worldToModel, normal);
//            return true;
//
//        }
//    }
//    else if (y0>=-1 && y0<=1)
//    {
//        // hit the cylinder bit
//        if (t0<=0) return false;
//        Point3D p(origin + (t0*direction));
//        ray.intersection.localPos = p;
//        ray.intersection.point = modelToWorld*p;
//        ray.intersection.t_value = t0;
//        Vector3D normal(p[0],0,p[2]);
//        ray.intersection.normal = normal;
//        ray.intersection.normal.normalize();
//        ray.intersection.normal = transNorm(worldToModel, normal);
//        return true;
//    }
//    else if (y0>1)
//    {
//        if (y1>1)
//            return false;
//        else
//        {
//            // hit the bottom cap
//            float th = t0 + (t1-t0) * (y0-1) / (y0-y1);
//            if (th<=0) return false;
//            Point3D p(origin + (th*direction));
//            ray.intersection.localPos = p;
//            ray.intersection.point = modelToWorld*p;
//            ray.intersection.t_value = th;
//            Vector3D normal(0,1,0);
//            ray.intersection.normal = transNorm(worldToModel, normal);
//            return true;
//        }
//    }
//    return false;
}





void SceneNode::rotate(char axis, double angle) {
    Matrix4x4 rotation;
    double toRadian = 2*M_PI/360.0;
    int i;
    
    for (i = 0; i < 2; i++) {
        switch(axis) {
            case 'x':
            rotation[0][0] = 1;
            rotation[1][1] = cos(angle*toRadian);
            rotation[1][2] = -sin(angle*toRadian);
            rotation[2][1] = sin(angle*toRadian);
            rotation[2][2] = cos(angle*toRadian);
            rotation[3][3] = 1;
            break;
            case 'y':
            rotation[0][0] = cos(angle*toRadian);
            rotation[0][2] = sin(angle*toRadian);
            rotation[1][1] = 1;
            rotation[2][0] = -sin(angle*toRadian);
            rotation[2][2] = cos(angle*toRadian);
            rotation[3][3] = 1;
            break;
            case 'z':
            rotation[0][0] = cos(angle*toRadian);
            rotation[0][1] = -sin(angle*toRadian);
            rotation[1][0] = sin(angle*toRadian);
            rotation[1][1] = cos(angle*toRadian);
            rotation[2][2] = 1;
            rotation[3][3] = 1;
            break;
        }
        if (i == 0) {
            this->trans = this->trans*rotation;
            angle = -angle;
        }
        else {
            this->invtrans = rotation*this->invtrans;
        }
    }
}

void SceneNode::translate(Vector3D trans) {
    Matrix4x4 translation;
    
    translation[0][3] = trans[0];
    translation[1][3] = trans[1];
    translation[2][3] = trans[2];
    this->trans = this->trans*translation;
    translation[0][3] = -trans[0];
    translation[1][3] = -trans[1];
    translation[2][3] = -trans[2];
    this->invtrans = translation*this->invtrans;
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
    Matrix4x4 scale;
    
    scale[0][0] = factor[0];
    scale[0][3] = origin[0] - factor[0] * origin[0];
    scale[1][1] = factor[1];
    scale[1][3] = origin[1] - factor[1] * origin[1];
    scale[2][2] = factor[2];
    scale[2][3] = origin[2] - factor[2] * origin[2];
    this->trans = this->trans*scale;
    scale[0][0] = 1/factor[0];
    scale[0][3] = origin[0] - 1/factor[0] * origin[0];
    scale[1][1] = 1/factor[1];
    scale[1][3] = origin[1] - 1/factor[1] * origin[1];
    scale[2][2] = 1/factor[2];
    scale[2][3] = origin[2] - 1/factor[2] * origin[2];
    this->invtrans = scale*this->invtrans;
}



