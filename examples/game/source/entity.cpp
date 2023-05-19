#include "entity.h"

using namespace Infinity;

#include <infinity.h>
#include <math.h> 


// An Entity is a general purpose <><> Infinity scene graph connected object.

Entity::Entity(SceneGraph *scene, Node *parent)
{
   m_node   = scene->CreateNode(parent);
   m_object = scene->CreateObject(m_node);
}


Entity::~Entity() 
{
   // Remove the node (& object) from the scene graph hierarchy.
   m_node->Disconnect(true);
}
