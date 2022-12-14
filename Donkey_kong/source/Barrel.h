//
//  Barrel.h
//  Donkey_Kong
//
//  Created by Luke Gulson, Chris Callahn, Mark George on 12/14/22.
//
//

#ifndef __Donkey_Kong__Barrel__
#define __Donkey_Kong__Barrel__

#include "Game.h"

using namespace tcg;

class Barrel{

  friend class Game;

  std::vector < vec2 > Barrel_vert;
  std::vector < vec2 > Barrel_uv;

  vec2 barrel_bbox[2];

  std::vector<unsigned char> barrel_im;
  unsigned im_width, im_height;


  float angle_increment = - M_PI / 32;

  //Barrel State
  struct {
    vec2 cur_location;
    vec2 velocity;
    float angle;
      int platform_num;
      int num_barrel;
      bool is_start =true;
      vec2 start_pos = vec2(-.63, .37);


  } state;

  //OpenGL variables for a barrel
  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
    GLuint barrel_texture;
  } GLvars;

public:

  Barrel(unsigned int index);

    inline void which_platform(){
        if(state.cur_location.y < -.74){
            state.platform_num = 0;
        }

        if(state.cur_location.y < -.48 && state.cur_location.y > -.74){
            state.platform_num = 1;
        }

        if(state.cur_location.y < -.22 && state.cur_location.y > -.48){
            state.platform_num = 2;
        }
        if(state.cur_location.y < .05 && state.cur_location.y > -.22){
            state.platform_num = 3;
        }
        if(state.cur_location.y < .32 && state.cur_location.y > .05){
            state.platform_num = 4;
        }
        if(state.cur_location.y > .36){
            state.platform_num = 777; // final platform
        }
    }

    // made to make mario fall if he is off the ledge.
    inline void send_to_platform(){

        if(state.platform_num == 1 && state.cur_location.x > 1.16){
            state.cur_location.y = -.92;
        }

        if(state.platform_num == 2 && state.cur_location.x < -1.18){
            state.cur_location.y = -.58;
        }
        
        if(state.platform_num == 3 && state.cur_location.x > 1.18){
            state.cur_location.y = -.31;
        }
        
        if(state.platform_num == 4 && state.cur_location.x < -1.18){
            state.cur_location.y = -.05;
        }

        if(state.platform_num == 777 && state.cur_location.x > 1.06){
            state.cur_location.y = .2;
        }
    }

  void update_state(vec4 extents);

  void gl_init();

  void draw(mat4 proj);

};


#endif /* defined(__Donkey_Kong__Barrel__) */
