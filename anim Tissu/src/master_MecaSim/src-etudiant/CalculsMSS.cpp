/*
 * CalculsMSS.cpp :
 * Copyright (C) 2016 Florence Zara, LIRIS
 *               florence.zara@liris.univ-lyon1.fr
 *               http://liris.cnrs.fr/florence.zara/
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file CalculsMSS.cpp
Programme calculant pour chaque particule i d un MSS son etat au pas de temps suivant 
 (methode d 'Euler semi-implicite) : principales fonctions de calculs.
\brief Fonctions de calculs de la methode semi-implicite sur un systeme masses-ressorts.
*/ 

#include <stdexcept>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "MSS.h"
#include "vec.h"
#include "ObjetSimule.h"
#include "ObjetSimuleMSS.h"
#include "Viewer.h"

using namespace std;





/**
* Calcul des forces appliquees sur les particules du systeme masses-ressorts.
 */
void ObjetSimuleMSS::CalculForceSpring()
{
	for(int j=0; j<_SystemeMasseRessort->GetNbRessort(); j++){
		Ressort* r = _SystemeMasseRessort->_RessortList[j];
		if (r != nullptr){
			int a = r->GetParticuleA()->_Id;
			int b = r->GetParticuleB()->_Id;
			Vector ab = P[b] - P[a];
			
			Force[a] = Force[a]+ (r->GetRaideur()*(length(ab) - r->GetLrepos())*ab/length(ab));
			Force[b] = Force[b]- (r->GetRaideur()*(length(ab) - r->GetLrepos())*ab/length(ab));
			
			Vector vab = V[b] - V[a]; 
			Force[a] = Force[a] + (dot(vab, ab/length(ab))* ab/length(ab));
			Force[b] = Force[b] - (dot(vab, ab/length(ab))* ab/length(ab));
		}
	}
	
	/// Rq : Les forces dues a la gravite et au vent sont ajoutees lors du calcul de l acceleration
    
		
}//voidvec3


/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleMSS::Collision()
{
    /// Arret de la vitesse quand touche le plan
   for(int j=0; j<_SystemeMasseRessort->GetNbParticule(); j++){
		
		Particule* p = _SystemeMasseRessort->_ParticuleList[j];
		if(P[p->_Id].y <= -10){
			V[p->_Id] = 0; 
		}
	}
    
}

/**
 * Gestion des déchirures du tissus.
 */
void ObjetSimuleMSS::Dechirures()
{
	for(int j=0; j<_SystemeMasseRessort->GetNbRessort(); j++){
		Ressort* r = _SystemeMasseRessort->_RessortList[j];
		if (r != nullptr){
			int a = r->GetParticuleA()->_Id;
			int b = r->GetParticuleB()->_Id;
			
			if (length(P[b]-P[a]) > 0.9){
				_SystemeMasseRessort->_RessortList[j] = nullptr; 
			}
		}
	}
}

void ObjetSimuleMSS::CollisionSphere(float rayon, float centreX, float centreY, float centreZ)
{
    for(int j = 0; j < _SystemeMasseRessort->GetNbParticule(); j++) {
        
        Particule* p = _SystemeMasseRessort->_ParticuleList[j];
        Vector cp; 
        cp.x = P[p->_Id].x - centreX; 
        cp.y = P[p->_Id].y - centreY;
        cp.z = P[p->_Id].z - centreZ;
        
        // Vérification si la particule est à l'intérieur de la sphère
        if(length(cp) < rayon) {
            
            // Mise à zéro de la vitesse et de l'accélération
            V[p->_Id] = Vector(0, 0, 0); 
            A[p->_Id] = Vector(0, 0, 0);
            
            // Déplacer la particule à la surface de la sphère
            Vector normalisee_cp = cp / length(cp);
            P[p->_Id] = Vector(centreX, centreY, centreZ) + normalisee_cp * rayon;
        }
    }
}

void ObjetSimuleMSS::Chute()
{
	for (int i=0; i<M.size(); i++){
		if(M[i] == 0){
			M[i] = 1; 
		}
	}
}

