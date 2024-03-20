// --------------------------------------------------------------------------------------
//
//    <><> Infinity/XSG secure programming environment : vertex manipulation example.
//
// --------------------------------------------------------------------------------------


#include <infinity.h>
#include <math.h> 

using namespace Infinity;

#include <inf_bootstrap.h>


class App : public Program
{
public:

   App() : Program()
   {
      m_item_node = nullptr;
      m_target = nullptr;
   }
   
   
   virtual uint32_t Init()
   {
      auto scene = m_zone->GetScene();
      auto scene_root = scene->GetRoot();
      
      m_item_node = scene->CreateNode(scene_root);
      
      auto item_object = scene->CreateObject(m_item_node);

      auto item_geometry = m_zone->Find("item.xsg");
      item_object->SetGeometry(item_geometry);
      
      auto n = item_geometry->FindNode("item");
      
      m_target = (Mesh*) n->GetAttachment();
      
      
      // Clean up
      delete scene;
      delete scene_root;
      delete item_object;
      delete item_geometry;

      return 1; // Success.
   }
   

   virtual uint32_t Process(float current_time)
   {
      // Move the object ...
   
      if (m_target)
      {
         const float magnitude = 3.0f;
         float speed = 5.0;  
         float angle = speed * current_time;
         
         for (int i=2; i<4; i++)
         {
            Vector3 p;
            m_target->GetVertexPosition(i, p);
            p.y = fabs(sin(angle)) * magnitude;
            m_target->SetVertexPosition(i, p);
         }
      }   
    
      return 1;
   }
   
   
   virtual uint32_t Terminate()
   {      
      // Clean up.
      delete m_item_node;
     
      return 1; // Success.
   }
   
   
   Node *m_item_node;
   Mesh *m_target;
};


// Application entry point.
extern "C" Program *Program_Create(uint32_t init_params)
{
   return new App;
}
