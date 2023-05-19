#ifndef ENTITY_H
#define ENTITY_H

#include <infinity.h>

// A Game_Entity is a general purpose <><> Infinity scene graph connected object.

class Game_Entity
{
public:

   Game_Entity(Infinity::SceneGraph *scene, Infinity::Node *parent);
   ~Game_Entity();

   Infinity::Object *m_object;
   Infinity::Node   *m_node;
};


#endif // ENTITY_H