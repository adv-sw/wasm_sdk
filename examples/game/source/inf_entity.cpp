#include "inf_entity.h"

#include <infinity.h>
#include <math.h>

using namespace Infinity;


// A Game_Entity is a general purpose <><> Infinity scene graph connected object.

Game_Entity::Game_Entity(Infinity::SceneGraph *scene, Infinity::Node *parent)
{
   m_node   = scene->CreateNode(parent);
   m_object = scene->CreateObject(m_node);
}


Game_Entity::~Game_Entity() 
{
   // Remove the node (& object) from the scene graph hierarchy.
   m_node->Disconnect(true);
}

