#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <future>
#include <curses.h>
#include "build.hpp"

//#define YAML_CONFIGS
#ifdef YAML_CONFIGS
#else //JSON
  #include "json.h"
  using json = nlohmann::json;
#endif

std::string build_type_strings[] = {
  "clang++ -g -O0 -std=c++1y -c",
  "clang++ -g -O0 -std=c++1y",
  "clang++ -g -O0 -std=c++1y -fvisibility=hidden -Xlinker -dylib",
  "libtool -static",
};

int output_header( std::string project ) {
  std::cout << "\33[35m----------[\33[36;1m Stak: \33[0;33m" << project << " \33[35m]----------\33[39m" <<std::endl;
  return 0;
}

int output_building( std::string source ) {
  std::cout << "\33[36;1mStak \33[0;37m➜ \33[32mBuilding \33[34m" << source << "...\33[39m" <<std::endl;
  return 0;
}

int output_complete( ) {
  std::cout << "\33[36mStak \33[37m➜ \33[32mBuild Complete!\33[39m" <<std::endl;
  return 0;
}

int output_failure( std::string error ) {
  std::cout << "\33[36;1mStak \33[37m➜ \33[31mBuild Failed!\33[0;39m" <<std::endl << error << std::endl;
  return 0;
}
//
//
//
std::string get_object_name_from_source_name ( const std::string& source_name ) {
  std::size_t dot_location = source_name.find_last_of( "." );
  if( dot_location < 2 )
    return "";
  return source_name.substr( 0, dot_location ) + ".o";
}

//
//
//
int build_project ( ark::build::project_t& project ) {
  output_header ( project.name );

  std::string object_files = "";
  std::string compilation_string = "";
  std::string includes = "";
  std::vector< std::shared_future< bool > > sources_building;

  // build includes list
  for ( auto& include: project.includes ) {
    includes += " -I" + include;
  }

  // build each source file
  for ( auto& source: project.sources ) {

    output_building ( source );
    std::string object = get_object_name_from_source_name( source );
    if( !object.empty() ) {

      compilation_string = build_type_strings[ static_cast< int > ( ark::build::build_type::object ) ] + includes + " -o " + object + " " + source;
      object_files += " " + object;
      int error = system( compilation_string.c_str() );

      if( error ) {
        throw std::logic_error ( "Error occurred in building" );
        break;
      }
    }

    // build final linker string
    compilation_string  = build_type_strings[ static_cast< int > ( project.type ) ];
    compilation_string += " -o ";
    compilation_string += project.executable;
    compilation_string += object_files;

    // link object files
    int error = system( compilation_string.c_str() );
    
    if( !error ) {
      output_complete ( );
    } else {
      throw std::logic_error ( "Error occurred in building" );
      break;
    }
  }
  return 0;
}

struct ark::build::project_t load_project_from_file( std::string file_name ) {

  std::string file_extension = ".json";
  json project_json;
  struct ark::build::project_t new_project;

  // read json file
  std::ifstream input_file  { file_name, std::ios::in | std::ios::binary };
  if ( !input_file ) {
    throw std::logic_error( "Could not read JSON configuration file" );
  }
  std::string file_input( ( std::istreambuf_iterator< char >( input_file ) ), std::istreambuf_iterator< char >() );

  // if file is not empty
  if( !file_input.empty() ) {
      project_json = json::parse (file_input);

      // load configuration
      new_project.name = project_json.find("name")->get<std::string>();
      new_project.executable = project_json.find("executable")->get<std::string>();
      std::string type = project_json.find("type")->get<std::string>();

      if( type.compare("build::executable") == 0 ) {
        new_project.type = ark::build::build_type::executable;

      } else if( type.compare("build::dynamic") == 0 ) {
        new_project.type = ark::build::build_type::dynamic_library;

      } else if( type.compare("build::static") == 0 ) {
        new_project.type = ark::build::build_type::static_library;

      } else {
        std::string error = "Unknown build type \"";
        error += type;
        error += "\"";
        throw std::logic_error( error );
      }

      auto sources = project_json.find("sources");
      for ( auto& source: sources.value() ) {
        std::string file = source;
        new_project.sources.push_back(file);
      }

      auto includes = project_json.find("includes");
      for ( auto& include: *includes ) {
        std::string file = include;
        new_project.includes.push_back(file);
      }

    return new_project;
  }
  else {
    throw std::logic_error("JSON build configuration empty");
  }
}
//
//
//
int main( int argc, char** argv ) {

  try {
    // if argument 1 exists, try and find project with that name
    if ( argc <= 1 ) {
      throw std::logic_error("No project specified");
    }

    std::string build_project_name = argv[1];
    build_project_name += ".json";

  
    auto project = load_project_from_file( build_project_name );
    build_project ( project );
  } catch ( std::logic_error error ) {
    output_failure ( error.what() );
    return 1;
  } catch ( ... ) {
    output_failure ( "Project failed to build..." );
    return 1;
  }

  return 0;
}