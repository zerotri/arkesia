#include <stdint.h>
#include <string>
#include <chrono>
#include <iostream>
#include <type_traits>
#include <chrono>
#include <ctime>

#define ARK_EXPORT extern "C"
using std::chrono::system_clock;

namespace ark {

    using time_t = double;
    using string = std::string;

    namespace states {
        struct game_state {
            void (*update)( ark::time_t delta_time );
            void (*draw)( ark::time_t delta_time );
        };
    }
    /*namespace plugins {
        uint32_t load();
        uint32_t unload();
        uint32_t reload();
    }*/
    namespace memory {

        constexpr uint64_t kilobytes ( uint64_t number )
        {
            return number*1024ULL;
        }
        constexpr uint64_t megabytes ( uint64_t number )
        {
            return kilobytes(number)*1024ULL;
        }
        constexpr uint64_t gigabytes ( uint64_t number )
        {
            return megabytes(number)*1024ULL;
        }

        struct block {
            void* base_ptr = 0;
            uint32_t total_size = 0;

            void create( uint64_t requested_size );
            void destroy();
            template<typename _T>
            _T get_offset( uint64_t offset ) {
                return reinterpret_cast<_T>( reinterpret_cast<char*>(this->base_ptr) + offset );
            }
        };
    }
}