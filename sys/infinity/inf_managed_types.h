/* ---------------------------------------------------------------------------------

Project     : <><> XSG/connect : 3D Web DOM/Web Assembly Secure Interface.

              Copyright ? 2002 - 2023, Advance Software Limited. All Rights Reserved.

              BSD Licensed.

File        : inf_managed_types.h

Description : Managed DOM type definitions.

------------------------------------------------------------------------------------ */

#ifndef INF_MANAGED_TYPES_H
#define INF_MANAGED_TYPES_H

// Sorted in approx order of access frequency to optimize cache hit likelihood.
// TODO: Run analysis to determine real-world frequency to optimize ordering for v2.
// TODO: Remove abstracts & those without managed reflection to keep list minimal & relevant.
// TODO: Every time this is changed, *every* managed script needs a recompile, so do so infrequently & bump version when this occurs.


namespace Infinity
{
   const unsigned long Type_Version = 1;

   enum Type {

   Type_Unknown = 0,

   Type_Node,
   
   Type_SceneGraph,
 
   Type_Object,
   Type_Mesh,
   Type_PrimitiveGroup,
   Type_Light,

   Type_Camera,
 
   Type_Texture,

   Type_Material,
   Type_Material_Part,
   Type_Mixer_Input,
   Type_Material_Override,
  
   Type_Sound_Manager,
   Type_Sound,

   Type_Animation,
   Type_Animation_Channel,

   Type_Emitter,
   Type_Instance,

   Type_Zone,

   Type_Vector4,
   Type_Transform,
   
   Type_Ray,
   Type_Skin,

   Type_CheckBox,
   Type_ComboBox,

   Type_Kernel,
   Type_Action,

   Type_Cursor,
   Type_Diagnostics,

   Type_Window,

   Type_Menu,
   Type_Menu_Component,

   Type_Font,

   Type_Attachment,
   Type_Behaviour,

   Type_Listener,
   
   Type_Parser,
   Type_Physics,
   Type_Protected,

   Type_Scalar,

   Type_String,

   Type_Timer,

   Type_Bool,
   Type_Vector2,
   Type_Vector3,
   Type_Data,
   Type_UInt,
   Type_Ptr,

   Type_Max
};


};

#endif // INF_MANAGED_TYPES_H
