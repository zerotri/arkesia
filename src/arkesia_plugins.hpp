#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>

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
            char working_directory[1024];
            getcwd( working_directory, 1024 );
            filename.insert(0, working_directory);
            plugin_t plugin = {};
            plugin.dynamic_library  = dlopen( filename.c_str() , RTLD_NOW|RTLD_GLOBAL);
            if (!plugin.dynamic_library) {
                fprintf(stderr, "%s\n", dlerror());
            }
            if( plugin.dynamic_library != nullptr) {
                plugin.initialize = dl_cast<void()>( dlsym( plugin.dynamic_library, "initialize" ) );
                plugin.shutdown   = dl_cast<void()>( dlsym( plugin.dynamic_library, "shutdown" ) );
                plugin.update     = dl_cast<void( float )>( dlsym( plugin.dynamic_library, "update" ) );
                plugin.draw       = dl_cast<void()>( dlsym( plugin.dynamic_library, "draw" ) );
                plugin.get_api    = dl_cast<void*( uint32_t )>( dlsym( plugin.dynamic_library, "get_api" ) );
            }
            else {
                std::cout << "Could not load plugin " << filename << std::endl;
                std::cout << "working directory: " << working_directory << std::endl;
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