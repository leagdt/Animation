/*
 * SolveurExpl.cpp : Application schemas semi-implicite sur les objets.
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

/** \file Calculs.cpp
 Fonctions de calculs communes aux objets simules.
 \brief Fonctions de calculs communes aux objets simules.
 */

#include <cmath>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "vec.h"
#include "ObjetSimule.h"
#include "Viewer.h"
#include "SolveurExpl.h"


using namespace std;



/**
 * Calcul de l acceleration des particules
 * avec ajout de la gravite aux forces des particules
 * et ajout de la force due au vent sur une des particules du maillage
 * et reinitialisation des forces.
 */
void SolveurExpl::CalculAccel_ForceGravite(Vector g,
                                           int nb_som,
                                           std::vector<Vector> &A,
                                           std::vector<Vector> &Force,
                                           std::vector<float> &M, 
                                           int Tps)
{
    for(int i=0; i<nb_som; i++){
        Force[i] = Force[i] + Vector(0,0, sin((Tps*180/M_PI)/1000)*10) ; 
        if (M[i] != 0)
		    A[i] = Force[i]/M[i] + g; 
        Force[i] =Vector(0,0,0);
	}
 			//// Cas SPH
            // On a calcule dans Force[i] : fij / rho_i
            // Il ne reste qu a ajoute le vecteur g
            // a_i = fij / rho_i + g
    
}//void


/*! Calcul des vitesses et positions : 
 *  Formule d Euler semi-implicite :
 *  x'(t+dt) = x'(t) + dt x"(t)
 *  x(t+dt) = x(t) + dt x'(t+dt)
 */
void SolveurExpl::Solve(float visco,
                        int nb_som,
                        int Tps,
                        std::vector<Vector> &A,
                        std::vector<Vector> &V,
                        std::vector<Vector> &P)
{
    for(int i=0; i<nb_som; i++){
		V[i] = (V[i]+_delta_t*A[i])*visco;
        P[i] = P[i]+_delta_t*V[i];
	}

       
}//void

