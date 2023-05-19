// --------------------------------------------------------------------------------------
//
//    <><> Infinity/XSG secure programming environment : Particle system example.
//
//                           A simple particle system
//
// --------------------------------------------------------------------------------------


#include <infinity.h>
#include <math.h> 
#include <vector> 


using namespace Infinity;

#include <inf_bootstrap.h>

typedef std::pair<std::shared_ptr<Node>, float> Particle;


class App : public Program
{
public:

   App() : Program()
   {
      m_last_time = 0.0f;
   }
   
   
   virtual uint32_t Init()
   {
      Emit(0.0f);
      
      return 1; // Success.
   }



   virtual uint32_t Process(float current_time)
   {
      // Death ...
      Expire(current_time, false);
      
      // Simulate live particles ...
      for (auto& particle : m_particles) 
         Simulate(particle, current_time);
      
      // Birth ...
      const auto emit_period = 0.5f;
     
      if (current_time - m_last_time > emit_period)
      {
         Emit(current_time);
         m_last_time = current_time;
      }
 
      return 1;
   }
   
   
   virtual uint32_t Terminate()
   {      
      Log("App::Terminate");
            
      Expire(0.0f, true);
      
      return 1; // Success.
   }
   
   
private:

   void Expire(float current_time, bool force)
   {
      const auto lifetime = 10.0f;

      for (auto iterator = m_particles.begin(); iterator != m_particles.end(); ) 
      {
          auto &particle = *iterator;
          
          auto age = current_time - particle.second;
          
          if (force || (age > lifetime))
          {
             auto node = particle.first;
             node->Disconnect(true);
 
             iterator = m_particles.erase(iterator);
          }
          else 
              ++iterator;
      }
   }


   void Simulate(Particle &particle, float current_time)
   {
      const auto radius = 10.0f;
      const auto height = 2.0f;
      const float distance = 50.0f;
      const auto speed = 1.5f;
      
      float angle = speed * (current_time + particle.second);
      
      Vector3 v;
      v.x = (float) sin(angle) * radius;
      v.y = height + (float) cos(angle) * radius;
      v.z = distance;
      
      auto node = particle.first;

      node->SetPosition(v);
   }
   
   void Emit(float time)
   {
      // The let macro allocates reference pointers which delete themselves when they go out of scope to avoid the need to manually perform that operation.
      let(SceneGraph, scene, m_zone->GetScene());     
      let(Node, scene_root, scene->GetRoot());
      let(Node, item_node, scene->CreateNode(scene_root.get()));
      let(Object, item_object, scene->CreateObject(item_node.get())); 
      let(SceneGraph, geometry, m_zone->Find("item.xsg"));
      item_object->SetGeometry(geometry.get());
      
      m_particles.push_back(Particle(item_node, time));
   }
     
   float m_last_time;
   std::vector<Particle> m_particles;
};


// Application entry point.
extern "C" Program *Program_Create(uint32_t init_params)
{
   return new App;
}
