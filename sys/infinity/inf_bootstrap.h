/* ---------------------------------------------------------------------------------

Project     : <><> XSG/connect : 3D Web DOM/Web Assembly Secure Interface.

              Copyright � 2002 - 2024, Advance Software Limited. All Rights Reserved.

              BSD Licensed.

File        : inf_managed_types.h

Description : <><> app C interface binding.

------------------------------------------------------------------------------------ */


#ifndef INF_BOOTSTRAP_H
#define INF_BOOTSTRAP_H


// Application must define this entrypoint.
extern "C" Program *Program_Create(uint32_t init_params);


static Program *__app = nullptr;


INF_EXPORT uint32_t app_init(uint32_t type_version, uint32_t zone_handle, uint32_t scene_handle)  // TODO: uint32_t init_params & pass to Program_Create
{    
   if (type_version != Type_Version)
      return 0; // Incompatible.
   
    // WORKAROUND: Force the import. Compiler optimizing it out even though process function calls it. 
   create_native(0);
   
   uint32_t init_params = 0; // TODO: Pass in to init()
   __app = Program_Create(init_params);
   
   // Our parent added a ref when accessing these handles. Base class (Program) destructor removes those refs.
   __app->m_zone = new Zone(zone_handle);
       __app->m_sg   = new SceneGraph(scene_handle);  // Scene the script is included from, which is not necessarily the zone scene root, which is accessed using zone->GetScene()
   
   return __app ? __app->Init() : 0;
}


INF_EXPORT uint32_t app_terminate()
{    
   auto rc =  0;

   if (__app)
   {
      rc = __app->Terminate();
      delete __app;
   }
   
   return rc;
}


INF_EXPORT uint32_t app_process(float time)
{    
    return __app ? __app->Process(time) : 0;
}


INF_EXPORT void app_move(float x, float y, float z)
{    
    if (__app)
      __app->OnMove(x, y, z);
}


INF_EXPORT void app_button(uint32_t b)
{    
   if (__app)
   __app->OnButtonDown(b);
}


INF_EXPORT void app_msg(uint32_t msg_id, uint32_t param_1, uint32_t param_2)
{    
    if (__app)
      __app->OnMsg(msg_id, param_1, param_2);
}


static Diagnostics * __d = nullptr;

void Log(const char *s)
{
   if (!__d)
      __d = new Diagnostics;
   
   __d->Log(s);
}



// Infinity object definitions that can't be inline here to keep compilation straightforward.

// We can do this here because inf_bootstrap.h should only been included into the file containing the Program definition, hence we won't end up with duplicate definitions.

Object *SceneGraph::CreateObject(Node *parent)
{
   return new Object(native_op(m_type, (uint32_t) SceneGraph_Op::CreateObject, m_handle, (uint_ptr) (parent ? parent->GetHandle() : 0) ));
}


#endif // INF_BOOTSTRAP_H