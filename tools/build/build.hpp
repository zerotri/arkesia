#include <string>
#include <vector>

namespace ark {
	using string = std::string;

	namespace console {
		auto escape = ark::string("\33");
		namespace colors {
			auto red = escape + "[35m";
			auto blue = escape + "[35m";
			auto green = escape + "[35m";
			auto yellow = escape + "[35m";
			auto magenta = escape + "[35m";
			auto cyan = escape + "[36m";
		}
	}

	namespace build {
		enum class build_type{
			object,
			executable,
			dynamic_library,
			static_library
		};
		struct project_t {
			std::string name;
			std::string executable;
			build_type  type;
			std::vector< std::string > includes;
			std::vector< std::string > sources;
		};
	}
	namespace error {
		inline int out( const char* file, int line, const char* function,const char* string ) {
			printf( "\33[36m[\33[35m %s \33[36;1m@\33[0;33m %4i \33[36m] \33[0;34m %64s \33[31mERROR\33[0;39m %s\n", file, line, function, string );
			return -1;
		}

	}
}

#ifndef STAK_ERROR_H
#define STAK_ERROR_H
#include <stdint.h>

#define LOG_ERRORS 0
#define LOG_INFO 0

/*int log( const char* file, int line, const char* function,const char* string ) {
	printf( "[ %s @ %4i ] <LOG> %s - %s\n", file, line, function, string );
}*/
//int error_throw( const char* file, int line, const char* function,const char* string );

#if LOG_ERRORS
	#define ark_error_throw( x ) ark::error::out( __FILE__, __LINE__, __FUNCTION__, x )
#else
	#define ark_error_throw( x ) -1
#endif

#if LOG_INFO
	#define ark_log( x, ... ) printf("\33[36m[\33[35m %32s \33[36;1m@\33[0;33m %4i \33[36m] \33[0;34m %64s\33[32m  LOG \33[0;39m " x "\n",  __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )
#else
	#define ark_log( x, ... )
#endif
#define stak_error_none( ) 0

typedef int32_t stak_error_t;

#endif