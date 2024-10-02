
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <regex>
#include <stdexcept>
#include <utility>

#include "CharAnimViewer.h"
#include "BVHJoint.h"
#include "CharacterController.h"
#include "FiniteStateMachine.h"
#include "MotionGraph.h"
#include "Skeleton.h"

using namespace std;
using namespace chara;


CharAnimViewer* CharAnimViewer::psingleton = NULL;

CharAnimViewer::CharAnimViewer(bool mg) : Viewer()
{
	psingleton = this;
	motionGraph = mg; 
}

CharAnimViewer::CharAnimViewer() : Viewer()
{
	psingleton = this;
}


int CharAnimViewer::init()
{
    Viewer::init();
    cout<<"==>master_CharAnim/CharAnimViewer"<<endl;
    m_camera.lookat( Point(0,0,0), 1000 );
	m_camera.rotation(180, 0);
    gl.light( Point(300, 300, 300 ) );

    //b_draw_grid = false;

    m_world.setParticlesCount( 10 );


    init_cylinder();
    init_sphere();

	mapBVH.insert(std::make_pair(Idle, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_smoke_idle.bvh")))); 
	mapBVH.insert(std::make_pair(SlowWalk, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_slowwalk.bvh")))); 
	mapBVH.insert(std::make_pair(Walk, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_walk.bvh")))); 
	mapBVH.insert(std::make_pair(Run, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_run.bvh")))); 
	mapBVH.insert(std::make_pair(Kick, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_kick_roundhouse_R.bvh")))); 
	mapBVH.insert(std::make_pair(Crouch, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_crouch.bvh"))));  
	mapBVH.insert(std::make_pair(CrouchWalk, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_crouchwalk.bvh"))));  
	mapBVH.insert(std::make_pair(Backflip, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_backflip.bvh"))));  
	mapBVH.insert(std::make_pair(Jump, chara::BVH(smart_path("data/bvh/motionGraph_second_life/avatar_jumpforjoy.bvh")))); 
	
	if (motionGraph){
		tabBVH.push_back(mapBVH[Idle]);
		tabBVH.push_back(mapBVH[Walk]);
		tabBVH.push_back(mapBVH[Run]);
		tabBVH.push_back(mapBVH[Kick]);
		
		mg.init(tabBVH);
	}

	control.init(mg);

	m_frameNumber_action = 0; 
    /*cout<<endl<<"========================"<<endl;
    cout<<"BVH decription"<<endl;
    cout<<m_bvh<<endl;
    cout<<endl<<"========================"<<endl;*/

    m_ske.init(mapBVH[Idle]);
    m_ske.setPose( mapBVH[Idle], -1, control.controller2world());// met le skeleton a la pose au repos
	etat_precedent = Idle; 
	timeAnim = 0; 
    return 0;
}



void CharAnimViewer::draw_skeleton(const Skeleton& s)
{
	for(int i=0; i<s.numberOfJoint(); i++){
		//if(s.getParentId(i)>=0){
			draw_sphere(s.getJointPosition(i), 3);
		//}
	}
	for(int i=0; i<s.numberOfJoint(); i++){
		if(s.getParentId(i)>=0){
			draw_cylinder(s.getJointPosition(i), s.getJointPosition(s.getParentId(i)), 3);
		}
	}
    // TODO

	//const int N = 4;
	//static float angleA[N] = { 0, 10, 20, 10 };
	//static float angleB[N] = { 30, -10, -20, -25 };
	//static float t = 0;



	//int et = int(t);
	//float q = t - et;
	//float a = q * angleA[(et+1)%N] + (1 - q) * angleA[(et) % N];
	//Transform  a2w = Translation(10, 10, 0) * RotationZ(  a );
	//draw_sphere(   a2w*Scale( 2,2,2)   );
	//draw_cylinder(a2w * RotationZ(-90) * Scale(1,10,1) );

	//float b = q * angleB[(et + 1) % N] + (1 - q) * angleB[(et) % N];
	//Transform b2a = Translation(10, 0, 0) * RotationZ(b);
	//draw_sphere(  a2w*b2a * Scale(2, 2, 2));
	//draw_cylinder( a2w * b2a * RotationZ(-90) * Scale(1, 10, 1));

	//Transform c2b = Translation(10, 0, 0);
	//draw_sphere(a2w * b2a * c2b * Scale(2, 2, 2));

	//t += 0.01;
	//if (t >= N) t = 0.f;
	//sleep(100);

	//draw_cylinder( );



	//Transform a2w = Translation(10, 10, 0) ;
	//draw_sphere( a2w*Scale(2,2,2));
	//draw_cylinder(a2w * RotationZ(-90)*Translation(0, 1, 0)*Scale(1,10,1) );

	//Transform b2a = Translation(10, 0, 0);
	//Transform b2w = a2w * b2a;
	//draw_sphere( b2w * Scale(2, 2, 2));
	//draw_cylinder(b2w * RotationZ(-90) * Translation(0, 1, 0) * Scale(1, 10, 1));


}



int CharAnimViewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw_quad( RotationX(-90)*Scale(500,500,1) );

    Viewer::manageCameraLight();
    gl.camera(m_camera);

	// Affiche les particules physiques (Question 3 : interaction personnage sphere/ballon)
    m_world.draw();


	// Affiche le skeleton � partir de la structure lin�aire (tableau) Skeleton
    draw_skeleton(m_ske);


	Transform A2W = RotationZ(0)  ;
	draw_cylinder(A2W * Scale(1, 10, 1));

	draw_sphere(control.position(), 5); 

#if 0			// exercice du cours
	float A[] = { 0, -10, -20, -30, -40 };
	float B[] = { 0, -10, -20, -30, -40 };
	float C[] = { 0, -10, -20, -30, -40 };
	float D[] = { 0, 10, 20, 30, 40 };
	float a, b, c, d;

	static int t = 3;
	if (t == 4) t = 0; else t++;

	a = A[t];
	b = B[t];
	c = C[t];
	d = D[t];

	Transform scaleS = Scale(12, 12, 12);
	Transform scaleA = Scale(20, 20, 20);
	Transform scale = Scale(10, 100, 10);
	Transform A2W = RotationZ(a);
	draw_cylinder(A2W*scale);
	draw_sphere(A2W*scaleS);
	draw_axe(A2W*scaleA);

	Transform B2A = Translation(0, 100, 0) * RotationZ(b);
	Transform B2W = A2W * B2A;
	draw_cylinder(B2W*scale);
	draw_sphere(B2W*scaleS);
	draw_axe(B2W*scaleA);

	Transform scaleP = Scale(5, 50, 5);
	Transform C2B = Translation(0, 100, 0) * RotationZ(c);
	Transform C2W = B2W * C2B;
	draw_cylinder(C2W*scaleP);
	draw_sphere(C2W*scaleS);
	draw_axe(C2W*scaleA);

	Transform D2B = Translation(0, 100, 0) * RotationZ(d);
	Transform D2W = B2W * D2B;
	draw_cylinder(D2W*scaleP);
	draw_axe(D2W*scaleA);
	draw_sphere(D2W*scaleS);
#endif


    return 1;
}

void CharAnimViewer::collision(){
	for(int i=0; i<m_ske.numberOfJoint(); i++){
		m_world.collision(m_ske.getJointPosition(i), 3);
	}
}

int CharAnimViewer::update( const float time, const float delta )
{
    // time est le temps ecoule depuis le demarrage de l'application, en millisecondes,
    // delta est le temps ecoule depuis l'affichage de la derniere image / le dernier appel a draw(), en millisecondes.
/*
	if (key_state('n')) { m_frameNumber++; cout << m_frameNumber << endl; }
	if (key_state('b')) { m_frameNumber--; cout << m_frameNumber << endl; }*/


//////////////////////////////////////////////////////////////

	
	if(motionGraph)
		return updateMg(time, delta); 

	return updateFsm(time, delta);
}

int CharAnimViewer::updateFsm( const float time, const float delta )
{

	float boucleAnim;

	if (control.getCurrentEtat() != control.getPreviousEtat()){
		timeAnim = time; 
		m_frameNumber_action = 0;
		boucleAnim = 0; 
	}

	// CALCUL DE T POUT L'INTERPOLATION
	float x = mapBVH[control.getCurrentEtat()].getFrameTime()*1000;
	float d = std::fmod((time - timeAnim), x*mapBVH[control.getCurrentEtat()].getNumberOfFrame());
	float t = std::fmod(d/x, 1.0f); 

	int numFrame = floor(d/x); 
	int numFrameSup = ceil(d/x);

	// nombre de fois que l'anim à bouclé 
	boucleAnim = (time - timeAnim)/(x*mapBVH[control.getCurrentEtat()].getNumberOfFrame());
	
	if (control.getCurrentEtat() == Kick || control.getCurrentEtat() == Jump || control.getCurrentEtat() == Backflip){

		if(control.getFrameAction(boucleAnim,1, control.getCurrentEtat()) != -1){
			
			m_ske.setPoseInterpolation(mapBVH[control.getCurrentEtat()], numFrame%mapBVH[control.getCurrentEtat()].getNumberOfFrame(), mapBVH[control.getCurrentEtat()], numFrameSup%mapBVH[control.getCurrentEtat()].getNumberOfFrame(), t, control.controller2world());
		}

	} else{
		
		m_ske.setPoseInterpolation(mapBVH[control.getCurrentEtat()], numFrame%mapBVH[control.getCurrentEtat()].getNumberOfFrame(), mapBVH[control.getCurrentEtat()], numFrameSup%mapBVH[control.getCurrentEtat()].getNumberOfFrame(), t, control.controller2world());
			
	}
	control.update(delta); 
    m_world.update(0.1f);
	collision(); 
	m_world.draw();
	numFramePrecedente = numFrame; 
    return 0;
}

int CharAnimViewer::updateMg( const float time, const float delta )
{

	float boucleAnim;

	if (control.getCurrentEtatMg() != mg.getPreviousIdBvh()){
		timeAnim = time; 
		m_frameNumber_action = 0;
		boucleAnim = 0; 
	}

	// CALCUL DE T POUT L'INTERPOLATION
	float x = mapBVH[control.getCurrentEtat()].getFrameTime()*1000;
	float d = std::fmod((time - timeAnim), x*mapBVH[control.getCurrentEtat()].getNumberOfFrame());
	float t = std::fmod(d/x, 1.0f); 

	int numFrame = floor(d/x); 
	int numFrameSup = ceil(d/x);

	// nombre de fois que l'anim à bouclé 
	boucleAnim = (time - timeAnim)/(x*mapBVH[control.getCurrentEtat()].getNumberOfFrame());

	if(numFramePrecedente != numFrame){
		control.frameSuivante(); 
	}

	m_ske.setPoseInterpolation(tabBVH[control.getCurrentEtatMg()], numFrame%tabBVH[control.getCurrentEtatMg()].getNumberOfFrame(), tabBVH[control.getCurrentEtatMg()], numFrameSup%tabBVH[control.getCurrentEtatMg()].getNumberOfFrame(), t, control.controller2world());
	
	control.updateMg(delta);
	
    m_world.update(0.1f);
	collision(); 
	m_world.draw();
	numFramePrecedente = numFrame; 
    return 0;
}