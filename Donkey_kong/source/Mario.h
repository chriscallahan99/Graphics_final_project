//
//  Mario.h
//  Donkey_Kong
//
//  Created by Luke Gulson, Chris Callahn, Mark George on 12/14/22.
//
//

#ifndef __Donkey_Kong__Ship__
#define __Donkey_Kong__Ship__

#include "Game.h"

using namespace tcg;

class Mario{

    friend class Game;

    std::vector < vec2 > mario_pos;
    std::vector < vec2 > mario_uv;

    std::vector<unsigned char> mario_im;
    unsigned mario_im_width, mario_im_height;

    vec2 mario_bbox[2];
    std::vector < vec2 > outline;

    std::vector<unsigned char> mario_t_im;
    unsigned mario_t_im_width, mario_t_im_height;

    std::vector<unsigned char> mario_left;
    unsigned s_left_width, s_left_height;

    vec2 left_bbox[2];
    std::vector < vec2 > outline_left;

    std::vector<unsigned char> mario_right;
    unsigned s_right_width, s_right_height;

    vec2 right_bbox[2];
    std::vector < vec2 > outline_right;

    std::vector<unsigned char> mario_left_t;
    unsigned s_left_t_width, s_left_t_height;

    std::vector<unsigned char> mario_right_t;
    unsigned s_right_t_width, s_right_t_height;

    //Initialized in constructor
    float max_speed;
    float damping_fact;
    float accel;
    float grav;
    bool is_start = true;

    // Determine which platform mario is on
    // Begin with Mario on starting platform
    bool is_start_platform = true;
    bool is_final_platform;
    bool is_even_platform;
    bool is_odd_platform;


    enum{ _NO_TURN, _TURN_LEFT, _TURN_RIGHT};

    enum{startClimb, stopClimb};


    //Mario State
    struct {
        vec2 cur_location;
        vec4 pointing;
        vec2 velocity;
        bool init_jump;
        bool jump_on;
        bool in_ladder_range;
        bool is_climbing;

        uint8_t turning;
        float angle;
        float platform_dist;
        int platform_num;
        int parity;
        float x_end;
        float y_end;

    } state;

    //OpenGL variables for a mario
    struct {
        GLuint vao;           //Vertex array object
        GLuint program;       //shader program
        GLuint buffer;        //Vertex buffer objects
        GLuint vertex_shader, fragment_shader;  //Shaders
        GLint vpos_location, vtex_location;   //reference to pos and color in shaders
        GLint M_location;     //Reference to matrix in shader
        GLuint mario_texture;
        GLuint mario_t_texture;
        GLuint left_texture;
        GLuint right_texture;
        GLuint left_t_texture;
        GLuint right_t_texture;
    } GLvars;


public:

    Mario();

    inline void start_jump(){ state.init_jump= true;}
    inline void is_jumping(){ state.jump_on= true;}
    inline void stop_jump() { state.jump_on= false; state.velocity = vec2(0.0, 0.0);}

    inline void jump_interpolate(vec4 extents, bool is_left_edge);

    inline void which_platform(){

        if(state.cur_location.y < -.67){
            state.platform_num = 0;
        }

        if(state.cur_location.y < -.39 && state.cur_location.y > -.65){
            state.platform_num = 1;
        }

        if(state.cur_location.y < -.15 && state.cur_location.y > -.39){
            state.platform_num = 2;
        }
        if(state.cur_location.y < .08 && state.cur_location.y > -.15){
            state.platform_num = 3;
        }
        if(state.cur_location.y < .36 && state.cur_location.y > .08){
            state.platform_num = 4;
        }
        if(state.cur_location.y > .36){
            state.platform_num = 777; // final platform
        }
    }

    // made to make mario fall if he is off the ledge.
    inline void send_to_platform(){

        if(state.platform_num == 1 && state.cur_location.x > 1.16){
            state.cur_location.y = -.85;
        }

        if(state.platform_num == 2 && state.cur_location.x < -1.18){
            state.cur_location.y = -.53;

        }
        if(state.platform_num == 3 && state.cur_location.x > 1.18){
            state.cur_location.y = -.3;
        }
        if(state.platform_num == 4 && state.cur_location.x < -1.18){
            state.cur_location.y = -.05;

        }

        if(state.platform_num == 777 && state.cur_location.x > 1.06){
            state.cur_location.y = .2;
        }



    }


    inline void ladder_pos(){
        // L1 start = x = -.75
        // L2 start = x = -.06
        // L3 = x = -1

        if(state.cur_location.y < -.67){
            state.platform_num = 0;
        }

        if(state.cur_location.y < -.39 && state.cur_location.y > -.65){
            state.platform_num = 1;
        }

        if(state.cur_location.y < -.15 && state.cur_location.y > -.39){
            state.platform_num = 2;
        }
        if(state.cur_location.y < .12 && state.cur_location.y > -.15){
            state.platform_num = 3;
        }
        if(state.cur_location.y < .36 && state.cur_location.y > .12){
            state.platform_num = 4;
        }
        if(state.cur_location.y > .36){
            state.platform_num = 777; // final platform
        }
    }


  inline void start_climb(){
      if(state.in_ladder_range == true){
          state.is_climbing = true;
          state.cur_location.y += .05;
      }
      else{
          stop_climb();
      }
  }

    inline void start_declimb(){
        if(state.in_ladder_range == true){
            state.is_climbing = true;
            state.cur_location.y -= .05;
        }
        else{
            stop_climb();
        }
    }

  inline void stop_climb() { state.is_climbing = false;}

  inline void in_ladder_range() { state.in_ladder_range= true;}
  inline void out_ladder_range() { state.in_ladder_range= false;}

  inline void rotateLeft() {
    state.turning = _TURN_LEFT;
    //state.angle+= M_PI/16;
    //mat4 R = RotateZ(state.angle);
    //state.pointing = R*vec4(0.0f, -1.0f, 0.0f, 0.0f);
  }

  inline void stopTurn(){
    state.turning = _NO_TURN;
  }

  inline void rotateRight(){
    state.turning = _TURN_RIGHT;
    //state.angle-= M_PI/16;
    //mat4 R = RotateZ(state.angle);
    //state.pointing = R*vec4(0.0f, -1.0f, 0.0f, 0.0f);
  }

  void update_state(vec4 extents);

  void gl_init();
  void gl_init_Debug();

  void draw(mat4 proj);
  void draw_Debug(mat4 proj);

};


#endif /* defined(__Donkey_Kong__Mario__) */
