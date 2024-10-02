#include "MotionGraph.h"
#include "Skeleton.h"
#include <cstddef>


MotionGraph::MotionGraph(){ }

void MotionGraph::init(const std::vector<chara::BVH>& ensBvh){

    // Création de l'ensemble des noeuds 
    for(int k = 0; k<ensBvh.size(); k++){
        m_BVH.push_back(ensBvh[k]); 
        for(int i =0; i<ensBvh[k].getNumberOfFrame(); i++){
            std::vector<GrapheNodeID> idNoeudSuivant; 
            m_GrapheNode.push_back(GrapheNode{k, i, idNoeudSuivant}); 
        }
    }

    //Création des arêtes entre les noeuds 
    for(size_t i=0; i<m_GrapheNode.size(); i++){
        // je regarde les autres noeuds
        for (size_t v=0; v<m_GrapheNode.size(); v++) {
            if(i != v){
                BVH_ID idI = m_GrapheNode[i].id_bvh;
                BVH_ID idV = m_GrapheNode[v].id_bvh;

                int numFrameI = m_GrapheNode[i].frame;
                int numFrameV = m_GrapheNode[v].frame;
                Transform mat; 

                Skeleton skeI;
                skeI.init(m_BVH[idI]); 
                skeI.setPose(m_BVH[idI], numFrameI, mat);
                
                Skeleton skeV;
                skeV.init(m_BVH[idV]); 
                skeV.setPose(m_BVH[idV], numFrameV, mat);

                if (distance(skeI, skeV)< 150){
                    m_GrapheNode[i].ids_next.push_back(v); 
                   
                }
            }
        }
    }
}


//fct qui renvoie l'indice de la transition si elle est possible
int MotionGraph::transition(int indNoeudCourant, MotionGraph::BVH_ID idBvhFinal){
    int indice = indNoeudCourant; 
    previousIdBvh = m_GrapheNode[indNoeudCourant].id_bvh; 
    for(int indiceTransistion: m_GrapheNode[indNoeudCourant].ids_next){
        if(m_GrapheNode[indiceTransistion].id_bvh == idBvhFinal){
            return indiceTransistion;
        }
    }
    return -1;
}

int MotionGraph::getPreviousIdBvh(){
    return previousIdBvh;
}

void MotionGraph::parcoursAleatoire(){
    GrapheNode noeudCourant = m_GrapheNode[28]; 
    int steps = noeudCourant.ids_next.size(); 
    for(int i = 0; i<steps; i++){
        if (noeudCourant.ids_next.size()>0){
            int rdm = std::rand()%noeudCourant.ids_next.size(); 
            noeudCourant = m_GrapheNode[noeudCourant.ids_next[rdm]]; 
        }else {
            break;
        }
    }
}