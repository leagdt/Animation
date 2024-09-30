#include "Creature.h"
#include "Box2D/Common/b2Math.h"
#include "Motion.h"
#include <iostream>
#include <ostream>

#ifdef _WIN32
#include <windows.h>
#else
DWORD GetTickCount() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (DWORD)(ts.tv_nsec / 1000000 + ts.tv_sec * 1000);
}
#endif

using namespace std;

Creature::Creature(b2World *world)
    : m_world(world), m_hasFallen(false), m_isTracking(false) { // Constructeur

  // Creation des corps rigides
  // ==========================

  // Proprietes communes
  b2BodyDef bodyDef;
  bodyDef.fixedRotation = false;
  bodyDef.allowSleep = false;
  bodyDef.awake = true;
  bodyDef.type = b2_dynamicBody;
  bodyDef.linearDamping = 0.01f;
  bodyDef.angularDamping = 0.01f;
  b2PolygonShape shape;
  b2FixtureDef fixture;
  fixture.shape = &shape;

  // PIED Droit
  bodyDef.position.Set(0.7f, 1.22f);
  m_bodies[PIEDDROIT] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.3f, 0.1f);
  fixture.density = 7.5f;
  fixture.friction = 0.92;
  m_bodies[PIEDDROIT]->CreateFixture(&fixture);

  // PIED Gauche
  bodyDef.position.Set(-0.7f, 1.22f);
  m_bodies[PIEDGAUCHE] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.3f, 0.1f);
  fixture.density = 7.5f;
  m_bodies[PIEDGAUCHE]->CreateFixture(&fixture);

  // Mollet Gauche
  bodyDef.position.Set(-0.55f, 1.8f);
  bodyDef.angle = 0;
  m_bodies[MOLLETGAUCHE] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.5f);
  fixture.density = 3.0f;
  m_bodies[MOLLETGAUCHE]->CreateFixture(&fixture);

  // Mollet DROIT
  bodyDef.position.Set(0.55f, 1.8f);
  m_bodies[MOLLETDROIT] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.5f);
  fixture.density = 3.0f;
  m_bodies[MOLLETDROIT]->CreateFixture(&fixture);

  // CUISSE Gauche
  bodyDef.position.Set(-0.35f, 2.7f);
  bodyDef.angle = -0.4;
  m_bodies[CUISSEGAUCHE] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.5f);
  fixture.density = 3.0f;
  m_bodies[CUISSEGAUCHE]->CreateFixture(&fixture);

  // CUISSE Droite
  bodyDef.position.Set(0.35f, 2.7f);
  bodyDef.angle = 0.4;
  m_bodies[CUISSEDROIT] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.5f);
  fixture.density = 3.0f;
  m_bodies[CUISSEDROIT]->CreateFixture(&fixture);

  // TRONC
  bodyDef.position.Set(0.0f, 3.86f);
  bodyDef.angle = 0;
  m_bodies[TRONC] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.2f, 0.9f);
  fixture.density = 3.0f;
  m_bodies[TRONC]->CreateFixture(&fixture);

  // BRASDROIT
  bodyDef.position.Set(0.5f, 3.9f);
  bodyDef.angle = 0.4;
  m_bodies[BRASDROIT] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.9f);
  fixture.density = 2.0f;
  m_bodies[BRASDROIT]->CreateFixture(&fixture);

  // BRASGAUCHE
  bodyDef.position.Set(-0.5f, 3.9f);
  bodyDef.angle = -0.4;
  m_bodies[BRASGAUCHE] = m_world->CreateBody(&bodyDef);
  shape.SetAsBox(0.1f, 0.9f);
  fixture.density = 2.0f;
  m_bodies[BRASGAUCHE]->CreateFixture(&fixture);

  // Creation des articulations
  // ==========================

  // Proprietes communes
  b2RevoluteJointDef jointDef;
  jointDef.lowerAngle = -0.5f * b2_pi;
  jointDef.upperAngle = 0.5f * b2_pi;
  jointDef.enableLimit = true;
  jointDef.enableMotor = true;
  jointDef.maxMotorTorque = 100.0f;

  // CHEVILLE Droite
  jointDef.Initialize(m_bodies[PIEDDROIT], m_bodies[MOLLETDROIT],
                      m_bodies[PIEDDROIT]->GetWorldCenter() +
                          b2Vec2(-0.3f, 0.1f));
  m_joints[CHEVILLEDROITE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // CHEVILLE Gauche
  jointDef.Initialize(m_bodies[PIEDGAUCHE], m_bodies[MOLLETGAUCHE],
                      m_bodies[PIEDGAUCHE]->GetWorldCenter() +
                          b2Vec2(0.3f, 0.1f));
  m_joints[CHEVILLEGAUCHE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // GENOU Droite
  jointDef.Initialize(m_bodies[MOLLETDROIT], m_bodies[CUISSEDROIT],
                      m_bodies[MOLLETDROIT]->GetWorldCenter() +
                          b2Vec2(0.0f, 0.5f));
  m_joints[GENOUDROIT] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // GENOU Gauche
  jointDef.Initialize(m_bodies[MOLLETGAUCHE], m_bodies[CUISSEGAUCHE],
                      m_bodies[MOLLETGAUCHE]->GetWorldCenter() +
                          b2Vec2(0.0f, 0.5f));
  m_joints[GENOUGAUCHE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // HANCHE Droite
  jointDef.Initialize(m_bodies[CUISSEDROIT], m_bodies[TRONC],
                      m_bodies[CUISSEDROIT]->GetWorldCenter() +
                          b2Vec2(-0.2f, 0.5f));
  m_joints[HANCHEDROITE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // HANCHE Gauche
  jointDef.Initialize(m_bodies[CUISSEGAUCHE], m_bodies[TRONC],
                      m_bodies[CUISSEGAUCHE]->GetWorldCenter() +
                          b2Vec2(0.2f, 0.5f));
  m_joints[HANCHEGAUCHE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // EPAULE Droite
  jointDef.Initialize(m_bodies[BRASDROIT], m_bodies[TRONC],
                      m_bodies[BRASDROIT]->GetWorldCenter() +
                          b2Vec2(-0.3f, 0.9f));
  m_joints[EPAULEDROITE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // EPAULE Gauche
  jointDef.Initialize(m_bodies[BRASGAUCHE], m_bodies[TRONC],
                      m_bodies[BRASGAUCHE]->GetWorldCenter() +
                          b2Vec2(0.3f, 0.9f));
  m_joints[EPAULEGAUCHE] = (b2RevoluteJoint *)m_world->CreateJoint(&jointDef);

  // Controleur
  // ==========
  m_PDControllers[CHEVILLEDROITE] = new PDController(0.9, 1.89);
  m_PDControllers[HANCHEDROITE] = new PDController(1.0, 2.0);
  m_PDControllers[CHEVILLEGAUCHE] = new PDController(0.9, 1.89);
  m_PDControllers[HANCHEGAUCHE] = new PDController(1.0, 2.0);
  m_PDControllers[EPAULEDROITE] = new PDController(1.0, 2.0);
  m_PDControllers[EPAULEGAUCHE] = new PDController(1.0, 2.0);
  m_PDControllers[GENOUDROIT] = new PDController(0.9, 1.89);
  m_PDControllers[GENOUGAUCHE] = new PDController(0.9, 1.89);

  // Creation du mouvement de reference a suivre
  m_motion = new Motion("motion.txt");
  m_motionJump = new Motion("motion2.txt");
}

Creature::~Creature() { // Destructor
  for (int i = 0; i < NB_ARTICULATIONS; ++i) {
    delete m_PDControllers[i];
    m_PDControllers[i] = NULL;
  }
  delete m_motion;
  delete m_motionJump;
}

void Creature::update(bool balanceControl, bool motionTracking, bool &saut, float intensity) {
  // Remise a zero des moments articulaires
  for (int j = 0; j < NB_ARTICULATIONS; ++j)
    m_motorTarget[j] = 0.0;
  // Calcule et met a jour les moments articulaires necessaires a equilibrer la
  // creature
  if (balanceControl)
    balanceController();

  // Calcule et met a jour les moments articulaires pour suivre le mouvement de
  // reference
  if (motionTracking)
    motionTracker();
  else
    m_isTracking = false;

  if (saut) {
    motionTrackerJump(saut);
  } else {
    intensityFactorJump = 0; 
    m_isJump = false;
  }

  // Applique les moments articulaires en tant que "motor speed"
  for (int j = 0; j < NB_ARTICULATIONS; ++j)
    m_joints[j]->SetMotorSpeed(m_motorTarget[j]);
}

void Creature::balanceController() {
  // BALANCE CONTROLLER
  // ==================

  // Calcul du CdM dans le repere du monde
  m_positionCOM = computeCenterOfMass();

  // Mettre a jour la pose seulement si la creature est debout
  if (m_hasFallen)
    return;

  // Etape 1.1: Decrire le CPS dans le repere du monde
  // definir la position du CPS localement a un corps rigide bien choisi
  // utiliser b2Body::GetTransform pour optenir la transformation decrivant
  // position+orientation du corps rigide dans le monde utiliser la
  // transformation pour convertir la position locale du CPS en sa position
  // globale
  b2Vec2 cpsMonde;

  if (m_bodies[PIEDDROIT]->GetContactList() &&
      m_bodies[PIEDGAUCHE]->GetContactList()) {
    cpsMonde =
        0.5 *
        (b2Mul(m_bodies[PIEDDROIT]->GetTransform(), b2Vec2(0.0f, -0.1f)) +
         b2Mul(m_bodies[PIEDGAUCHE]->GetTransform(), b2Vec2(0.0f, -0.1f)));

  } else if (m_bodies[PIEDDROIT]->GetContactList() &&
             !m_bodies[PIEDGAUCHE]->GetContactList()) {
    cpsMonde = b2Mul(m_bodies[PIEDDROIT]->GetTransform(), b2Vec2(0.0f, -0.1f));
  } else if (!m_bodies[PIEDDROIT]->GetContactList() &&
             m_bodies[PIEDGAUCHE]->GetContactList()) {
    cpsMonde = b2Mul(m_bodies[PIEDGAUCHE]->GetTransform(), b2Vec2(0.0f, -0.1f));
  } else {
    return;
  }
  b2Vec3 forceToApply(cpsMonde.x - m_positionCOM.x, 0.0f, 0.0f);

  // Etape 1.2: Calculer la force d'equilibre, en 3D, a appliquer a partir de la
  // distance entre les CPS et CdM projetes au sol

  // Etape 2.1: decrire la position, en 3D, de la cheville dans le repere du
  // pied
  b2Vec3 positionOfAxisLocal(0.0f, 0.1f, 0.0f);
  // Etape 2.2: decrire l'axe de rotation, en 3D, de la cheville dans le repere
  // du pied : z
  b2Vec3 axisOfRotationLocal(0.0f, 0.0f, 1.0f);
  // Etape 2.3: appeller jacobianTranspose afin d'estimer le moment necessaire a
  // la cheville pour simuler l'effet de la force
  float jointTorque = jacobianTranspose(
      positionOfAxisLocal, axisOfRotationLocal,
      b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f), PIEDDROIT, forceToApply);
  // Etape 2.4: mettre a l'echelle l'erreur en moment articulaire
  jointTorque *= 1.0f;
  // Etape 2.5: ajouter le resultat dans le tableau m_motorTarget
  m_motorTarget[CHEVILLEDROITE] += jointTorque;

  // Etape 3.1 to 3.5: faire de meme pour la hanche
  positionOfAxisLocal = b2Vec3(0.0f, 1.0f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque =
      jacobianTranspose(positionOfAxisLocal, axisOfRotationLocal,
                        b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f),
                        MOLLETDROIT, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[GENOUDROIT] += jointTorque;

  // Etape 3.1 to 3.5: faire de meme pour la hanche
  positionOfAxisLocal = b2Vec3(0.0f, 1.0f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque =
      jacobianTranspose(positionOfAxisLocal, axisOfRotationLocal,
                        b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f),
                        CUISSEDROIT, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[HANCHEDROITE] += jointTorque;

  positionOfAxisLocal = b2Vec3(0.0f, 0.9f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque = jacobianTranspose(
      positionOfAxisLocal, axisOfRotationLocal,
      b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f), TRONC, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[EPAULEDROITE] += jointTorque;

  positionOfAxisLocal = b2Vec3(0.0f, 0.1f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque = jacobianTranspose(
      positionOfAxisLocal, axisOfRotationLocal,
      b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f), PIEDGAUCHE, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[CHEVILLEGAUCHE] += jointTorque;

  // Etape 3.1 to 3.5: faire de meme pour la hanche
  positionOfAxisLocal = b2Vec3(0.0f, 1.0f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque =
      jacobianTranspose(positionOfAxisLocal, axisOfRotationLocal,
                        b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f),
                        MOLLETGAUCHE, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[GENOUGAUCHE] += jointTorque;

  positionOfAxisLocal = b2Vec3(0.0f, 1.0f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque =
      jacobianTranspose(positionOfAxisLocal, axisOfRotationLocal,
                        b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f),
                        CUISSEGAUCHE, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[HANCHEGAUCHE] += jointTorque;

  positionOfAxisLocal = b2Vec3(0.0f, 0.9f, 0.0f);
  axisOfRotationLocal = b2Vec3(0.0f, 0.0f, 1.0f);
  jointTorque = jacobianTranspose(
      positionOfAxisLocal, axisOfRotationLocal,
      b2Vec3(m_positionCOM.x, m_positionCOM.y, 0.0f), TRONC, forceToApply);
  jointTorque *= 1.0f;
  m_motorTarget[EPAULEGAUCHE] += jointTorque;
}

void Creature::applyJumpForces(b2Vec2 force) {
  // Force à appliquer pour intensifier le saut (ajustez les valeurs selon vos
  // besoins)

  // Appliquer la force aux pieds droit et gauche
  m_bodies[PIEDDROIT]->ApplyForceToCenter(force, true);
  m_bodies[PIEDGAUCHE]->ApplyForceToCenter(force, true);
}

void Creature::motionTracker() {

  // Activation du suivi
  if (!m_isTracking) {
    m_isTracking = true;
    m_motion->setStartTime(GetTickCount());
  }
  // Calculer le temps depuis le debut du suivi (en ms)
  DWORD elapsedTime = GetTickCount() - m_motion->getStartTime();
  // Calculer l'indice dans le mouvement
  unsigned int frameIndex =
      (unsigned int)(elapsedTime / (100 * m_motion->getFrequency())) %
      m_motion->getNbFrames();
  // Recuperer les donnees correspondant a l'indice
  std::vector<float> frameData = m_motion->getMotionDataAtFrame(frameIndex);
  // Calculer le moment pour chaque articulation
  for (int j = 0; j < NB_ARTICULATIONS; ++j) {
    // Lire l'angle pour l'articulation j
    float targetAngle = frameData[j];
    // Affecter l'angle cible pour le regulateur PD
    m_PDControllers[j]->setTarget(targetAngle);
    // Lire l'angle actuel
    float32 currentAngle = m_joints[j]->GetJointAngle();
    // Calculer le moment (appel a PDController::compute)
    float trackingMotor = m_PDControllers[j]->compute(currentAngle);
    // Ajouter le resultat dans le tableau m_motorTarget
    m_motorTarget[j] += trackingMotor;
  }
}

void Creature::motionTrackerJump(bool& saut) {

  // Activation du suivi
  if (!m_isJump) {
    m_isJump = true;
    m_motionJump->setStartTime(GetTickCount());
  }
  // Calculer le temps depuis le debut du suivi (en ms)
  DWORD elapsedTime = GetTickCount() - m_motionJump->getStartTime();
  // Calculer l'indice dans le mouvement
  unsigned int frameIndex =
      (unsigned int)(elapsedTime / (100 * m_motionJump->getFrequency())) %
      m_motionJump->getNbFrames();

  if (frameIndex == m_motionJump->getNbFrames()-1){
    intensityFactorJump = 0; 
    saut = false; 
  }

  if(frameIndex >= 170 && frameIndex <= 173 && intensityFactorJump == 0 && intensityFactorJump != 170){
    intensityFactorJump = 170; 
  }

  // Recuperer les donnees correspondant a l'indice
  std::vector<float> frameData = m_motionJump->getMotionDataAtFrame(frameIndex);

  b2Vec2 force(0.0f, 0.0f);

  if (frameIndex >= 193 && frameIndex <= 213) {
    force = b2Vec2(0.0f, intensityFactorJump);
  }

  applyJumpForces(force);

  // Calculer le moment pour chaque articulation
  for (int j = 0; j < 6; ++j) {
    // Lire l'angle pour l'articulation j
    float targetAngle = frameData[j];
    // Affecter l'angle cible pour le regulateur PD
    m_PDControllers[j]->setTarget(targetAngle);
    // Lire l'angle actuel
    float32 currentAngle = m_joints[j]->GetJointAngle();
    // Calculer le moment (appel a PDController::compute)
    float trackingMotor = m_PDControllers[j]->compute(currentAngle);
    // Ajouter le resultat dans le tableau m_motorTarget
    m_motorTarget[j] += trackingMotor;
  }
}

bool Creature::hasFallen() {
  if (m_hasFallen)
    return m_hasFallen; // vrai si deja a terre
  if (m_bodies[TRONC]->GetWorldCenter().y < 1.22) {
    // detection que la creature est tombee (le CdM du tronc est au niveau de la
    // plateforme)
    for (int j = 0; j < NB_ARTICULATIONS; ++j) {
      m_joints[j]->SetMotorSpeed(0.0f);
      m_joints[j]->EnableMotor(false);
    }
    m_hasFallen = true;
  }
  return m_hasFallen;
}

b2Vec2 Creature::computeCenterOfMass() {
  float32 total_mass = 0.0f;
  b2Vec2 sum_COMs(0.0f, 0.0f);
  for (int i = 0; i < NB_CORPS; ++i) {
    float32 massBody = m_bodies[i]->GetMass();
    b2Vec2 comBody = m_bodies[i]->GetWorldCenter();
    sum_COMs += massBody * comBody;
    total_mass += massBody;
  }
  return (1.0f / total_mass) * sum_COMs;
}

float Creature::jacobianTranspose(b2Vec3 positionOfAxisLocal,
                                  b2Vec3 axisOfRotationLocal,
                                  b2Vec3 positionApplyForce, int ID_RIGIDBODY,
                                  b2Vec3 forceToApply) {
  // Convertir la position locale de l'articulation en position dans le monde
  b2Vec2 positionOfAxisLocal2D(positionOfAxisLocal.x, positionOfAxisLocal.y);
  b2Vec2 positionOfAxisInWorld2D =
      b2Mul(m_bodies[ID_RIGIDBODY]->GetTransform(), positionOfAxisLocal2D);
  b2Vec3 positionOfAxisInWorld(positionOfAxisInWorld2D.x,
                               positionOfAxisInWorld2D.y, 0.0f);
  // Convertir l'orientation de l'axe de rotation locale dans le monde (rien a
  // faire en 2D)
  b2Vec3 axisOfRotationWorld = axisOfRotationLocal;
  // Calculer le moment articulaire le long de l'axe de rotation (formule de la
  // transposee de Jacobienne)
  float jointTorque =
      b2Dot(b2Cross(axisOfRotationWorld,
                    (positionApplyForce - positionOfAxisInWorld)),
            forceToApply);
  // Retourner le moment articulaire
  return jointTorque;
}
