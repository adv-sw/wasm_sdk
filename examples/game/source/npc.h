// Non-player character class - very simple in this example.

#ifndef NPC_H
#define NPC_H

#include "actor.h"

#include <math.h>

const float pi = 3.141592653589793238462643383f;
const float two_pi = pi * 2;
   

class NPC : public Actor
{
public:

    NPC(SceneGraph *scene, Node *parent) : Actor(scene, parent) 
    {
       m_shields = 1.0f;
    }

    void Init()
    {
       SceneGraph *geom = m_object->GetGeometry();
       m_damage_mat = geom->FindMaterial("main");
       
       if (m_damage_mat)
          m_damage_mat->GetColour(m_damage_colour);
          
       m_damage_colour_increment = (White - m_damage_colour) * 0.1f; // arbitrary scale.
    }

   virtual void Move(float current_time)
   {
#if 0
      // up/down movement
      
      const float cycle_time = 4.0f;
   
      uint32_t num_cycles =  current_time / cycle_time;
      
      // time in cycle
      float t = current_time - (num_cycles * cycle_time);

      Vector3 p;
      
      float section_time = cycle_time / 2;
      
      const float vrange = 4.0f;
      const float v_offset = -2.0f;
      
      
      if (t > section_time)
         p = Vector3(5.0f,  v_offset + ((2 * section_time - t) * vrange), 0.0f);
      else
         p = Vector3(5.0f,  v_offset + (t * vrange), 0.0f);

#else   
      // Move in a circle
      float t = current_time;
      Vector3 p(5.0f + (float)sin(t) * 2.0f, (float)cos(t) * 2.0f, 0.0f);
#endif

      m_node->SetPosition(p);
    }
    

    void Process(float current_time, SceneGraph *live_geom)
    {
       if (m_state == Actor::State::Expiring)
       {
          if (current_time >= m_state_transition_time)
             m_state = Actor::State::Expired;
       }

       if (m_state == Actor::State::Expired)
       {
          // This simple example just regenerates the enemy after it expires.
          // TODO: trigger follow on action if any.
          m_state = Actor::State::Live;
          m_shields = 1.0f;
          m_object->SetGeometry(live_geom);
       }

       if (m_state == Actor::State::Live)
       {
           Move(current_time);
       }

       // TODO: Do this with a material override so multiple enemies can share the same geometry.
       if (m_damage_mat)
       {
          float fac = 1.0f - m_shields;;
          Vector3 effect(m_damage_colour_increment.x *fac, m_damage_colour_increment.y*fac, m_damage_colour_increment.z *fac); 
          Vector3 c(m_damage_colour + effect);
          m_damage_mat->SetColour(c);
       }
    }
    

    uint32_t Interact(Object *candidate, float current_time, SceneGraph *effect_geom)
    {
       if (m_state == Actor::State::Live)
       {
          if (candidate->Intersection(m_object))
          {
             m_shields -= 0.05f;

             if (m_shields <= 0.2f)
             {
                // Play effect 
                m_object->SetGeometry(effect_geom);
                m_object->SetAnimationPlaybackOffset(current_time);

                m_state = Actor::State::Expiring;
                m_state_transition_time = current_time + effect_geom->GetAnimation()->GetDuration();
                
                // Hardwired score for now, extend as required.
                return 10;
             }
          }
       } 

       return 0;
    }
    

    float    m_shields;
   
    Material *m_damage_mat;
    Vector3   m_damage_colour_increment;
    Vector3   m_damage_colour;
};

#endif // NPC_H
