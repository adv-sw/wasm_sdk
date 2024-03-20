/* ---------------------------------------------------------------------------------

Project     : <><> XSG/connect : 3D Web DOM/Web Assembly Secure Interface.

              Copyright � 2002 - 2024, Advance Software Limited. All Rights Reserved.

              BSD Licensed.

File        : inf_managed_types.h

Description : Definition of managed types.

------------------------------------------------------------------------------------ */

#ifndef INF_MANAGED_H
#define INF_MANAGED_H

// Block __leaf__ attribute unknown warning.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"

#include <stdint.h>
typedef void * uint_ptr;

#define INF_EXPORT __attribute__((__visibility__("default"))) extern "C"
#define INF_IMPORT(x) __attribute__((import_name(#x)));

// Easy managed pointers.
#define let(type, id, value) std::shared_ptr<type> id(value);

extern "C" 
{
uint32_t create_native(uint32_t type_id) INF_IMPORT(create_native);
uint32_t native_op(uint32_t type_id, uint32_t op_cmd, uint32_t handle=0, uint_ptr data=0) INF_IMPORT(native_op);
uint32_t release_native(uint32_t type_id, uint32_t handle) INF_IMPORT(release_native);
}


#define as_float(x) *(float*)(&(x))
#define as_uint(x)  *(uint32_t*)(&(x))
#define array_len(x) (sizeof(x)/sizeof(x[0]))

// For now.
#define INF_ALIGN_16 

class Vector2
{
public:
   Vector2() {}
   Vector2(float _x, float _y) : x(_x), y(_y){}
   Vector2(const Vector2 &src) : x(src.x), y(src.y) {}

   void operator = (const Vector2 &src)
   {
      x = src.x;
      y = src.y;
   }

   bool operator == (const Vector2 &src) const
   {
      return (x == src.x) && (y == src.y);
   }

   bool operator != (const Vector2 &src) const
   {
      return (x != src.x) || (y != src.y);
   }

   Vector2 operator - () const
   {
      return Vector2(-x, -y);
   }

   Vector2 operator +  (const Vector2 &src) const
   {
      return Vector2(x + src.x, y + src.y);
   }

   Vector2 operator - (const Vector2 &src) const
   {
      return Vector2(x - src.x, y - src.y);
   }

   Vector2 operator * (float scale) const
   {
      return Vector2(scale*x, scale*y);
   }

   void operator *= (float scale)
   {
      x *= scale;
      y *= scale;
   }

   void operator -= (const Vector2 &src)
   {
      x -= src.x;
      y -= src.y;
   }

   void operator += (const Vector2 &src)
   {
      x += src.x;
      y += src.y;
   }

   float x, y;
};


class Vector3
{
public:
   Vector3() {}
   Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
   Vector3(const Vector3 &src) : x(src.x), y(src.y), z(src.z) {}

   void operator = (const Vector3 &src)
   {
      x = src.x;
      y = src.y;
      z = src.z;
   }

   bool operator == (const Vector3 &src) const
   {
      return (x == src.x) && (y == src.y) && (z == src.z);
   }

   bool operator != (const Vector3 &src) const
   {
      return (x != src.x) || (y != src.y) || (z != src.z);
   }

   Vector3 operator - () const
   {
      return Vector3(-x, -y, -z);
   }

   Vector3 operator +  (const Vector3 &src) const
   {
      return Vector3(x + src.x, y + src.y, z + src.z);
   }

   Vector3 operator - (const Vector3 &src) const
   {
      return Vector3(x - src.x, y - src.y, z - src.z);
   }

   Vector3 operator * (float scale) const
   {
      return Vector3(x *scale, y *scale, z * scale);
   }

   void operator *= (float scale)
   {
      x *= scale;
      y *= scale;
      z *= scale;
   }

   void operator += (const Vector3 &src)
   {
      x += src.x;
      y += src.y;
      z += src.z;
   }

   void operator -= (const Vector3  &src)
   {
      x -= src.x;
      y -= src.y;
      z -= src.z;
   }


   float x, y, z;
};


class INF_ALIGN_16 Vector4
{
public:
   Vector4() {}
   Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
   Vector4(const Vector4 &src) : x(src.x), y(src.y), z(src.z), w(src.w) {}

   void operator = (const Vector4 &src)
   {
      x = src.x;
      y = src.y;
      z = src.z;
      w = src.w;
   }

   bool operator == (const Vector4 &src) const
   {
      return (x == src.x) && (y == src.y) && (z == src.z) && (w == src.w);
   }

   bool operator != (const Vector4 &src) const
   {
      return (x != src.x) || (y != src.y) || (z != src.z) || (w != src.w);
   }

   Vector4 operator - () const
   {
      return Vector4(-x, -y, -z, -w);
   }

   Vector4 operator +  (const Vector4 &src) const
   {
      return Vector4(x + src.x, y + src.y, z + src.z, w + src.w);
   }

   Vector4 operator - (const Vector4 &src) const
   {
      return Vector4(x - src.x, y - src.y, z - src.z, w - src.w);
   }

   Vector4 operator * (float scale) const
   {
      return Vector4(x *scale, y * scale, z *scale, w * scale);
   }

   void operator *= (float scale)
   {
      x *= scale;
      y *= scale;
      z *= scale;
      w *= scale;
   }

   void operator += (const Vector4 &src)
   {
      x += src.x;
      y += src.y;
      z += src.z;
      w += src.w;
   }

   float x, y, z, w;
};


#include "./inf_managed_types.h"
#include "./inf_managed_op.h"

#include <memory>
using namespace std;

#include "./inf_managed_class.h"

void Log(const char *s);

class Program
{
   public:
   
   Program()
   {
      m_zone = nullptr;
      m_sg   = nullptr;
   }

   virtual ~Program()
   {
      if (m_sg)
         delete m_sg;

      if (m_zone)
         delete m_zone;
   }      
   
   virtual uint32_t Init() { return 0; }
   virtual uint32_t Process(float current_time) { return 0; }
   virtual uint32_t Terminate() { return 0; }

   virtual void OnMove(float x, float y, float z) {}
   
   virtual void OnMsg(uint32_t msg_id, uint32_t param_1, uint32_t param_2) {}
   virtual void OnButtonDown(uint32_t button) {}
      
   Infinity::Zone       *m_zone;
   Infinity::SceneGraph *m_sg;   
};



// Currently supported messages & interpretation.
// TODO: Split out to its own header file : inf_app_msg.h
const uint32_t INF_MSG_KEY_DOWN = 0x0100;
const uint32_t INF_MSG_KEY_UP   = 0x0101;

const uint32_t INF_KEY_LEFT  = 0x25; // LEFT ARROW key
const uint32_t INF_KEY_UP    = 0x26; // UP ARROW key
const uint32_t INF_KEY_RIGHT = 0x27; // RIGHT ARROW key
const uint32_t INF_KEY_DOWN  = 0x28; // DOWN ARROW key



// Restore default diagnostics.
#pragma clang diagnostic pop


#endif // INF_MANAGED_H