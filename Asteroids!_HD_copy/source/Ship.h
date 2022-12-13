//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "Game.h"

using namespace tcg;

class Ship{
    
    friend class Game;
    
    std::vector < vec2 > ship_pos;
    std::vector < vec2 > ship_uv;
    
    std::vector<unsigned char> ship_im;
    unsigned ship_im_width, ship_im_height;
    
    vec2 ship_bbox[2];
    std::vector < vec2 > outline;
    
    std::vector<unsigned char> ship_t_im;
    unsigned ship_t_im_width, ship_t_im_height;
    
    std::vector<unsigned char> ship_left;
    unsigned s_left_width, s_left_height;
    
    vec2 left_bbox[2];
    std::vector < vec2 > outline_left;
    
    std::vector<unsigned char> ship_right;
    unsigned s_right_width, s_right_height;
    
    vec2 right_bbox[2];
    std::vector < vec2 > outline_right;
    
    std::vector<unsigned char> ship_left_t;
    unsigned s_left_t_width, s_left_t_height;
    
    std::vector<unsigned char> ship_right_t;
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
    
    
    //Ship State
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
    } state;
    
    //OpenGL variables for a ship
    struct {
        GLuint vao;           //Vertex array object
        GLuint program;       //shader program
        GLuint buffer;        //Vertex buffer objects
        GLuint vertex_shader, fragment_shader;  //Shaders
        GLint vpos_location, vtex_location;   //reference to pos and color in shaders
        GLint M_location;     //Reference to matrix in shader
        GLuint ship_texture;
        GLuint ship_t_texture;
        GLuint left_texture;
        GLuint right_texture;
        GLuint left_t_texture;
        GLuint right_t_texture;
    } GLvars;
    
    
public:
    
    Ship();
    
    inline void start_jump(){ state.init_jump= true;}
    inline void is_jumping(){ state.jump_on= true;}
    inline void stop_jump() { state.jump_on= false;}
    
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
    
    
  inline void start_climb(){
      if(state.in_ladder_range == true){
          state.is_climbing= true;
          state.cur_location.y += .05;
      }
  }
    
    inline void start_declimb(){
        if(state.in_ladder_range == true){
            state.cur_location.y -= .05;
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
  
  Bullet pew_pew(){
    return Bullet(state.cur_location,  state.velocity + 5.0f * accel * vec2(state.pointing.x, state.pointing.y));
  }
  
  void update_state(vec4 extents);
  
  void gl_init();
  void gl_init_Debug();
  
  void draw(mat4 proj);
  void draw_Debug(mat4 proj);
  
};


#endif /* defined(__Asteroids__Ship__) */
