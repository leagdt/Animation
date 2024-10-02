#ifndef __CHARACTERCONTROLLER_H__
#define __CHARACTERCONTROLLER_H__

#include "FiniteStateMachine.h"
#include "mat.h"
#include <SDL2/SDL.h>
#include "window.h"
#include <ctime>
#include <unistd.h>
#include "MotionGraph.h"

class CharacterController {
public:
  CharacterController(); 

  void init(MotionGraph& motionGraph); 

  void update(const float dt);
  void updateMg(const float dt);

  void turnXZ(const float &rot_angle_v);
  void accelerate(const float &speed_inc);
  void setVelocityMax(const float vmax);
  void setEtats(const ETATS& e); 
  void marche();
  void marcheDoucement();
  void court();
  void kick();
  void iddle();
  void saute();
  void atterri();

  int getFrameAction(float& boucleAnim, int nbIteration, ETATS action); 


  int getCurrentNode(){ return  noeudCourant;}
  const Point position() const;
  const Vector direction() const;
  float velocity() const;
  const Transform &controller2world() const { return m_ch2w; }
  ETATS getCurrentEtat() const; 
  ETATS getPreviousEtat() const; 

  int getCurrentEtatMg() const; 

  void frameSuivante(); 

protected:
  Transform m_ch2w; // matrice du character vers le monde
                    // le personnage se déplace vers X
                    // il tourne autour de Y
                    // Z est sa direction droite

  float m_v;    // le vecteur vitesse est m_v * m_ch2w * Vector(1,0,0)
  float m_h; 
  float m_vMax; // ne peut pas accélérer plus que m_vMax
  FiniteStateMachine fsm; 
  int noeudCourant; 
  MotionGraph* mg; 
  
};
#endif