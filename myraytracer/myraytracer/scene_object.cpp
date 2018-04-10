/***********************************************************
 
 Starter code for Assignment 3
 
 Implements scene_object.h
 
 ***********************************************************/
#include <cmath>
#include "scene_object.h"
#include <math.h>
#define PI 3.14159265
using namespace std;
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
    
    //added by us, to optimize structure
    __sync_fetch_and_add(&numRaySquareTests, 1);
    
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

//compute intersection of a cylinder
bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel,const Matrix4x4& modelToWorld){
    //transform the ray(origin,direction) to object space
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
   // direction.normalize();
    double threshold =0.00000001;
    bool return_true = false; //yes, if equals to true, return true
    //referenced algorithm,solve for wall intersection
    
    double z_max = 1;
    double z_min = -1;
    if(direction[2] == 0){
        return false;
    }
    //check if hit close or far cap
    //calculate dot(light,normal) to decide normal direction
    //t3, is where the light hit the far cap
    //t4, is where the light hit the near cap
    double t3 = (z_min - origin[2])/direction[2];
    double t4 = (z_max - origin[2])/direction[2];
   
    if (!ray.intersection.none&&t3>ray.intersection.t_value){
        t3 = -1;
    }
    if(t3>threshold){
        double x3 = origin[0] + t3*direction[0];
        double y3 = origin[1] + t3*direction[1];
        if (x3*x3+y3*y3 <= 1) {
            Point3D p(origin + t3*direction);
            Vector3D normal(0,0,-1);
            //check normal direction
            if(normal.dot(direction) > 0){
                normal = -normal;
            }
            ray.intersection.t_value =t3;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.localPos = p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            ray.intersection.none = false;
            return_true = true;

        }
    }
    // the near cap
    if (!ray.intersection.none&&t4>ray.intersection.t_value){
        t4 = -1;
    }
    if(t4> threshold ){
        double x4 = origin[0] + t4*direction[0];
        double y4 = origin[1] + t4*direction[1];
        if (x4*x4+y4*y4 <= 1) {
            Point3D p(origin + t4*direction);
            Vector3D normal(0,0,1);
            //check normal direction
            if(direction.dot(normal) > 0)
                normal = -normal;
            ray.intersection.localPos = p;
            ray.intersection.t_value =t4;
            ray.intersection.point = modelToWorld*p;
            ray.intersection.normal = transNorm(worldToModel, normal);
            ray.intersection.none = false;
            return_true = true;
        }
    }
    
    //now calculate the wall intersection
    double t0,t1;
    double a = direction[0]*direction[0] + direction[1]*direction[1];
    double b = 2*origin[0]*direction[0]+2*origin[1]*direction[1];
    double c = origin[0]*origin[0] + origin[1]*origin[1]- 1;
    double delta = b*b - 4*a*c;
    //if no solution,return false
    //if(delta < threshold)return false;
    //find two root
    if(delta < 0){
        //no need to continue
        return return_true;
    }
    t0 = (-b - sqrt(delta))/(2*a);
    t1 = (-b + sqrt(delta))/(2*a);
    Point3D  p0(origin + t0*direction);
    Point3D  p1(origin + t1*direction);
    //within the depth of cylinder
    bool t0_valid = p0[2]<z_max && p0[2]>z_min;
    bool t1_valid = p1[2]<z_max && p1[2]>z_min;
    if(t0< threshold && t1<threshold) return return_true;
    else if(t0 > threshold && t1 > threshold){
        if(!t0_valid && !t1_valid)return return_true;//no valid
        //only consider both valid case
        else if(t0_valid && t1_valid){
            double t_small = fmin(t0,t1);
            //if no intersection before
            if(ray.intersection.none || t_small < ray.intersection.t_value){
                Point3D p_small(origin + t_small*direction);
                Vector3D normal(p_small[0], p_small[1], 0);
                if(direction.dot(normal)>0)normal = -normal;
                ray.intersection.localPos = p_small;
                ray.intersection.t_value = t_small;
                ray.intersection.point = modelToWorld*p_small;
                ray.intersection.normal = transNorm(worldToModel, normal);
                 ray.intersection.none = false;
                return_true = true;
            }
        }else{
            double t_small = t0_valid ? t0 : t1;
            //if no intersection before
            if(ray.intersection.none || t_small < ray.intersection.t_value){
                Point3D p_small(origin + t_small*direction);
                Vector3D normal(p_small[0], p_small[1], 0);
                if(direction.dot(normal)>0)normal = -normal;
                ray.intersection.localPos = p_small;
                ray.intersection.t_value = t_small;
                ray.intersection.point = modelToWorld*p_small;
                ray.intersection.normal = transNorm(worldToModel, normal);
                 ray.intersection.none = false;
                return_true = true;
            }
        }
    }
   else if (t0_valid && t0 > threshold){
            if(ray.intersection.none || t0 < ray.intersection.t_value){
                Point3D p_small(origin + t0*direction);
                Vector3D normal(p_small[0], p_small[1], 0);
                if(direction.dot(normal)>0)normal = -normal;
                ray.intersection.localPos = p_small;
                ray.intersection.t_value = t0;
                ray.intersection.point = modelToWorld*p_small;
                ray.intersection.normal = transNorm(worldToModel, normal);
                 ray.intersection.none = false;
                return_true = true;
            }
    }
   else if (t1_valid && t1 > threshold){
            if(ray.intersection.none || t1 < ray.intersection.t_value){
                Point3D p_small(origin + t1*direction);
                Vector3D normal(p_small[0], p_small[1], 0);
                if(direction.dot(normal)>0)normal = -normal;
                ray.intersection.localPos = p_small;
                ray.intersection.t_value = t1;
                ray.intersection.point = modelToWorld*p_small;
                ray.intersection.normal = transNorm(worldToModel, normal);
                 ray.intersection.none = false;
                return_true = true;
        }
    }
    return return_true;
}


