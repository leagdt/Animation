#include "CharacterController.h"
#include "FiniteStateMachine.h"
#include <sys/types.h>
#include <unistd.h>

CharacterController::CharacterController() {
  m_v = 0;
  m_vMax = 0.5;
  noeudCourant = 0;
  m_h = 0;
}

void CharacterController::init(MotionGraph &motionGraph) {
  m_v = 0;
  m_h = 0;
  m_vMax = 0.5;
  mg = &motionGraph;
  noeudCourant = 0;

  // Création des transitions de la machine à état
  fsm.addTransition(Idle, Walk, Z,
                    std::bind(&CharacterController::marche, this));
  fsm.addTransition(Idle, Run, ZShift,
                    std::bind(&CharacterController::court, this));
  fsm.addTransition(Idle, SlowWalk, ZAlt,
                    std::bind(&CharacterController::marcheDoucement, this));
  fsm.addTransition(Idle, Kick, V,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Idle, Backflip, B,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Idle, Jump, Space,
                    std::bind(&CharacterController::saute, this));
  fsm.addTransition(Idle, Crouch, Ctrl,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Idle, CrouchWalk, CtrlZ,
                    std::bind(&CharacterController::marcheDoucement, this));
  fsm.addTransition(Crouch, CrouchWalk, CtrlZ,
                    std::bind(&CharacterController::marcheDoucement, this));
  fsm.addTransition(Walk, CrouchWalk, CtrlZ,
                    std::bind(&CharacterController::marcheDoucement, this));
  fsm.addTransition(Walk, Run, ZShift,
                    std::bind(&CharacterController::court, this));
  fsm.addTransition(Walk, Jump, Space,
                    std::bind(&CharacterController::saute, this));
  fsm.addTransition(Run, Jump, Space,
                    std::bind(&CharacterController::saute, this));
  fsm.addTransition(SlowWalk, Jump, Space,
                    std::bind(&CharacterController::saute, this));
  fsm.addTransition(Walk, Backflip, B,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Run, Backflip, B,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(SlowWalk, Backflip, B,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Walk, Kick, V, std::bind(&CharacterController::kick, this));
  fsm.addTransition(Run, Kick, V, std::bind(&CharacterController::kick, this));
  fsm.addTransition(SlowWalk, Kick, V,
                    std::bind(&CharacterController::kick, this));
  fsm.addTransition(Run, Idle, I, std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Walk, Idle, I,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(SlowWalk, Idle, I,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Crouch, Idle, I,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(CrouchWalk, Walk, Z,
                    std::bind(&CharacterController::marche, this));
  fsm.addTransition(CrouchWalk, Idle, I,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Kick, Idle, FinKick,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Jump, Idle, FinJump,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(Backflip, Idle, FinBackflip,
                    std::bind(&CharacterController::iddle, this));
  fsm.addTransition(CrouchWalk, Crouch, Ctrl,
                    std::bind(&CharacterController::iddle, this));
}

void CharacterController::updateMg(const float dt) {
  if (key_state('d')) {
    turnXZ(10);
  }
  if (key_state('q')) {
    turnXZ(-10);
  }

  if (key_state('z') && key_state(SDLK_LSHIFT)) {
    if (mg->transition(noeudCourant, 2) != -1) {
      court();
      noeudCourant = mg->transition(noeudCourant, 2);
    } else {
      std::cout << "PAS DE TRANSITION POSSIBLE" << std::endl;

    }
  } else if (key_state('z')) {
    if (mg->transition(noeudCourant, 1) != -1) {
      marche();
      noeudCourant = mg->transition(noeudCourant, 1);
    } else {
      std::cout << "PAS DE TRANSITION POSSIBLE" << std::endl;

    }
  } else {
    if (mg->transition(noeudCourant, 0) != -1) {
      iddle();
      noeudCourant = mg->transition(noeudCourant, 0);
    } else {
      std::cout << "PAS DE TRANSITION POSSIBLE" << std::endl;

    }
  }
  if (key_state('v')) {
    if (mg->transition(noeudCourant, 3) != -1) {
      m_v = 0;
      kick();
      noeudCourant = mg->transition(noeudCourant, 3);
    } else {
      std::cout << "PAS DE TRANSITION POSSIBLE" << std::endl;
    }
  }

  m_ch2w = m_ch2w * Translation(0, 0, m_v * dt);
}

void CharacterController::frameSuivante() {
  if (mg->getMotionGraph()[noeudCourant].id_bvh ==
      mg->getMotionGraph()[noeudCourant + 1].id_bvh) {
    
    noeudCourant++;
  } else {
    int i = 0;
    while (mg->getMotionGraph()[noeudCourant].id_bvh ==
           mg->getMotionGraph()[noeudCourant - i].id_bvh) {
      i++;
    }
    noeudCourant = noeudCourant - i + 1;
  }
}

void CharacterController::update(const float dt) {
  if (key_state('d')) {
    turnXZ(5);
  }
  if (key_state('q')) {
    turnXZ(-5);
  }

  // je cours
  if (key_state('z') && key_state(SDLK_LSHIFT)) {
    fsm.handleEvent(ZShift);
  }
  // Je marche doucement
  else if (key_state('z') && key_state(SDLK_LALT)) {
    fsm.handleEvent(ZAlt);
  }
  // Je marche accroupi
  else if (key_state('z') && key_state(SDLK_LCTRL)) {
    fsm.handleEvent(CtrlZ);
  }
  // Je marche
  else if (key_state('z')) {
    fsm.handleEvent(Z);
  }
  // je m'accroupis
  else if (key_state(SDLK_LCTRL)) {
    fsm.handleEvent(Ctrl);
  }
  // Je m'arrete
  else {
    fsm.handleEvent(I);
  }

  // je saute
  if (key_state(' ')) {
    fsm.handleEvent(Space);
  }
  // je fais un backflip
  if (key_state('b')) {
    fsm.handleEvent(B);
  }
  // je donne un kick
  if (key_state('v')) {
    fsm.handleEvent(V);
  }

  m_ch2w = m_ch2w * Translation(0, m_h, m_v * dt);
}

void CharacterController::turnXZ(const float &rot_angle_v) {
  m_ch2w = m_ch2w * RotationY(rot_angle_v);
}

void CharacterController::accelerate(const float &speed_inc) {
  if (m_v + speed_inc <= m_vMax && m_v + speed_inc >= -m_vMax)
    m_v += speed_inc;
}

void CharacterController::marche() { m_v = 0.06; }

void CharacterController::court() { m_v = 0.1; }

void CharacterController::marcheDoucement() { m_v = 0.03; }

void CharacterController::saute() {
  std::cout << "je saute" << std::endl;
  // m_h = 0.2;
}

void CharacterController::kick() {
  m_v = 0;
  // std::cout << "je donne un kick" << std::endl;
}

void CharacterController::iddle() {
  m_v = 0;
  m_h = 0;
}

int CharacterController::getFrameAction(float &boucleAnim, int nbIteration,
                                        ETATS action) {
  if (boucleAnim < nbIteration) {
    return 1;
  } else {
    if (action == Kick)
      fsm.handleEvent(FinKick);
    if (action == Jump)
      fsm.handleEvent(FinJump);
    if (action == Backflip)
      fsm.handleEvent(FinBackflip);
  }
  boucleAnim = 0;
  return -1;
}

void CharacterController::setVelocityMax(const float vmax) { m_vMax = vmax; }

void CharacterController::setEtats(const ETATS &e) { /*etatCourant = e;*/
}
const Point CharacterController::position() const {
  return m_ch2w(Point(0, 0, 0));
}

const Vector CharacterController::direction() const {
  return m_ch2w(Vector(1, 0, 0));
}

ETATS CharacterController::getCurrentEtat() const {
  return fsm.getCurrentState();
}

ETATS CharacterController::getPreviousEtat() const {
  return fsm.getPreviousState();
}

int CharacterController::getCurrentEtatMg() const {
  return mg->getMotionGraph()[noeudCourant].id_bvh;
}

float CharacterController::velocity() const { return m_v; }