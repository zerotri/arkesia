#include "arkesia.hpp"
#include "term11/term11.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip>
static std::string return_current_time()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%X");
    return ss.str();
}
namespace ark {
    namespace memory {
        void block::create( uint64_t requested_size ) {
        #ifndef NDEBUG
            auto requested_base_address = (char*)gigabytes(8);
            auto allocation_flags = MAP_PRIVATE|MAP_ANON|MAP_FIXED;
        #else
            auto requested_base_address = (char*)0;
            auto allocation_flags = MAP_PRIVATE|MAP_ANON;
        #endif
            this->base_ptr = mmap(  requested_base_address,
                                    requested_size,
                                    PROT_READ|PROT_WRITE,
                                    allocation_flags,
                                    -1, 0);
        #ifndef NDEBUG
            if( this->base_ptr == MAP_FAILED ) {
            this->total_size = requested_size;
            std::cout << term11::fg( term11::Blue )
                      << "[ " << return_current_time() << " ] "
                      << term11::fg( term11::Red )
                      << "Error allocating "
                      << requested_size
                      << "bytes"
                      << std::endl
                      << " => \""
                      << strerror(errno)
                      << "\""
                      << std::endl;
            } else {
            std::cout   << term11::fg( term11::Blue )
                        << "[ " << return_current_time() << " ] "
                        << term11::fg( term11::Green )
                        << "Allocated "
                        << term11::fg( term11::Magenta )
                        << requested_size
                        << term11::fg( term11::Green )
                        << " bytes at "
                        << term11::fg( term11::Magenta )
                        << reinterpret_cast<void *>( this->base_ptr )
                        << std::endl
                        << " => \""
                        << "Testing..."
                        << "\""
                        << term11::reset()
                        << std::endl;
            }
        #endif
        };
        void block::destroy() {
            munmap( this->base_ptr, this->total_size );
        };
    }
}