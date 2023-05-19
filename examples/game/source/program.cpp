//----------------------------------------------------------------------------------------------------------------------
//
// <><> Infinity/XSG secure programming environment : Simple shoot-em up game.
//
// File: program.cpp
//
// Implements a simple game engine & associated game within the <><> Infinity/XSG secure programming environment. 
//
//----------------------------------------------------------------------------------------------------------------------


#include <infinity.h>

using namespace Infinity;
#include <inf_bootstrap.h>


using namespace std;


#include "./overlay.h"
#include "./player.h"
#include "./util.h"


#include <set>
std::set<uint32_t> __keys_down;


class Game : public Program
{
public:
   
   Game()
   {
      m_game_state = Game_State::Begin;
      m_scene = nullptr;
      m_current_time = 0.0f;
   }    
    
   uint32_t Init()
   {   
       m_scene = m_zone->GetScene();
       m_scene_root = m_scene->GetRoot();       
       
       m_title_geometry = m_zone->Find("title.xsg");       
       m_stats_geometry = m_zone->Find("stats.xsg");
       
       m_overlay = new Overlay(m_scene, m_scene_root);
         
       Vector3 p(100, -35, 80);
       m_overlay->m_node->SetPosition(p);
       m_overlay->m_object->SetGeometry(m_title_geometry);
        
       // Asset load
       m_player_geom = m_zone->Find("player.xsg");
       m_player_geom_xplode = m_zone->Find("player_x.xsg");
       m_ordnance_geom = m_zone->Find("bullet.xsg");
       
       m_npc_geom        = m_zone->Find("other.xsg");
       m_npc_geom_xplode = m_zone->Find("other_x.xsg");
      
       // Default configuration override - we're a game that implements custom controls.
       static uint32_t init_flags_ok                = 0x01;
       static uint32_t init_flags_control_exclusive = 0x02;
       
       return init_flags_ok|init_flags_control_exclusive; // Initialized ok, proceed to update loop.
    }
    
    
    void Game_Init()
    {
       // Configure player ... 
       m_player = new Player(m_scene, m_scene_root);
     
       m_player->Reset(m_player_geom);

       Node *n = m_stats_geometry->FindNode("score");

       if (n)
       {
           m_ui_player_score = n->GetAttachment();
           //n.Dispose();
       }

       UI_Update();

       // Configure non player character ...
       NPC *e = new NPC(m_scene, m_scene_root);
     
       Vector3 p(5,0,0);
       e->m_node->SetPosition(p);
       e->m_object->SetGeometry(m_npc_geom);
       e->Init();
       m_npc.push_back(e);
 
     
       // Hide title sequence ...
       //m_overlay.m_node.SetVisibility(false);
      
       m_overlay->m_object->SetGeometry(m_stats_geometry);
   }   
   
    
   void Game_Play(float current_time)
   {

       if (m_player->m_state == Actor::State::Expiring)
       {
           if (current_time >= m_player->m_state_transition_time)
           {
              m_player->Reset(m_player_geom);
              m_player->m_state = Actor::State::Live;
           }
       }

       for (auto i = m_npc.begin() ; i != m_npc.end() ; ++i)
       {
          (*i)->Process(current_time, m_npc_geom);
       }
     

       // Remove any expired ordnance.
     
       bool done = false;
     
       while (!done)
       {
           done = true;

           for (auto i = m_player->m_ordnance.begin(); i != m_player->m_ordnance.end(); ++i)
           {
              auto ord = *i;
           
              if (ord->Expired(current_time))
              {
                 m_player->m_ordnance.erase(i);
                 done = false;
                 break; // Round again - loop iterator broken by removal.
              }
           }
       }
     
     
       // Determine whether any player ordnance has hit a non player character  ...

       for (int ord = 0; ord < m_player->m_ordnance.size(); ord++)
       {
          for (int i = 0; i < m_npc.size(); i++)
          {
             m_player->m_score += m_npc[i]->Interact(m_player->m_ordnance[ord]->m_object, current_time, m_npc_geom_xplode);
          }
       }

     
       // TODO: Update score visualization
       
       
       // Check player/npc interaction

       for (int i = 0; i < m_npc.size(); i++)
       {
          if (m_player->Interact(m_npc[i], current_time, m_player_geom_xplode))
          {
             GameOver();
             break;
          }
       }
   }
   
   bool Loading() { return m_zone->Loading(); }
   
