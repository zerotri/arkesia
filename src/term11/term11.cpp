#include "term11.hpp"

namespace term11 {
    std::string escape( std::string input ) { return std::string("\33[") + input + std::string("m"); }
    std::string fg( enum color_codes color ) {
        return escape( std::to_string( 30 + (int)color ) );
    }
    std::string bg( enum color_codes color ) {
        return escape( std::to_string( 40 + (int)color ) );
    }
    std::string reset( ) {
        return escape( std::to_string( 39 ) );
    }
}