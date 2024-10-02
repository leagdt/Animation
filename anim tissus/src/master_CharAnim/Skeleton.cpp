
#include "Skeleton.h"
#include "BVHAxis.h"
#include "BVHChannel.h"
#include "BVHJoint.h"
#include "TransformQ.h"
#include <cmath>
#include <cstddef>

using namespace chara;

void Skeleton::init(const BVH& bvh)
{
    // on initialise 
    for(int i=0; i<bvh.getNumberOfJoint(); i++){
        chara::BVHJoint parent = bvh.getJoint(i);
        SkeletonJoint s;
        s.m_parentId = parent.getParentId();
        m_joints.push_back(s);
    }
}


Point Skeleton::getJointPosition(int i) const
{
    return m_joints[i].m_l2w(Point(0, 0, 0));
}


int Skeleton::getParentId(const int i) const
{
    return m_joints[i].m_parentId;
}


void Skeleton::setPose(const BVH& bvh, int frameNumber, const Transform& m)
{
    // TODO
    // Parcourir toutes les articulations (SkeletonJoint ou BVHJoint) 
    //     Declarer la matrice l2f (pere<-local)
    //     Init avec la translation Sffset
    //     Parcourir tous les channels
    //          Accumuler dans la matrice l2f les translations et rotation en fonction du type de Channel
    // Multiplier la matrice l2f avec la matrice l2w (world<-local) du p�re qui est d�j� stock� dans le tableau 
    // Attention il peut ne pas y avoir de p�re (pour la racine)
    for(size_t i=0; i<m_joints.size(); i++){
        Transform l2f; 
        float x, y, z;
        BVHJoint noeudCourant = bvh.getJoint(i);
        noeudCourant.getOffset(x, y, z);
        l2f=Translation(x,y,z);

        for(int j=0; j<noeudCourant.getNumberOfChannel(); j++){

            BVHChannel& cha = noeudCourant.getChannel(j);
            if (cha.isRotation()){
                if (cha.getAxis() == chara::AXIS_X){
                    l2f= l2f*RotationX(cha.getData(frameNumber));
                }
                if (cha.getAxis() == chara::AXIS_Y){
                    l2f= l2f*RotationY(cha.getData(frameNumber));
                }
                if (cha.getAxis() == chara::AXIS_Z){
                    l2f= l2f*RotationZ(cha.getData(frameNumber));
                }
            }else if (cha.isTranslation()){
                if (cha.getAxis() == chara::AXIS_X){
                    l2f = l2f*Translation(cha.getData(frameNumber), 0, 0);
                }
                if (cha.getAxis() == chara::AXIS_Y){
                    l2f = l2f*Translation(0,cha.getData(frameNumber), 0);
                }
                if (cha.getAxis() == chara::AXIS_Z){
                    l2f = l2f*Translation(0, 0, cha.getData(frameNumber));
                }
            }
        }

        if(m_joints[i].m_parentId >= 0){
            BVHJoint parent = bvh.getJoint(m_joints[i].m_parentId);
            m_joints[i].m_l2w = m_joints[m_joints[i].m_parentId].m_l2w*l2f;
        }else {
            m_joints[i].m_l2w = m*l2f; 
        }
    }         
}

float distance(const Skeleton& a, const Skeleton& b){
    float distance = 0; 
    for(size_t i=0; i<a.m_joints.size(); i++){
        Point posA = a.getJointPosition(i);
        Point posB = b.getJointPosition(i); 
        distance += sqrt(pow(posA.x-posB.x, 2)+ pow(posA.y-posB.y, 2) + pow(posA.z-posB.z, 2));
    }
    
    return distance;
}

void Skeleton::setPoseInterpolation(const BVH& bvhSrc, int frameNbSrc, const BVH& bvhDst, int frameNbDst, float t, const Transform& m)
{
    for(size_t i = 0; i < m_joints.size(); i++)
    {
        TransformQ l2fSrc, l2fDst, l2fQ;

        float xSrc, ySrc, zSrc;
        float xDst, yDst, zDst;

        // Get source and destination joint data
        BVHJoint noeudSrc = bvhSrc.getJoint(i);
        BVHJoint noeudDst = bvhDst.getJoint(i);

        // Initialize with the offset (translation) of the joint
        noeudSrc.getOffset(xSrc, ySrc, zSrc);
        l2fSrc = TransformQ(vec3(), 0, vec3(xSrc, ySrc, zSrc));

        noeudDst.getOffset(xDst, yDst, zDst);
        l2fDst = TransformQ(vec3(), 0, vec3(xDst, yDst, zDst));

        for(int j=0; j<noeudSrc.getNumberOfChannel(); j++){

            BVHChannel& chaSrc = noeudSrc.getChannel(j);
            BVHChannel& chaDst = noeudDst.getChannel(j);
            if (chaSrc.isRotation() && chaDst.isRotation()){
                float angleSrc = chaSrc.getData(frameNbSrc); 
                float angleDst = chaDst.getData(frameNbDst);

                if (chaSrc.getAxis() == chara::AXIS_X){
                    l2fSrc= l2fSrc*TransformQ(vec3(1.0f, 0.0f, 0.0f), angleSrc);
                    l2fDst= l2fDst*TransformQ(vec3(1.0f, 0.0f, 0.0f), angleDst);
                }
                if (chaSrc.getAxis() == chara::AXIS_Y){
                    l2fSrc= l2fSrc*TransformQ(vec3(0.0f, 1.0f, 0.0f), angleSrc);
                    l2fDst= l2fDst*TransformQ(vec3(0.0f, 1.0f, 0.0f), angleDst);
                }
                if (chaSrc.getAxis() == chara::AXIS_Z){
                    l2fSrc= l2fSrc*TransformQ(vec3(0.0f, 0.0f, 1.0f), angleSrc);
                    l2fDst= l2fDst*TransformQ(vec3(0.0f, 0.0f, 1.0f), angleDst);
                }

            }else if (chaSrc.isTranslation() && chaDst.isTranslation()){
                float translationSrc = chaSrc.getData(frameNbSrc); 
                float translationDst = chaDst.getData(frameNbDst);
                
                if (chaSrc.getAxis() == chara::AXIS_X){
                    l2fSrc= l2fSrc*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(translationSrc, 0.0f, 0.0f));
                    l2fDst= l2fDst*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(translationDst, 0.0f, 0.0f));
                }
                if (chaSrc.getAxis() == chara::AXIS_Y){
                    l2fSrc= l2fSrc*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(0.0, translationSrc, 0.0f));
                    l2fDst= l2fDst*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(0.0, translationDst, 0.0f));
                }
                if (chaSrc.getAxis() == chara::AXIS_Z){
                    l2fSrc= l2fSrc*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(0.0, 0.0, translationSrc));
                    l2fDst= l2fDst*TransformQ(vec3(0.0f, 0.0f, 0.0f), 0, vec3(0.0, 0.0, translationDst));
                }
            }
        }

        l2fQ = TransformQ::slerp(l2fSrc, l2fDst, t);
        Transform l2f;
        l2fQ.getMatrix44(l2f.m);

        if(m_joints[i].m_parentId >= 0){
            m_joints[i].m_l2w = m_joints[m_joints[i].m_parentId].m_l2w*l2f;
        }else {
            m_joints[i].m_l2w = m*l2f; 
        }
    }
}