//
//  Game.cpp
//  Donkey_Kong
//
//  Created by Luke Gulson, Chris Callahn, Mark George on 12/14/22.
//
// Contains test interactions, and gameover / BG logic

#include "Game.h"


bool Game::testIntersections(){
    // mario mins should be current
    // current position is the center, so mario min should be current location - 1/2 bbox, to go from center of mario to bottom left
    // mario max should be current location + 1/2 bbox to go from center of mario to top right.
    
    // since min starts at
    if (mario -> state.turning == Mario::_TURN_LEFT) {
        mario -> mario_bbox[0] = mario -> left_bbox[0];
        mario -> mario_bbox[1] = mario -> left_bbox[1];
    }
    if (mario -> state.turning == Mario::_TURN_RIGHT) { //
        mario -> mario_bbox[0] = mario -> right_bbox[0];
        mario -> mario_bbox[1] = mario -> right_bbox[1];
    }
    
    vec2 mario_mins = vec2(mario->state.cur_location.x + mario->mario_bbox[0].x, mario->state.cur_location.y +  mario->mario_bbox[0].y);
    vec2 mario_maxs = vec2(mario->state.cur_location.x + mario->mario_bbox[1].x, mario->state.cur_location.y +  mario->mario_bbox[1].y);
    
    vec2 barrel_1_mins = vec2(barrels[0]->state.cur_location.x + barrels[0]->barrel_bbox[0].x, barrels[0]->state.cur_location.y + barrels[0]->barrel_bbox[0].y);
    vec2 barrel_1_maxs = vec2(barrels[0]->state.cur_location.x + barrels[0]->barrel_bbox[1].x, barrels[0]->state.cur_location.y + barrels[0]->barrel_bbox[1].y);
    
    vec2 barrel_2_mins = vec2(barrels[1]->state.cur_location.x + barrels[1]->barrel_bbox[0].x, barrels[1]->state.cur_location.y + barrels[1]->barrel_bbox[0].y);
    vec2 barrel_2_maxs = vec2(barrels[1]->state.cur_location.x + barrels[1]->barrel_bbox[1].x, barrels[1]->state.cur_location.y + barrels[1]->barrel_bbox[1].y);

    bool x_intersecting1 = false;
    bool y_intersecting1 = false;
    bool x_intersecting2 = false;
    bool y_intersecting2 = false;
    
    if  (mario_maxs.x >= barrel_1_mins.x)   {
        if  (mario_mins.x <= barrel_1_maxs.x)  {
            x_intersecting1 = true;
        }
    }
    if  (mario_maxs.y >= barrel_1_mins.y)   {
        if  (mario_mins.y <= barrel_1_maxs.y)  {
            y_intersecting1 = true;
        }
    }
    
    if  (mario_maxs.x >= barrel_2_mins.x)   {
        if  (mario_mins.x <= barrel_2_maxs.x)  {
            x_intersecting2 = true;
        }
    }
    if  (mario_maxs.y >= barrel_2_mins.y)   {
        if  (mario_mins.y <= barrel_2_maxs.y)  {
            y_intersecting2 = true;
        }
    }
    
    
    if ((x_intersecting1 && y_intersecting1) || ((x_intersecting2 && y_intersecting2)))   {
        std::cout << mario->mario_bbox[0].x  <<std::endl;
        std::cout << barrel_1_mins  <<std::endl;
        return true;
    }
    else    {
        return false; 
    }
        
}

Game::Game(){
  mario = new Mario();
  barrels.resize(2);
  barrels[0] = new Barrel(1);
  barrels[1] = new Barrel(2);
  game_over = false;
    
  is_alive = true;
  
    std::string file_location = source_path + "sprites/background_better.png";
  unsigned error = lodepng::decode(game_over_im, go_width, go_height, file_location.c_str());
  std::cout << go_width << " X " << go_height << " game image loaded\n";
  
};


void Game::gl_init(){
  
  std::vector <vec2> pos(4);
  std::vector <vec2> uv(4);
  
  pos[0] = vec2(screen_extents[0],  screen_extents[3]);
  pos[1] = vec2(screen_extents[0],  screen_extents[2]);
  pos[2] = vec2(screen_extents[1],  screen_extents[3]);
  pos[3] = vec2(screen_extents[1],  screen_extents[2]);
  
  uv[0] = vec2(0.0,0.0);
  uv[1] = vec2(0.0,1.0);
  uv[2] = vec2(1.0,0.0);
  uv[3] = vec2(1.0,1.0);
  

  unsigned int vert_size = pos.size()*sizeof(vec2);
  unsigned int uv_size = uv.size()*sizeof(vec2);

  std::string vshader = source_path + "shaders/vshader_Texture.glsl";
  std::string fshader = source_path + "shaders/fshader_Texture.glsl";

  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  GOGLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GOGLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GOGLvars.vertex_shader);
  check_shader_compilation(vshader, GOGLvars.vertex_shader);

  GOGLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GOGLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GOGLvars.fragment_shader);
  check_shader_compilation(fshader, GOGLvars.fragment_shader);

  GOGLvars.program = glCreateProgram();
  glAttachShader(GOGLvars.program, GOGLvars.vertex_shader);
  glAttachShader(GOGLvars.program, GOGLvars.fragment_shader);

  glLinkProgram(GOGLvars.program);
  check_program_link(GOGLvars.program);
  glUseProgram(GOGLvars.program);
  
  glBindFragDataLocation(GOGLvars.program, 0, "fragColor");

  GOGLvars.vpos_location   = glGetAttribLocation(GOGLvars.program, "vPos");
  GOGLvars.vtex_location = glGetAttribLocation(GOGLvars.program, "vTexCoord" );
  GOGLvars.M_location = glGetUniformLocation(GOGLvars.program, "M" );
  
  glGenTextures( 1, &GOGLvars.texture );

  glBindTexture( GL_TEXTURE_2D, GOGLvars.texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, go_width, go_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &game_over_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );


  // Create a vertex array object
  glGenVertexArrays( 1, &GOGLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GOGLvars.vao );

  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GOGLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GOGLvars.buffer );

  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, vert_size + uv_size, NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, vert_size, &pos[0] );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, vert_size, uv_size, &uv[0] );

  glEnableVertexAttribArray(GOGLvars.vpos_location);
  glEnableVertexAttribArray(GOGLvars.vtex_location );

  glVertexAttribPointer( GOGLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GOGLvars.vtex_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vert_size) );

  glBindVertexArray(0);
  
}

void Game::draw_game_over(mat4 proj){
    mario->state.cur_location.x = -1.0;
    mario->state.cur_location.y = -.85;
  
  glUseProgram(GOGLvars.program);
  glBindVertexArray( GOGLvars.vao );
  glBindBuffer( GL_ARRAY_BUFFER, GOGLvars.buffer );
  
  glUniformMatrix4fv( GOGLvars.M_location, 1, GL_TRUE, proj);
  
  glBindTexture( GL_TEXTURE_2D, GOGLvars.texture  );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  
  glBindVertexArray(0);
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  
}

void Game::draw_alive_bg(mat4 proj){
  
  glUseProgram(GOGLvars.program);
  glBindVertexArray( GOGLvars.vao );
  glBindBuffer( GL_ARRAY_BUFFER, GOGLvars.buffer );
  
  glUniformMatrix4fv( GOGLvars.M_location, 1, GL_TRUE, proj);
  
  glBindTexture( GL_TEXTURE_2D, GOGLvars.texture  );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  
  glBindVertexArray(0);
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  
}
