#ifndef CREATURE_H
#define CREATURE_H

#include <iostream>
#include "Box2D/Box2D.h"
#include "PDController.h"
#include "Motion.h"

class Creature {

public:
	Creature (b2World* world);               // Constructeur
	virtual	~Creature();                     // Destructeur

	void update(bool balanceControl, bool motionTracking, bool& saut, float intensity);  // Mise a jour de la creature (controleur)
	bool hasFallen();					                    // Vrai si la creature est tombee

	b2Vec2 getCOM() {return m_positionCOM;}	            // Retourne la position du CdM
	void motionTrackerJump(bool& saut);

	void setIntensityFactorJump(const float& i){
		intensityFactorJump = i;
	}

protected:

	enum {PIEDDROIT, PIEDGAUCHE, MOLLETDROIT, MOLLETGAUCHE, CUISSEDROIT, CUISSEGAUCHE, TRONC, BRASDROIT, BRASGAUCHE, NB_CORPS};           // Segments de la creature
	enum {CHEVILLEDROITE, CHEVILLEGAUCHE, GENOUDROIT, GENOUGAUCHE, HANCHEDROITE, HANCHEGAUCHE, EPAULEDROITE, EPAULEGAUCHE, NB_ARTICULATIONS};    // Articulations de la creature

	b2World             *	m_world;		    		        // Le monde physique
	b2Body			    *	m_bodies[NB_CORPS];                 // Le tableau de corps rigides
	b2RevoluteJoint     *	m_joints[NB_ARTICULATIONS];		    // Le tableau d'articulations
	float                   m_motorTarget[NB_ARTICULATIONS];    // Le tableau de moments articulaires a appliquer
	PDController		*	m_PDControllers[NB_ARTICULATIONS];	// Le tableau de regulateurs PD (un par articulation)

	bool	    m_hasFallen;            // Vrai si la creature est tombee
	bool        m_isTracking;           // Vrai si le suivi est active
	bool        m_isJump;               // Vrai si le saut est actif
	Motion  *   m_motion;               // Le mouvement a suivre
	Motion  *   m_motionJump;
	b2Vec2      m_positionCOM;          // La position du CdM
	float intensityFactorJump = 0; 

	b2Vec2 computeCenterOfMass();                               // Calcule la position du CdM de la creature dans le repere du monde
	void balanceController();                                   // Calcule et met a jour les moments articulaires pour equilibrer la creature
	void motionTracker();                                       // Calcule et met a jour les moments articulaires pour suivre le mouvement de reference 
	float jacobianTranspose(b2Vec3,b2Vec3,b2Vec3,int,b2Vec3);   // Calcule les moments necessaires a simuler les effets de la force (en 3D)
	void applyJumpForces(b2Vec2 force);

};

#endif
