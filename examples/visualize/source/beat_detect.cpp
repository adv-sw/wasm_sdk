//--------------------------------------------------------------------------------------
// File: beat_detect.cpp
//
// Feature demonstration.
//
//-----------------------------------------------------------------------------------------------

/* Copyright (c) 2020-2024, Advance Software Limited. All rights reserved.

Based on code by DJ Katy.

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

#include <math.h> 
#include <vector>

#include <infinity.h>
using namespace Infinity;


class Beat_Detector
{
public:

   Beat_Detector()
   {
      m_beat_threshold_volume = 0.3f;
      m_beat_sustain = 0.150f;
      m_beat_post_ignore = 0.250f;
      m_beat_track_cutoff = 10.0f;
      m_beat_last_time = 0.0f;
      m_beat_ignore_last_time = 0.0f;
   }

   bool Beat_Detect(float sample, float current_time)
   {
      bool beat = false;
      
      if ((sample >= m_beat_threshold_volume) && (m_beat_last_time == 0.0f) && (m_beat_ignore_last_time == 0.0f))
      {
         m_beat_last_time = current_time;
         
         m_beat_times.push_back(m_beat_last_time);

         while (true)
         {
            int last_index = m_beat_times.size() -1;
            
            if (!((current_time - m_beat_times[last_index]) > m_beat_track_cutoff))
               break;

            
            m_beat_times.erase(m_beat_times.begin() + last_index);

            if (m_beat_times.size() == 0)
                 break;
         }
      }

      if ((current_time - m_beat_last_time) < m_beat_sustain)
         beat = true;
      else if ((m_beat_ignore_last_time == 0.0f) && (m_beat_last_time != 0.0f))
      {
         m_beat_last_time = 0.0f;
         m_beat_ignore_last_time = current_time;
      }

      if ((current_time - m_beat_ignore_last_time) >= m_beat_post_ignore)
         m_beat_ignore_last_time = 0.0f;

      return beat;
   }
   

   // Beat detection.
   std::vector<float>  m_beat_times;

   // Beat detection parameters
   float m_beat_threshold_volume;
   float m_beat_sustain;
   float m_beat_post_ignore;

   float m_beat_last_time;
   float m_beat_ignore_last_time;

   // List of how many ms ago the last beats were
   float m_beat_track_cutoff;
};