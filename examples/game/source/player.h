#ifndef PLAYER_H
#define PLAYER_H


#include "npc.h"


class Player : public Actor
{
public:

   Player(SceneGraph *scene, Node *parent) : Actor(scene, parent) 
   {
      m_score = 0;
      m_prev_score = 0;
      m_lives = 3;
   }

   virtual void Reset(SceneGraph *default_geom)
   {
      Vector3 p(-10,0,0);
      m_node->SetPosition(p);
      m_object->SetGeometry(default_geom);
   }

   bool Interact(NPC *candidate, float current_time, SceneGraph *effect_geom)
   {
      // Only live objects currently interact - damage fragments are just eye candy.
      if ( (m_state != Actor::State::Live) || (candidate->m_state != Actor::State::Live) )
         return false;
      
      bool hit = m_object->Intersection(candidate->m_object);

      if (hit)
      {
     
         // Switch player geometry to player explosion geometry, play explosion animation.
         m_object->SetGeometry(effect_geom);
    
         m_object->SetAnimationPlaybackOffset(current_time);
       
         m_state = Actor::State::Expiring;
        
         m_state_transition_time = current_time + effect_geom->GetAnimation()->GetDuration();
           
         // Player loses a life.
         m_lives -= 1;

         return m_lives == 0;
      }

      return false;
   }
   
    uint32_t m_score;
    uint32_t m_prev_score;
    uint32_t m_lives;
};

#endif // PLAYER_H

