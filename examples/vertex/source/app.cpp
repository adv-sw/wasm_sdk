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
      
      m_last_change_time = -1.0f;
      
      // Clean up
      delete scene;
      delete scene_root;
      delete item_object;
      delete item_geometry;

      return 1; // Success.
   }
   

   virtual uint32_t Process(float current_time)
   {
      // Move the vertex ...
   
      if (m_target)
      {
         const float magnitude = 5.0f;
         float speed = 10.0;  
         float angle = speed * current_time;
         
         int i = m_current_vertex;
         //for (int i=; i<4; i++)
         {
            Vector3 p;
            m_target->GetVertexPosition(i, p);
            p.y = fabs(sin(angle)) * magnitude;
            m_target->SetVertexPosition(i, p);
         }
      }
      
      const float change_period = 1.0f;
      
      if (m_last_change_time < 0)
         m_last_change_time = current_time;
      else if ( (current_time - m_last_change_time) > change_period)
      {
         m_last_change_time = current_time;
         Vector3 p;
         m_target->GetVertexPosition(m_current_vertex, p);
         p.y = 0.0f;
         m_target->SetVertexPosition(m_current_vertex, p);
         
         m_current_vertex++;
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
   
   uint32_t m_current_vertex;
   float m_last_change_time;
};


// Application entry point.
extern "C" Program *Program_Create(uint32_t init_params)
{
   return new App;
}
