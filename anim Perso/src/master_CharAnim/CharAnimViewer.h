
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "FiniteStateMachine.h"
#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"
#include "CharacterController.h"

#include <PhysicalWorld.h>

class CharAnimViewer : public Viewer
{
public:
    CharAnimViewer();
    CharAnimViewer(bool mg);

    int init();
    int render();
    int update( const float time, const float delta );
    void collision(); 

	static CharAnimViewer& singleton() { return *psingleton;  }

protected:
	void bvhDrawRec(const chara::BVHJoint& bvh, const Transform& f2w, int f);

    std::map<ETATS, chara::BVH> mapBVH;

    std::vector<chara::BVH> tabBVH; 
    
    int m_frameNumber_action;

    Skeleton m_ske;

    ETATS etat_precedent;
    int numFramePrecedente; 

    float timeAnim;

    PhysicalWorld m_world;

	void draw_skeleton(const Skeleton& );

private:
	static CharAnimViewer* psingleton;
    CharacterController control; 
    MotionGraph mg; 
    float timeState =0;
    
    int updateMg( const float time, const float delta );
    int updateFsm( const float time, const float delta );

    bool motionGraph = false; 
};



#endif
