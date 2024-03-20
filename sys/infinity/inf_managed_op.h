/* ---------------------------------------------------------------------------------

Project     : <><> XSG/connect : 3D Web DOM/Web Assembly Secure Interface.

              Copyright � 2002 - 2024, Advance Software Limited. All Rights Reserved.

              BSD Licensed.

File        : inf_managed_op.h

Description : DOM class interop.

------------------------------------------------------------------------------------ */

#ifndef INF_MANAGED_OP_H
#define INF_MANAGED_OP_H

// enum class allows us to have the same identifier in different enums.

enum class Zone_Op { GetScene = 1, FindSceneGraph, Loading };
enum class SceneGraph_Op { Delete=0, CreateNode = 1, CreateObject, GetRoot, FindMaterial, FindNode, GetAnimation };
enum class Material_Op { Delete=0, GetPart = 1, GetColour, SetColour };
enum class Material_Part_Op { Delete=0, GetInput = 1 };
enum class Mixer_Input_Op { Delete=0, SetScale = 1, SetOffset };
enum class Diagnostics_Op { Log = 1, Get };
enum class Sound_Manager_Op { Sample = 1, Get };
enum class Animation_Op { Delete=0, GetDuration = 1 };


// IMPORTANT: Ensure derived class parameters don't stomp base class methods.
enum class Attachment_Op { Delete=0, SetParameter = 1 }; 
enum class Object_Op { Delete=0, SetParameter = 1, SetGeometry, GetGeometry, Intersection, SetAnimationPlaybackOffset };
enum class Node_Op   { Delete=0, SetParameter = 1, SetPosition, GetPosition, Disconnect, SetVelocity, GetAttachment };
enum class Mesh_Op   { Delete=0, SetParameter = 1, GetVertexCount, GetVertexPosition, SetVertexPosition, Evaluate };

#endif // INF_MANAGED_OP_H