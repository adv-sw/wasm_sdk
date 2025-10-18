
//--------------------------------------------------------------------------------------
// File: doodle.cpp
//
// Audio Visualizer
//
//-----------------------------------------------------------------------------------------------

/* Copyright (c) 2020-2025, Advance Software Limited. All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification are permitted provided that the following conditions are met :

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Advance Software Limited nor the names of the
      contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ADVANCE SOFTWARE LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#include <infinity.h>
using namespace Infinity;
#include <inf_bootstrap.h>

#include <math.h>

using namespace std;


#include "beat_detect.cpp"


class Doodle : public Program
{
public:
   Doodle() { m_beat_detector = nullptr; m_visualize_input = nullptr; m_audio = nullptr; m_scene_initialized = false; }
   
   // Simple HTTP_GET handler - currently waits (sync) for its respose as a quicky.
   /*
   string Get(string uri)
   {
      HttpClient c = new HttpClient();
      Task<string> t = c.GetStringAsync(uri);
      return t.Result;
   }*/
   

   uint32_t Init()
   {
      m_beat_detector = new Beat_Detector();
      m_audio = new Sound_Manager();
              
      return true; // Initialized ok, proceed to update loop.
   }
   
   
   void Scene_Init()
   {
       auto root = m_sg->GetRoot();

       // If our scene hasn't loaded yet, we can't initialize.
       if (!root)
          return;

       auto m = m_sg->FindMaterial("main");

       if (m)
       {
           Log("Got mtl");
           m->Hint(Material_Hint_Dynamic);  // This material changes frequently.

           auto mp = m->GetPart(0); ; // 0=diffuse. TODO: use enum or define

           if (mp)
              Log("MP :)");
           else
             Log("!MP");

           m_visualize_input = mp->GetInput(1);

          if (m_visualize_input)
             Log("Got VI");
          else
             Log("!VI");
       }
   
       m_scene_initialized = true;
   }


   uint32_t Process(float current_time)
   {
      const float _visualize_level_adjust = 1.28f;
      const float _visualize_scale_adjust = 0.2f;

      if (!m_scene_initialized)
         Scene_Init();
      
      if (m_scene_initialized && m_visualize_input)
      {
         float freq_sample = (1.0f - m_audio->Sample(0, 0));
         
            // Modulate scale to get decal beating in time to the music - we currently pick up the beat from the lowest fft sample.      
         float level = _visualize_level_adjust + freq_sample * _visualize_scale_adjust;

          m_v.x = level;
          m_v.y = -level;
          m_visualize_input->SetTextureCoordinateScale(m_v);
   

          // Centre
          float range = 1 / level;
          float margin = (1 - range) * 0.5f;
          float scaled_margin = margin * level;

          m_v.x = - scaled_margin; 
          m_v.y  = scaled_margin;
   
          m_visualize_input->SetTextureCoordinateOffset(m_v);
          
          return 1;
      }
      
      return 0;
   }
    
  
   uint32_t Terminate()
   {
      if (m_beat_detector)
        delete m_beat_detector;
      
      if (m_visualize_input)
        delete m_visualize_input;
      
      delete m_audio;
      
      return 1;
   }
   
   
   bool           m_scene_initialized;
   Mixer_Input   *m_visualize_input;

   Sound_Manager *m_audio;
   Vector2        m_v; // Used for calc. Keep it around to reduce garbage collection overhead. TODO: Not required C++ version.
   Beat_Detector *m_beat_detector;
};



extern "C" Program *Program_Create(uint32_t init_params)
{
   return new Doodle;
}