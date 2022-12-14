//
//  Mario.cpp
//  Donkey_Kong
//
//  Created by Luke Gulson, Chris Callahn, Mark George on 12/14/22.
//
//

#include "Game.h"

#define _MAX_SPEED 0.05f
#define _DAMPING 0.98f

Mario::Mario(){
  state.cur_location = vec2(0.0f,0.0f);
  state.pointing = vec4(0.0f,-1.0f,0.0f,0.0f);
  state.velocity = vec2(0.0f,0.0f);
  state.jump_on = false;
  state.turning = _NO_TURN;
  state.angle = 0.0;

  max_speed = 0.025;
  damping_fact = 0.7;
    grav = 0.002;

  mario_pos.resize(16);
  mario_uv.resize(16);

  //Mario
  mario_pos[0] = vec2(-0.075, -0.075);     mario_uv[0] = vec2(0.0,0.0);
  mario_pos[1] = vec2(-0.075,  0.075);     mario_uv[1] = vec2(0.0,1.0);
  mario_pos[2] = vec2(0.075,  -0.075);     mario_uv[2] = vec2(1.0,0.0);
  mario_pos[3] = vec2(0.075,   0.075);     mario_uv[3] = vec2(1.0,1.0);

  //Min and max coordinates
  mario_bbox[0] = mario_pos[0];
  mario_bbox[1] = mario_pos[3];

  //Mario with thruster (because the image gets bigger 39px -> 43px)
  mario_pos[4] = vec2(-0.075, -0.075);     mario_uv[4] = vec2(0.0,0.0);
  mario_pos[5] = vec2(-0.075,  0.0903846);     mario_uv[5] = vec2(0.0,1.0);
  mario_pos[6] = vec2(0.075,  -0.075);     mario_uv[6] = vec2(1.0,0.0);
  mario_pos[7] = vec2(0.075,   0.0903846);     mario_uv[7] = vec2(1.0,1.0);

  //left or right turn (skinnier)
  mario_pos[8] = vec2(-0.057692, -0.075);     mario_uv[8] = vec2(0.0,0.0);
  mario_pos[9] = vec2(-0.057692,  0.075);     mario_uv[9] = vec2(0.0,1.0);
  mario_pos[10] = vec2(0.057692,  -0.075);     mario_uv[10] = vec2(1.0,0.0);
  mario_pos[11] = vec2(0.057692,   0.075);     mario_uv[11] = vec2(1.0,1.0);

  left_bbox[0] = mario_pos[8];
  left_bbox[1] = mario_pos[11];

  //left or right turn with thruster
  mario_pos[12] = vec2(-0.057692, -0.075);     mario_uv[12] = vec2(0.0,0.0);
  mario_pos[13] = vec2(-0.057692,  0.0903846);     mario_uv[13] = vec2(0.0,1.0);
  mario_pos[14] = vec2(0.057692,  -0.075);     mario_uv[14] = vec2(1.0,0.0);
  mario_pos[15] = vec2(0.057692,   0.0903846);     mario_uv[15] = vec2(1.0,1.0);

  right_bbox[0] = mario_pos[12];
  right_bbox[1] = mario_pos[15];

  std::string file_location = source_path + "sprites/mario_stationary_right.png";
  unsigned error = lodepng::decode(mario_im, mario_im_width, mario_im_height, file_location.c_str());
  std::cout << mario_im_width << " X " << mario_im_height << " image loaded\n";

  for(unsigned int i=0; i < mario_im_width*mario_im_height; i++){
    unsigned int x = i%mario_im_width;
    unsigned int y = i/mario_im_width;

    bool neighbor_alpha = false;
    if(mario_im[4*i+3] != 0){
      if(x > 0){
        if(mario_im[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(x < mario_im_width-1){
        if(mario_im[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y > 0){
        if(mario_im[4*(i-mario_im_width)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y < mario_im_height-1){
        if(mario_im[4*(i+mario_im_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }

    if(neighbor_alpha){
      outline.push_back(vec2((float)x/(float)mario_im_width,(float)y/(float)mario_im_height));
    }
  }

  for(unsigned int i=0; i < outline.size(); i++){
    outline[i].x =  mario_bbox[0].x + outline[i].x*(mario_bbox[1].x-mario_bbox[0].x);
    outline[i].y =  mario_bbox[0].y + outline[i].y*(mario_bbox[1].y-mario_bbox[0].y);
  }

  file_location = source_path + "sprites/mario_jumping_right.png";
  error = lodepng::decode(mario_t_im, mario_t_im_width, mario_t_im_height, file_location.c_str());
  std::cout << mario_t_im_width << " X " << mario_t_im_height << " image loaded\n";

  file_location = source_path + "sprites/mario_running_right.png";
  error = lodepng::decode(mario_left, s_left_width, s_left_height, file_location.c_str());
  std::cout << s_left_width << " X " << s_left_height << " image loaded\n";

  for(unsigned int i=0; i < s_left_width*s_left_height; i++){
    unsigned int x = i%s_left_width;
    unsigned int y = i/s_left_width;

    bool neighbor_alpha = false;
    if(mario_left[4*i+3] != 0){
      if(x > 0){
        if(mario_left[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(x < s_left_width-1){
        if(mario_left[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y > 0){
        if(mario_left[4*(i-s_left_width)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y < s_left_height-1){
        if(mario_left[4*(i+s_left_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }

    if(neighbor_alpha){
      outline_left.push_back(vec2((float)x/(float)s_left_width,(float)y/(float)s_left_height));
    }
  }
  for(unsigned int i=0; i < outline_left.size(); i++){
    outline_left[i].x =  left_bbox[0].x + outline_left[i].x*(left_bbox[1].x-left_bbox[0].x);
    outline_left[i].y =  left_bbox[0].y + outline_left[i].y*(left_bbox[1].y-left_bbox[0].y);
  }

  file_location = source_path + "sprites/mario_running_left.png";
  error = lodepng::decode(mario_right, s_right_width, s_right_height, file_location.c_str());
  std::cout << s_right_width << " X " << s_right_height << " image loaded\n";

  for(unsigned int i=0; i < s_right_width*s_right_height; i++){
    unsigned int x = i%s_right_width;
    unsigned int y = i/s_right_width;

    bool neighbor_alpha = false;
    if(mario_right[4*i+3] != 0){
      if(x > 0){
        if(mario_right[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(x < s_right_width-1){
        if(mario_right[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y > 0){
        if(mario_right[4*(i-s_right_width)+3] == 0){
          neighbor_alpha = true;
        }
      }

      if(y < s_right_height-1){
        if(mario_right[4*(i+s_right_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }

    if(neighbor_alpha){
      outline_right.push_back(vec2((float)x/(float)s_right_width,(float)y/(float)s_right_height));
    }
  }

  for(unsigned int i=0; i < outline_right.size(); i++){
    outline_right[i].x =  right_bbox[0].x + outline_right[i].x*(right_bbox[1].x-right_bbox[0].x);
    outline_right[i].y =  right_bbox[0].y + outline_right[i].y*(right_bbox[1].y-right_bbox[0].y);
  }

  file_location = source_path + "sprites/mario_jumping_right.png";
  error = lodepng::decode(mario_left_t, s_left_t_width, s_left_t_height, file_location.c_str());
  std::cout << s_left_t_width << " X " << s_left_t_height << " image loaded\n";

  file_location = source_path + "sprites/mario_jumping_left.png";
  error = lodepng::decode(mario_right_t, s_right_t_width, s_right_t_height, file_location.c_str());
  std::cout << s_right_t_width << " X " << s_right_t_height << " image loaded\n";


};

void Mario::jump_interpolate(vec4 extents, bool is_left_edge){
    std::cout << "current y when jump_interpolate" << std::endl;
    std::cout << state.cur_location.y << std::endl;
    if (is_left_edge) {
        if (state.cur_location.y >= (155.0 / 383.0)) {
            state.cur_location.y = (155.0 / 383.0);
        }
        else if (state.cur_location.y >= (-17.0 / 383.0)) {
            state.cur_location.y = (-17.0 / 383.0);
        }
        else if (state.cur_location.y >= (-214.0 / 383.0)) {
            state.cur_location.y = (-214.0 / 383.0);
        }
        else {
            state.cur_location.y = -0.82;
        }
        state.cur_location.x = extents[0] + 0.075;
    }
    else {
        if (state.cur_location.y >= (80.0 / 383.0)) {
            state.cur_location.y = (80.0 / 383.0);
        }
        else if (state.cur_location.y >= (-117.0 / 383.0)) {
            state.cur_location.y = (-117.0 / 383.0);
        }
        else {
            state.cur_location.y = -0.82;
        }
        state.cur_location.x = extents[1] - 0.075;
    }
    stop_jump();
}

// TODO
// Include distance from platform for mario to impact jump where if mario is 2 units away from platform subtract from velocity until mario y = platform y
// Make platform y = platform heights + step formula
// Lock movement when jumping
//
void Mario::update_state(vec4 extents){
    
    // Mario's movement on first platform
    // since start platform is flat, movement logic works (number 0 platform
    which_platform();
    send_to_platform();
    
    float ramp_pts[14] = {};
    float ramp_unit = (extents[1] - extents[0]) / 14;
    float min_bound;
    float max_bound;
    
    for (int i = 0; i < 14; i++) {
        ramp_pts[i] = extents[0] + (i * ramp_unit);
    }
    
    if (state.jump_on) {
        
        if (state.cur_location.x <= (extents[0] + 0.075)) {
            jump_interpolate(extents, true);
        }
        else if (state.cur_location.x >= (extents[1] - 0.075)) {
            jump_interpolate(extents, false);
        }
        
        else {
            if (state.parity < 0) {
                
                if (state.cur_location.x <= state.x_end) {
                    stop_jump();
                }
            }
            
            if (state.parity > 0){
                if (state.cur_location.x >= state.x_end) {
                    stop_jump();
                }
            }
            else {
                if ((state.velocity.y < 0) && (state.cur_location.y <= state.y_end)) {
                    state.cur_location.y = state.y_end;
                    stop_jump();
                }
            }
            
            state.cur_location += state.velocity;
            state.velocity.y -= grav;
            
        }
    }
    
    else if (state.init_jump) {
        if (state.turning == _TURN_LEFT){
            state.parity = -1;
        }
        else if (state.turning == _TURN_RIGHT){
            state.parity = 1;
        }
        else {
            state.parity = 0;
        }
        if (state.parity != 0) {
            state.velocity = 0.03 * normalize(vec2(state.parity * 1.0, 1.0));
        }
        else {
            state.velocity = 0.022 * normalize(vec2(0.0, 1.0));
        }
        
        if (is_start_platform){
            state.x_end = state.cur_location.x + (state.parity * ((2 * state.velocity.x * state.velocity.x) / grav));
            state.y_end = state.cur_location.y;
        }
        
        state.jump_on = true;
        state.init_jump = false;
    }
    else {
        // Mario's movement on first platform
        // since start platform is flat, movement logic works (number 0 platform
        which_platform();

        if(is_start){
            state.cur_location = vec2(-1.0, -.85);
            is_start = false;
        }

        if(state.platform_num == 0){
            // Moves sprite left
            if(state.turning == _TURN_LEFT){
                std::cout << state.cur_location << std::cout;
                state.velocity.x -= .15;
                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            // Moves sprite right
            if(state.turning == _TURN_RIGHT){
                std::cout << state.cur_location << std::cout;
                state.velocity += .15 * vec2(1.0, 0.0);
                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }
            // first ladder
            if(state.cur_location.x > .75 && state.cur_location.x < 1.0 && state.cur_location.y > -.92 && state.cur_location.y < -.67){
                state.in_ladder_range = true;
            }
            else{
                state.in_ladder_range = false;
            }
        }

        // movement logic for odd platforms
        // platform 1, 3
        // if odd platform
        if(state.platform_num == 1 || state.platform_num == 3){
            // Moves sprite left
            if(state.turning == _TURN_LEFT){
                std::cout << state.cur_location << std::cout;
                state.velocity -= .15 * vec2(1.0, 0.0);

                // diagonal upwards movement
                state.velocity.y += .007;
                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            // Moves sprite right
            if(state.turning == _TURN_RIGHT){
                std::cout << state.cur_location << std::cout;
                state.velocity += .15 * vec2(1.0, 0.0);
                // diagonal downward movement

                state.velocity -= .007 * vec2(0.0, 1.0);

                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            // second + third ladders
            if((state.cur_location.x > -1.0 && state.cur_location.x < -.75) || (state.cur_location.x > -.18 && state.cur_location.x < .07) || (state.cur_location.x > -.57 && state.cur_location.x < -.32))
            {
                state.in_ladder_range = true;
            }
            else{
                state.in_ladder_range = false;
            }

            // Make it so all other movement is locked. (no turning, no changing directions)
            if(state.jump_on == true){
                state.velocity += .15 * vec2(0.0, 1.0);
            }

        }
        // if even platform
        if(state.platform_num == 2 || state.platform_num == 4){
            // Moves sprite left
            if(state.turning == _TURN_LEFT){
                std::cout << state.cur_location << std::cout;
                state.velocity -= .15 * vec2(1.0, 0.0);

                // diagonal upwards movement
                state.velocity.y -= .007;
                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            // Moves sprite right
            if(state.turning == _TURN_RIGHT){
                std::cout << state.cur_location << std::cout;
                state.velocity += .15 * vec2(1.0, 0.0);
                // diagonal downward movement

                state.velocity += .007 * vec2(0.0, 1.0);

                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            if((state.cur_location.x > .75 && state.cur_location.x < 1) || (state.cur_location.x > 0.05 && state.cur_location.x < .2) || (state.cur_location.x > 0.77 && state.cur_location.x < 0.92))
            {
                state.in_ladder_range = true;
            }
            else{
                state.in_ladder_range = false;
            }

            // Make it so all other movement is locked. (no turning, no changing directions)
            if(state.jump_on == true){
                state.velocity += .15 * vec2(0.0, 1.0);
            }

        }

        // if final platform
        if(state.platform_num == 777){
            // Moves sprite left
            if(state.turning == _TURN_LEFT){
                std::cout << state.cur_location << std::cout;
                state.velocity -= .15 * vec2(1.0, 0.0);

                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }

            // Moves sprite right
            if(state.turning == _TURN_RIGHT){
                std::cout << state.cur_location << std::cout;
                state.velocity += .15 * vec2(1.0, 0.0);
                // diagonal downward movement

                if(length(state.velocity) > max_speed){
                    state.velocity = normalize(state.velocity);
                    state.velocity*=max_speed;
                }
            }
            // TODO
            //  final 3 ladders
            if((state.cur_location.x > .05 && state.cur_location.x < 0.3) ||
               (state.cur_location.x > -.4 && state.cur_location.x < -.25) ||
               (state.cur_location.x > -.68 && state.cur_location.x < -.38))
            {
                state.in_ladder_range = true;
            }
            else{
                state.in_ladder_range = false;
            }
        }

        // Maybe delete damping effect
        state.velocity*= damping_fact;
        state.cur_location+=state.velocity;
        if (state.cur_location.x <= (extents[0] + 0.075)) {
            state.cur_location.x = extents[0] + 0.075;
            state.velocity.x = 0.0;
        }
        else if (state.cur_location.x >= (extents[1] - 0.075)){
            state.cur_location.x = (extents[1] - 0.075);
            state.velocity.x = 0.0;
        }

    }

    if(state.cur_location.y < extents[2] ||state.cur_location.y > extents[3]){
        state.cur_location.y = -state.cur_location.y;
    }

    if (state.turning == _TURN_LEFT) {
        float numb = .2;
        float old_numb = 0.057692;
        
        left_bbox[0] = vec2(state.cur_location.x - numb, state.cur_location.y - numb);
        left_bbox[1] = vec2(state.cur_location.x + numb, state.cur_location.y + numb);
    }
    else if (state.turning == _TURN_RIGHT) {
        right_bbox[0] = vec2(state.cur_location.x - 0.057692, state.cur_location.y - 0.075);
        right_bbox[1] = vec2(state.cur_location.x + 0.057692, state.cur_location.y +  0.0903846);

    }
    else{
        mario_bbox[0] = vec2(state.cur_location.x - 0.075, state.cur_location.y - 0.075);
        mario_bbox[1] = vec2(state.cur_location.x + 0.075, state.cur_location.y + 0.075);
    }

}


void Mario::gl_init(){

  unsigned int mario_vert_size = mario_pos.size()*sizeof(vec2);
  unsigned int mario_uv_size = mario_uv.size()*sizeof(vec2);

  std::string vshader = source_path + "shaders/vshader_Texture.glsl";
  std::string fshader = source_path + "shaders/fshader_Texture.glsl";

  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GLvars.vertex_shader);
  check_shader_compilation(vshader, GLvars.vertex_shader);

  GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GLvars.fragment_shader);
  check_shader_compilation(fshader, GLvars.fragment_shader);

  GLvars.program = glCreateProgram();
  glAttachShader(GLvars.program, GLvars.vertex_shader);
  glAttachShader(GLvars.program, GLvars.fragment_shader);

  glLinkProgram(GLvars.program);
  check_program_link(GLvars.program);
  glUseProgram(GLvars.program);

  glBindFragDataLocation(GLvars.program, 0, "fragColor");

  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.vtex_location = glGetAttribLocation(GLvars.program, "vTexCoord" );
  GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );

  glGenTextures( 1, &GLvars.mario_texture );
  glGenTextures( 1, &GLvars.mario_t_texture );
  glGenTextures( 1, &GLvars.left_texture );
  glGenTextures( 1, &GLvars.right_texture );
  glGenTextures( 1, &GLvars.left_t_texture );
  glGenTextures( 1, &GLvars.right_t_texture );

  glBindTexture( GL_TEXTURE_2D, GLvars.mario_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mario_im_width, mario_im_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.mario_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mario_t_im_width, mario_t_im_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_t_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.left_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_left_width, s_left_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_left[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.right_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_right_width, s_right_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_right[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.left_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_left_t_width, s_left_t_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_left_t[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.right_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_right_t_width, s_right_t_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &mario_right_t[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );

  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );

  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, mario_vert_size + mario_uv_size, NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, mario_vert_size, &mario_pos[0] );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, mario_vert_size, mario_uv_size, &mario_uv[0] );

  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vtex_location );

  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vtex_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mario_vert_size) );

  glBindVertexArray(0);

}


void Mario::draw(tcg::mat4 Projection){
  glUseProgram( GLvars.program );
  glBindVertexArray( GLvars.vao );

  mat4 ModelView = Translate(state.cur_location.x, state.cur_location.y, 0);


  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, Projection*ModelView);

  if(!state.jump_on && state.turning == _NO_TURN){
    glBindTexture( GL_TEXTURE_2D, GLvars.mario_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  }

  if(state.jump_on && state.turning == _NO_TURN){
    glBindTexture( GL_TEXTURE_2D, GLvars.mario_t_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
  }

  if(!state.jump_on && state.turning == _TURN_LEFT){
    glBindTexture( GL_TEXTURE_2D, GLvars.left_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
  }

  if(!state.jump_on && state.turning == _TURN_RIGHT){
    glBindTexture( GL_TEXTURE_2D, GLvars.right_texture  );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
  }

  if(state.jump_on && state.turning == _TURN_LEFT){
    glBindTexture( GL_TEXTURE_2D, GLvars.left_t_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
  }

  if(state.jump_on && state.turning == _TURN_RIGHT){
    glBindTexture( GL_TEXTURE_2D, GLvars.right_t_texture  );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
  }

  glBindVertexArray(0);
  glUseProgram(0);

}