//added to make a cube and do texture mapping
//referenced https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
bool UnitCube::intersect(Ray3D& ray, const Matrix4x4& worldToModel,const Matrix4x4& modelToWorld){
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    int side = -1;//record is x,y,z plane being hit first,0x,1y,2z
   
    double min_x = -1.0; double max_x = 1.0;
    double min_y = -1.0; double max_y = 1.0;
    double min_z = -1.0;double max_z = 1.0;
    
    
    double t_min = (min_x - origin[0])/direction [0];
    side = 0;
    double t_max = (max_x - origin[0])/direction [0];
  
    if(t_min > t_max) swap(t_min, t_max);
    double tymin = (min_y-origin[1])/direction [1];
    double tymax = (max_y-origin[1])/direction [1];

    if(t_min > t_max) swap(tymin,tymax);
    if((t_min>tymax)|| (tymin > t_max))return false;
    
    if(tymin>t_min){
        t_min=tymin;
        side = 1;
    }
    if(tymax < t_max){
        t_max = tymax;
    }
   
    double tzmin = (min_z - origin[2]) / direction[2];
    double tzmax = (max_z - origin[2]) / direction[2];
    
    if (tzmin > tzmax) swap(tzmin, tzmax);
    
    if ((t_min > tzmax) || (tzmin > t_max))
        return false;
    
    if (tzmin > t_min){
        t_min = tzmin;
        side = 2;
    }
    if (tzmax < t_max)
        t_max = tzmax;
    
    Point3D p(origin + t_min*direction);
    Vector3D normal;
    if(side == 0){//hit side
        normal = Vector3D(1,0,0);
        if(direction.dot(normal)>0)normal = -normal;
    }else if(side == 1){
        normal = Vector3D(0,1,0);
         if(direction.dot(normal)>0)normal = -normal;
    }else if(side==2){
        normal = Vector3D(0,0,1);
        if(direction.dot(normal)>0)normal = -normal;
    }
    ray.intersection.localPos = p;
    ray.intersection.t_value = t_min;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
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

bool Triangle::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
                    const Matrix4x4& modelToWorld){
    Point3D origin = worldToModel * ray.origin;
    Vector3D direction = worldToModel * ray.dir;
    double t=-origin[2]/direction[2];
    if(t<0.0000000001||direction[2] == 0||(!ray.intersection.none&&t>ray.intersection.t_value)){
        return false;
    }
    
    
    
    double u=origin[0]+t*direction[0];
    double v=origin[1]+t*direction[1];
    Vector3D normal(0,0,1);
    Point3D p(u,v,0);
    if(u>0&&v>0&&(u+v)<=1){
        ray.intersection.localPos = Point3D(u,v,0);
        ray.intersection.t_value =t;
        ray.intersection.point = modelToWorld*p;
        ray.intersection.normal = transNorm(worldToModel,normal);
        ray.intersection.normal.normalize();
        return true;
    }else{
        return false;
    }
    
    
    
}

Triangle::Triangle(Point3D origin,Point3D iu, Point3D iv){
    this->o=origin;
    this->u=iu-origin;
    this->v=iv-origin;
}

