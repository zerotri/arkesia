#pragma once
#include <stdint.h>

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

namespace arkesia {
    struct memory_block {
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