   uint32_t Process(float current_time)
   {
      m_current_time = current_time;
     
      if (m_game_state == Game_State::Begin)
      {
         if (!Loading())   
         {
            // Initialize game when everything required has loaded.   
            Game_Init();
            SetState(Game_State::Play);
         }
       
         // Don't jump into into the game processing loop until the first frame has
         // been rendered to enable bounding volumes, etc. to calculate in the render pass.
         // TODO: Check whether this is required & optimal solution.
         return 0;
      }

      if (m_game_state == Game_State::Play)
      {
          Game_Play(current_time);
          UI_Update();
          
          if (__keys_down.find('A') != __keys_down.end())
          {
             const float speed = 2.0f; // TODO: Factor in frame rate.
             
             Vector3 p;
             m_player->m_node->GetPosition(p);
             p += Vector3(0,0,speed);
             m_player->m_node->SetPosition(p);
          }
      }
     
      return 0;
    }
    

    uint32_t HiScore(uint32_t score)
    {
       // TODO: Implement hi-score table.
       return 0;
    }

    
    enum class Game_State { Title, Begin, Play, GetPlayerName };
    
    
    void SetState(Game_State gs)
    {
       m_game_state = gs;
    }
    
    
    void HighScoreTable_Insert(uint32_t position, const char *name)
    {
    }
    
    
    void GameOver()
    {
        uint32_t position = HiScore(m_player->m_score);

        if (position != 0)
        {
            const char *name = "player";
            SetState(Game_State::GetPlayerName);
            
            HighScoreTable_Insert(position, name);
            SetState(Game_State::Title);
        }
    }


    void OnButtonDown(uint32_t button)
    {
       if (button == 0)
       {
          if (m_game_state == Game_State::Title)
          {
              SetState(Game_State::Begin);
          }
          else if (m_game_state == Game_State::Play)
          {
             if (m_player->m_state == Actor::State::Live)
             {
                Vector3 v;
                m_player->m_node->GetPosition(v);

                Ordnance *ord = new Ordnance(m_scene, m_scene_root, m_current_time);
                ord->m_object->SetGeometry(m_ordnance_geom);

                // TODO: Add & locate a hardpoint to specify ordnance origin, instead of hardwired origin.
                const float ordnance_origin_offset = 0.5f;
                Vector3 p(v.x, v.y + ordnance_origin_offset, v.z);
                ord->m_node->SetPosition(p);
            
                Vector3 vel(15.0f, 0.0f, 0.0f);
                ord->m_node->SetVelocity(vel);
                
                m_player->m_ordnance.push_back(ord);
             }
          }
       }
       
       if (button == 1)
       {
          // TODO: special weapons, bombs, etc.
       }

    }

  
    void OnMove(float x, float y, float z)
    {
       if (m_game_state == Game_State::Play)
       {
          // Flip standard y/z controls to emulate side scrolling shoot-em up.
          if (m_player->m_state == Actor::State::Live)
          {
             Vector3 p;
             m_player->m_node->GetPosition(p);
             p += Vector3(x,z,0);
             m_player->m_node->SetPosition(p);
          }
       }
    }


    void OnMsg(uint32_t msg_id, uint32_t param_1, uint32_t param_2)
    {
       if (msg_id == INF_MSG_KEY_DOWN)
         __keys_down.insert(param_1);
      
       else if (msg_id == INF_MSG_KEY_UP)
         __keys_down.erase(param_1);
    }

    
   uint32_t Terminate()
   {   
       // TODO: Do this properly.
   
       m_player = nullptr;
     
       // Enemies
       m_npc.clear();
    
       m_overlay = nullptr;
      
       // TODO: Release assets
      
      return 0;
   }
    
    
   ~Game()
   {
      // Dispose of assets in Terminate rather than destructor.
   }
   
   
   void UI_Update()
   {
      if (m_ui_player_score)
      {
         if (m_player->m_score != m_player->m_prev_score)
         {
            char msg[100] = "Score : ";
         
            Append_Number(msg, array_len(msg), m_player->m_score);
         
            //swprintf(msg, array_len(msg), L"S%d", m_player->m_score);

            // TODO: Wide string equivalent.
            m_ui_player_score->SetParameter("text", msg);
            m_player->m_prev_score = m_player->m_score;
         }
      }
   }    
    
    
    // ----------------------
    //  Asset sources :
    // ----------------------
   
    SceneGraph *m_scene;
    Node       *m_scene_root;
   
    SceneGraph *m_title_geometry;
    SceneGraph *m_stats_geometry;
   
    SceneGraph  *m_player_geom;
    SceneGraph  *m_player_geom_xplode;
    SceneGraph  *m_ordnance_geom;

    SceneGraph *m_npc_geom;
    SceneGraph *m_npc_geom_xplode;
   
    // ----------------------
    //  Custom assets :
    // ----------------------
   
    // Player(s)
    Player     *m_player;
    Attachment *m_ui_player_score; 
   
    // Non-player characters
    std::vector<NPC*> m_npc;    
    
    // Misc
    Overlay *m_overlay;
    
    // ----------------------
    //  Custom state :
    // ----------------------
    
    Game_State m_game_state;
   float m_current_time;
};


Program *Program_Create(uint32_t init_params)
{
   return new Game;
}