SceneNode::SceneNode(Triangle* obj, Material* mat){
    this->obj=obj;
    this->mat=mat;
    this->ismesh=true;
	this->firstTouch = true;
    Point3D o=obj->o;
    Vector3D u=obj->u;
    Vector3D v=obj->v;
    Vector3D normal=u.cross(v);
    this->normal=normal;
    this->normal.normalize();
    Matrix4x4 P;
    Matrix4x4 inv;
    P.m_data[0]=u[0];P.m_data[1]=v[0];P.m_data[2]=normal[0];P.m_data[3]=o[0];
    P.m_data[4]=u[1];P.m_data[5]=v[1];P.m_data[6]=normal[1];P.m_data[7]=o[1];
    P.m_data[8]=u[2];P.m_data[9]=v[2];P.m_data[10]=normal[2];P.m_data[11]=o[2];
    P.m_data[12]=0;P.m_data[13]=0;P.m_data[14]=0;P.m_data[15]=1;
    this->modelToWorld=P;
    
    
    inv.m_data[0] = P.m_data[5]  * P.m_data[10] * P.m_data[15] -
    P.m_data[5]  * P.m_data[11] * P.m_data[14] -
    P.m_data[9]  * P.m_data[6]  * P.m_data[15] +
    P.m_data[9]  * P.m_data[7]  * P.m_data[14] +
    P.m_data[13] * P.m_data[6]  * P.m_data[11] -
    P.m_data[13] * P.m_data[7]  * P.m_data[10];
    
    inv.m_data[4] = -P.m_data[4]  * P.m_data[10] * P.m_data[15] +
    P.m_data[4]  * P.m_data[11] * P.m_data[14] +
    P.m_data[8]  * P.m_data[6]  * P.m_data[15] -
    P.m_data[8]  * P.m_data[7]  * P.m_data[14] -
    P.m_data[12] * P.m_data[6]  * P.m_data[11] +
    P.m_data[12] * P.m_data[7]  * P.m_data[10];
    
    inv.m_data[8] = P.m_data[4]  * P.m_data[9] * P.m_data[15] -
    P.m_data[4]  * P.m_data[11] * P.m_data[13] -
    P.m_data[8]  * P.m_data[5] * P.m_data[15] +
    P.m_data[8]  * P.m_data[7] * P.m_data[13] +
    P.m_data[12] * P.m_data[5] * P.m_data[11] -
    P.m_data[12] * P.m_data[7] * P.m_data[9];
    
    inv.m_data[12] = -P.m_data[4]  * P.m_data[9] * P.m_data[14] +
    P.m_data[4]  * P.m_data[10] * P.m_data[13] +
    P.m_data[8]  * P.m_data[5] * P.m_data[14] -
    P.m_data[8]  * P.m_data[6] * P.m_data[13] -
    P.m_data[12] * P.m_data[5] * P.m_data[10] +
    P.m_data[12] * P.m_data[6] * P.m_data[9];
    
    inv.m_data[1] = -P.m_data[1]  * P.m_data[10] * P.m_data[15] +
    P.m_data[1]  * P.m_data[11] * P.m_data[14] +
    P.m_data[9]  * P.m_data[2] * P.m_data[15] -
    P.m_data[9]  * P.m_data[3] * P.m_data[14] -
    P.m_data[13] * P.m_data[2] * P.m_data[11] +
    P.m_data[13] * P.m_data[3] * P.m_data[10];
    
    inv.m_data[5] = P.m_data[0]  * P.m_data[10] * P.m_data[15] -
    P.m_data[0]  * P.m_data[11] * P.m_data[14] -
    P.m_data[8]  * P.m_data[2] * P.m_data[15] +
    P.m_data[8]  * P.m_data[3] * P.m_data[14] +
    P.m_data[12] * P.m_data[2] * P.m_data[11] -
    P.m_data[12] * P.m_data[3] * P.m_data[10];
    
    inv.m_data[9] = -P.m_data[0]  * P.m_data[9] * P.m_data[15] +
    P.m_data[0]  * P.m_data[11] * P.m_data[13] +
    P.m_data[8]  * P.m_data[1] * P.m_data[15] -
    P.m_data[8]  * P.m_data[3] * P.m_data[13] -
    P.m_data[12] * P.m_data[1] * P.m_data[11] +
    P.m_data[12] * P.m_data[3] * P.m_data[9];
    
    inv.m_data[13] = P.m_data[0]  * P.m_data[9] * P.m_data[14] -
    P.m_data[0]  * P.m_data[10] * P.m_data[13] -
    P.m_data[8]  * P.m_data[1] * P.m_data[14] +
    P.m_data[8]  * P.m_data[2] * P.m_data[13] +
    P.m_data[12] * P.m_data[1] * P.m_data[10] -
    P.m_data[12] * P.m_data[2] * P.m_data[9];
    
    inv.m_data[2] = P.m_data[1]  * P.m_data[6] * P.m_data[15] -
    P.m_data[1]  * P.m_data[7] * P.m_data[14] -
    P.m_data[5]  * P.m_data[2] * P.m_data[15] +
    P.m_data[5]  * P.m_data[3] * P.m_data[14] +
    P.m_data[13] * P.m_data[2] * P.m_data[7] -
    P.m_data[13] * P.m_data[3] * P.m_data[6];
    
    inv.m_data[6] = -P.m_data[0]  * P.m_data[6] * P.m_data[15] +
    P.m_data[0]  * P.m_data[7] * P.m_data[14] +
    P.m_data[4]  * P.m_data[2] * P.m_data[15] -
    P.m_data[4]  * P.m_data[3] * P.m_data[14] -
    P.m_data[12] * P.m_data[2] * P.m_data[7] +
    P.m_data[12] * P.m_data[3] * P.m_data[6];
    
    inv.m_data[10] = P.m_data[0]  * P.m_data[5] * P.m_data[15] -
    P.m_data[0]  * P.m_data[7] * P.m_data[13] -
    P.m_data[4]  * P.m_data[1] * P.m_data[15] +
    P.m_data[4]  * P.m_data[3] * P.m_data[13] +
    P.m_data[12] * P.m_data[1] * P.m_data[7] -
    P.m_data[12] * P.m_data[3] * P.m_data[5];
    
    inv.m_data[14] = -P.m_data[0]  * P.m_data[5] * P.m_data[14] +
    P.m_data[0]  * P.m_data[6] * P.m_data[13] +
    P.m_data[4]  * P.m_data[1] * P.m_data[14] -
    P.m_data[4]  * P.m_data[2] * P.m_data[13] -
    P.m_data[12] * P.m_data[1] * P.m_data[6] +
    P.m_data[12] * P.m_data[2] * P.m_data[5];
    
    inv.m_data[3] = -P.m_data[1] * P.m_data[6] * P.m_data[11] +
    P.m_data[1] * P.m_data[7] * P.m_data[10] +
    P.m_data[5] * P.m_data[2] * P.m_data[11] -
    P.m_data[5] * P.m_data[3] * P.m_data[10] -
    P.m_data[9] * P.m_data[2] * P.m_data[7] +
    P.m_data[9] * P.m_data[3] * P.m_data[6];
    
    inv.m_data[7] = P.m_data[0] * P.m_data[6] * P.m_data[11] -
    P.m_data[0] * P.m_data[7] * P.m_data[10] -
    P.m_data[4] * P.m_data[2] * P.m_data[11] +
    P.m_data[4] * P.m_data[3] * P.m_data[10] +
    P.m_data[8] * P.m_data[2] * P.m_data[7] -
    P.m_data[8] * P.m_data[3] * P.m_data[6];
    
    inv.m_data[11] = -P.m_data[0] * P.m_data[5] * P.m_data[11] +
    P.m_data[0] * P.m_data[7] * P.m_data[9] +
    P.m_data[4] * P.m_data[1] * P.m_data[11] -
    P.m_data[4] * P.m_data[3] * P.m_data[9] -
    P.m_data[8] * P.m_data[1] * P.m_data[7] +
    P.m_data[8] * P.m_data[3] * P.m_data[5];
    
    inv.m_data[15] = P.m_data[0] * P.m_data[5] * P.m_data[10] -
    P.m_data[0] * P.m_data[6] * P.m_data[9] -
    P.m_data[4] * P.m_data[1] * P.m_data[10] +
    P.m_data[4] * P.m_data[2] * P.m_data[9] +
    P.m_data[8] * P.m_data[1] * P.m_data[6] -
    P.m_data[8] * P.m_data[2] * P.m_data[5];
    
    double det = P.m_data[0] * inv.m_data[0] + P.m_data[1] * inv.m_data[4] + P.m_data[2] * inv.m_data[8] + P.m_data[3] * inv.m_data[12];
    
    if (det == 0)
        std::cout<<"det < 0"<<std::endl;
    
    det = 1.0 / det;
    
    for (int i = 0; i < 16; i++)
        inv.m_data[i] = inv.m_data[i] * det;
    

    this->worldToModel=inv;
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
    
    
    ray.intersection.localPos = Point3D(u,v,0);
    ray.intersection.t_value =t0;
    ray.intersection.point = modelToWorld*p;
    ray.intersection.normal = transNorm(worldToModel, normal);
    ray.intersection.normal.normalize();
    return true;
}

