#include <string>

#pragma once
namespace term11 {
  enum color_codes{
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7,
  };
  std::string escape( std::string input );
  std::string fg( enum color_codes color );
  std::string bg( enum color_codes color );
  std::string reset( );
}