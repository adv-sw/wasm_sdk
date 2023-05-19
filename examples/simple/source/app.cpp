// --------------------------------------------------------------------------------------
//
//    <><> Infinity/XSG secure programming environment : minimal example.
//
//                Hello world in 3D - a moving cube.
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
   }
   
   
   virtual uint32_t Init()
   {
      auto scene = m_zone->GetScene();
      auto scene_root = scene->GetRoot();
      
      m_item_node = scene->CreateNode(scene_root);
      
      auto item_object = scene->CreateObject(m_item_node);

      auto item_geometry = m_zone->Find("item.xsg");
      item_object->SetGeometry(item_geometry);
      
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
   
      if (m_item_node)
      {
         const float radius = 10;
         float speed = 1;  
         float angle = speed * current_time;
         
         Vector3 v;
         v.x = (float) sin(angle);
         v.y = (float) cos(angle);
         v.z = 10;
         
         m_item_node->SetPosition(v);
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
};


// Application entry point.
extern "C" Program *Program_Create(uint32_t init_params)
{
   return new App;
}
