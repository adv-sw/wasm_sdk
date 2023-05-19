#ifndef ACTOR_H
#define ACTOR_H

#include "ordnance.h"

#include <vector>


const Vector3 White(1,1,1);
 

class Actor  : public Game_Entity
{
public:

   enum class State { Live, Expiring, Expired };
   
   Actor(SceneGraph *scene, Node *parent) : Game_Entity(scene, parent) 
   {
       m_state = State::Live;
   }
   
    virtual void Reset(SceneGraph default_geom) {}
    virtual void Move(float time) {}
 
    State           m_state;
    float           m_state_transition_time;

    // TODO: move this to game control class.  Don't need to control our own ordnance.
    std::vector<Ordnance*>  m_ordnance;
};
 
 #endif // ACTOR_H