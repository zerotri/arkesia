#include <dlfcn.h>
#include <functional>
namespace ark {
    namespace plugin {
        template <typename Signature>
        Signature* dl_cast(void* f)
        {
            return reinterpret_cast<Signature*>(f);
        }
        struct plugin_t {
            void* dynamic_library;

            // standard 
            void (*initialize)( );
            void (*shutdown)( );
            void (*update)( float dt );
            void (*draw)( );
            void* (*get_api)( uint32_t );
        };
        plugin_t load( std::string filename ) {
            plugin_t plugin = {};
            plugin.dynamic_library  = dlopen( filename.c_str() , RTLD_NOW|RTLD_GLOBAL);
            if( plugin.dynamic_library < 0) {
                plugin.initialize = dl_cast<void()>( dlsym( plugin.dynamic_library, "initialize" ) );
                plugin.shutdown   = dl_cast<void()>( dlsym( plugin.dynamic_library, "shutdown" ) );
                plugin.update     = dl_cast<void( float )>( dlsym( plugin.dynamic_library, "update" ) );
                plugin.draw       = dl_cast<void()>( dlsym( plugin.dynamic_library, "draw" ) );
                plugin.get_api    = dl_cast<void*( uint32_t )>( dlsym( plugin.dynamic_library, "get_api" ) );
            }
            return plugin;
        }
        void* request_api( plugin_t plugin, uint32_t api ) {
            //plugin.get_api
        }
        template<typename _T>
        _T request_api( plugin_t plugin ) {

        }
        void close_plugin( plugin_t plugin ) {
            dlclose( plugin.dynamic_library );
        }
    }
}