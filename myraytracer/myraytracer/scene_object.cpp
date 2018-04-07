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
        return true;
    }
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


//cylinder intersection, referenced online algorithm
bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                           const Matrix4x4& modelToWorld) {
    //transform to object space
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    direction.normalize();
    
    //referenced algorithm,solve for wall intersection
    double a = direction[0]*direction[0] + direction[1]*direction[1];
    double b = 2*origin[0]*direction[0]+2*origin[1]*direction[1];
    double c = origin[0]*origin[0] + origin[1]*origin[1]- 1;
    double delta = b*b - 4*a*c;
    //if no solution,return false
    double threshold =0.00000001;
    if(delta < threshold)return false;
    //find two root
    double t0 = (-b - sqrt(delta))/(2*a);
    double t1 = (-b + sqrt(delta))/(2*a);
    if(t0>t1){
        double tmp = t0;
        t0 = t1;
        t1 = tmp; //swap
    }
    //always t0 < t1
    if(t0 < threshold)return false; //behind camera
    bool has_inter = false;
    double z_max = 1;
    double z_min = -1;
    
    //check if hit close or far cap
    double t3 = (z_min - origin[2])/direction[2];
    double t4 = (z_max - origin[2])/direction[2];
    bool swaped = false;
    if (t4<t3){
        double tmp = t3;
        t3 = t4;
        t4 = tmp;
        swaped = true;
    }//t3 always close cap
    //by math, t3<t4, so compute close cap first
    if(t3>threshold ){
        double x3 = origin[0] + t3*direction[0];
        double y3 = origin[1] + t3*direction[1];
        if (x3*x3+y3*y3 <= 1) {
            Point3D p(origin + t3*direction);
            Vector3D normal(0,0,-1);
            if (swaped) normal = -1* normal; //rotation, so normal should be flipped
            ray.intersection.localPos = p;
            ray.intersection.t_value =t3;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            return true;
        }
    }
  /*  if(t4> threshold){
        double x4 = origin[0] + t4*direction[0];
        double y4 = origin[1] + t4*direction[1];
        if (x4*x4+y4*y4 <= 1) {
            Point3D p(origin + t4*direction);
            Vector3D normal(0,0,1);
            ray.intersection.localPos = p;
            ray.intersection.t_value =t4;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            return true;
        }
    }*/
    //now compute wall intersection
    Point3D  p0(origin + t0*direction);
    Point3D  p1(origin + t1*direction);
    //check depth of the intersection
    if(p0[2]<z_max&& p0[2]>z_min){
        t0 = t0;
        has_inter = true;
    }else if(p1[2]<z_max&& p1[2]>z_min){
        t0 = t1;
        has_inter = true;
    }
    if (has_inter){
    Point3D p(origin + t0*direction);
    Vector3D normal(p[0],p[1],0);
    normal.normalize();
    ray.intersection.localPos = p;
    ray.intersection.t_value =t0;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
    return true;//purpose bug
    }
  /*  if(t3>threshold){
        double x3 = origin[0] + t3*direction[0];
        double y3 = origin[1] + t3*direction[1];
        if (x3*x3+y3*y3 <= 1) {
            Point3D p(origin + t3*direction);
            Vector3D normal(0,0,-1);
            ray.intersection.localPos = p;
            ray.intersection.t_value =t3;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            return true;
        }
    }*/
    //then far cap
    if(t4> threshold ){
        double x4 = origin[0] + t4*direction[0];
        double y4 = origin[1] + t4*direction[1];
        if (x4*x4+y4*y4 <= 1) {
            Point3D p(origin + t4*direction);
            Vector3D normal(0,0,1);
            if (swaped) normal = -1* normal; //rotation, so normal should be flipped
            ray.intersection.localPos = p;
            ray.intersection.t_value =t4;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            return true;
        }
    }
    
    return false;
}


//added to compute triangle intersection, to find mesh
//referenced online algorithm
bool UnitTriangle::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                             const Matrix4x4& modelToWorld) {
    //transform to object space
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    direction.normalize();
    double threshold = 0.00000001;
    //points
    Vector3D p1(-0.75983,-0.658035,0);//left
    Vector3D p2(0,0.658035,0);//up
    Vector3D p3(0.75983,-0.658035,0);//right
    Vector3D e1 = p2 - p1;
    Vector3D e2 = p3 - p1;

    Vector3D pvec = direction.cross(e2);
    double det = e1.dot(pvec);


    if(det < threshold && det > -threshold) return false;
    double invDet = 1/det;
    Vector3D ori_vec(origin[0],origin[1],origin[2]);
    // Vector3D ori_vec(0,0,0);
    Vector3D tvec = ori_vec - p1;
  //  tvec.normalize();
    double u = tvec.dot(pvec)*invDet;
    if(u < 0 || u > 1)return false;
    Vector3D qvec = tvec.cross(e1);
  //  qvec.normalize();
    double v = invDet*direction.dot(qvec);

   // std::cout <<v <<" "<<u <<std::endl;
    
    if(v< 0 || u+v > 1)return false;


    double t0 = e2.dot(qvec)*invDet;
    if(t0 < threshold) return false;
    Point3D p(origin + t0*direction);
    Vector3D normal(0,0,1);
    if(normal.dot(direction)>threshold)normal = Vector3D(0,0,-1);
   // std::cout << modelToWorld*p <<normal<<std::endl;
    normal.normalize();
    ray.intersection.localPos = p;
    ray.intersection.t_value =t0;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
    //std::cout <<"yeah"<<std::endl;
    return true;
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



