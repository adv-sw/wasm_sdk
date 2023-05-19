#ifndef ORDNANCE_H
#define ORDNANCE_H


#include "inf_entity.h"


class Ordnance : public Game_Entity
{
public:
   Ordnance(SceneGraph *scene, Node *parent, float current_time) : Game_Entity(scene, parent) 
   {
      m_creation_time = current_time;
   }
   
   bool Expired(float current_time) { return (current_time-m_creation_time) > 1; }

   float m_creation_time;
};


#endif // ORDNANCE_H