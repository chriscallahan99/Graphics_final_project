//
//  Game.h
//  Donkey_Kong
//
//  Created by Luke Gulson, Chris Callahn, Mark George on 12/14/22.
//
//

#ifndef Donkey_Kong__Game_h
#define Donkey_Kong__Game_h

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <list>
#include <vector>
#include <utils/shader_process.h>
#include <path/path.h>

#include <utils/vec.h>
#include <utils/mat.h>
#include <utils/lodepng.h>
#include <utils/loadTexture.h>


#include "Mario.h"
#include "Barrel.h"

class Game{

public:
  Mario *mario;
  std::vector <Barrel *> barrels;

  //left right bottom top of window
  vec4 screen_extents;
  
private:
  
  bool game_over;
  bool is_alive;
  
  std::vector<unsigned char> game_over_im;
  unsigned go_width, go_height;

  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vtex_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
    GLuint texture;
  } GOGLvars;

  
public:
  Game();
  
  ~Game(){
    delete mario;
    barrels.clear();
  }
  
  void init(){
    for(unsigned int i=0; i < barrels.size(); i++){
      barrels[i]->gl_init();
    }
    mario->gl_init();
    gl_init();
  }
  
  void draw(mat4 proj){
      if(is_alive){
          draw_alive_bg(proj);
      }
      mario->draw(proj);
    for(unsigned int i=0; i < barrels.size(); i++){
      barrels[i]->draw(proj);
    }
    
    if(game_over){
      draw_game_over(proj);
    }
  }
  
  void update(){
    for(unsigned int i=0; i < barrels.size(); i++){
      barrels[i]->update_state(screen_extents);
    }
    mario->update_state(screen_extents);
    
    game_over = testIntersections();
  }
  
private:
  
  void gl_init();
  void draw_game_over(mat4 proj);
  void draw_alive_bg(mat4 proj);
  
  bool testIntersections();
  
};


#endif
