#include "game_state_main.h"
#include <GL/Regal.h>
#include <iostream>

ARK_EXPORT void initialize() {

}
ARK_EXPORT void shutdown( ) {

}
ARK_EXPORT void update( float dt ) {

}
ARK_EXPORT void draw( ) {
	glClearColor(0, 0, 0, 1);
	glClear( GL_COLOR_BUFFER_BIT );
}
ARK_EXPORT void* get_api( uint32_t ) {

}