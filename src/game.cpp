
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <game.hpp>
#include <iostream>
#include <string>

void Game::run() {
  initialize();
  Uint64 current_time, last_time = SDL_GetPerformanceCounter();
  delta_time_ = 0;
  load_bckground();
  play_game_start_audio();
  while (running_) {
    current_time = SDL_GetPerformanceCounter();

    while (in_menu_) {
      render_menu();
      SDL_RenderPresent(renderer_);
      handle_menu_input();

      current_time = SDL_GetPerformanceCounter();
      last_time = current_time;
    }

    delta_time_ = calculate_delta_time(current_time, last_time);
    handle_input();
    render();

    SDL_RenderPresent(renderer_);

    if (ball_.hit_bottom()) {
      game_over();
      current_time = SDL_GetPerformanceCounter();
    }
    if (check_win()) current_time = SDL_GetPerformanceCounter();
    last_time = current_time;
  }
  quit();
}

void Game::render() {
  SDL_RenderClear(renderer_);

  SDL_RenderCopy(renderer_, background_texture_, NULL, NULL);

  render_score();
  for (auto brick : bricks_) brick.render();
  paddle_.render();
  ball_.render();
  ball_.move(delta_time_);
}
bool Game::check_win() {
  bool win = true;
  int brick_counter = 0;
  for (auto& brick : bricks_) {
    if (brick.is_alive()) {
      win = false;
      brick_counter++;
    }
  }
  if (brick_counter < bricks_alive_) {
    bricks_alive_ = brick_counter;
    play_ball_hit_audio();
  }
  if (win) {
    game_won();
    return true;
  }
  return false;
}

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return;
  }
  if (!init_audio()) {
    return;
  }

  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    std::cerr << "Failed to initialize SDL_image: " << IMG_GetError()
              << std::endl;
    quit();
    return;
  }
  window_ = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, window_width_,
                             window_height_, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (window_ == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    quit();
    return;
  }

  renderer_ = SDL_CreateRenderer(  // add error handling
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == NULL) {
    std::cerr << "Renderer could not be created!" << SDL_GetError()
              << std::endl;
    quit();
    return;
  }

  if (TTF_Init() == -1) {
    std::cerr << "TTF_Init:" << TTF_GetError() << std::endl;
    quit();
    return;
  }
  font_ = TTF_OpenFont("assets/fonts/silkscreen.ttf", 20);
  if (!font_) {
    std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
    quit();
    return;
  }

  paddle_ = Paddle(window_width_, window_height_, renderer_);
  initialize_bricks();
  Brick::load_texture(renderer_);
  ball_ = Ball(window_width_, window_height_, &paddle_, &bricks_, renderer_);
  running_ = true;
}

void Game::handle_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
      SDL_SetWindowFullscreen(window_, 0);
      in_menu_ = true;
    }
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
      {
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
      }
    }
  }
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);
  if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
    paddle_.move(Direction::left, delta_time_);
  }
  if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
    paddle_.move(Direction::right, delta_time_);
  }
  if (keystate[SDL_SCANCODE_ESCAPE]) in_menu_ = !in_menu_;
}

void Game::initialize_bricks() {
  for (auto& brick : bricks_) {
    brick = Brick(renderer_, 0, 0, 0);
  }
  load_level(1);
  level_ = 1;
}

void Game::render_menu() {
  SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, background_texture_, NULL, NULL);
  std::string a;

  a = "BRICK BREAKER";
  display_text(a, window_width_ / 2, window_height_ / 4, 80, {255, 255, 255},
               true);
  if (!in_levels_menu_) {
    if (selected_menu_item_ == 0)
      a = !game_in_progress_ ? "> START GAME <" : "> RESUME GAME <";
    else
      a = !game_in_progress_ ? "START GAME" : "RESUME GAME";
    display_text(a, window_width_ / 2, window_height_ / 2, 40, {255, 255, 255},
                 true);

    if (selected_menu_item_ == 1)
      a = "> EXIT GAME <";
    else
      a = "EXIT GAME";
    display_text(a, window_width_ / 2, window_height_ / 2 + 50, 40,
                 {255, 255, 255}, true);

    if (selected_menu_item_ == 2)
      a = "> LEVELS <";
    else
      a = "LEVELS";
    display_text(a, window_width_ / 2, window_height_ / 2 + 100, 40,
                 {255, 255, 255}, true);

    if (game_in_progress_) {
      if (selected_menu_item_ == 3)
        a = "> RESET GAME <";
      else
        a = "RESET GAME";
      display_text(a, window_width_ / 2, window_height_ / 2 + 150, 40,
                   {255, 255, 255}, true);
    }

  } else {
    if (selected_menu_item_ == 0)
      a = "> LEVEL 1 <";
    else
      a = "LEVEL 1";
    display_text(a, window_width_ / 2, window_height_ / 2, 40, {255, 255, 255},
                 true);
    if (selected_menu_item_ == 1)
      a = "> LEVEL 2 <";
    else
      a = "LEVEL 2";
    display_text(a, window_width_ / 2, window_height_ / 2 + 50, 40,
                 {255, 255, 255}, true);
    if (selected_menu_item_ == 2)
      a = "> LEVEL 3 <";
    else
      a = "LEVEL 3";
    display_text(a, window_width_ / 2, window_height_ / 2 + 100, 40,
                 {255, 255, 255}, true);
    if (selected_menu_item_ == 3)
      a = "> LEVEL 4 <";
    else
      a = "LEVEL 4";
    display_text(a, window_width_ / 2, window_height_ / 2 + 150, 40,
                 {255, 255, 255}, true);
    if (selected_menu_item_ == 4)
      a = "> BACK <";
    else
      a = "BACK";
    display_text(a, window_width_ / 2, window_height_ / 2 + 200, 40,
                 {255, 255, 255}, true);
  }
  a = "- Berina Majdancic -";
  display_text(a, window_width_ / 2, window_height_ - 50, 30, {255, 255, 255},
               true);
}

void Game::handle_menu_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running_ = false;
    }
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
      SDL_SetWindowFullscreen(window_, 0);
    }
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
      {
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
      }
    }
    if (!in_levels_menu_) {
      if (event.type == SDL_KEYDOWN &&
          (event.key.keysym.scancode == SDL_SCANCODE_DOWN ||
           event.key.keysym.scancode == SDL_SCANCODE_S)) {
        if (selected_menu_item_ + 1 <= 3) {
          selected_menu_item_++;
          if (!game_in_progress_ && selected_menu_item_ == 3)
            selected_menu_item_ = 2;
        }
      } else if (event.type == SDL_KEYDOWN &&
                 (event.key.keysym.scancode == SDL_SCANCODE_UP ||
                  event.key.keysym.scancode == SDL_SCANCODE_W)) {
        if (selected_menu_item_ - 1 >= 0) selected_menu_item_--;
      }
    } else {
      if (event.type == SDL_KEYDOWN &&
          (event.key.keysym.scancode == SDL_SCANCODE_DOWN ||
           event.key.keysym.scancode == SDL_SCANCODE_S)) {
        if (selected_menu_item_ + 1 <= 4) {
          selected_menu_item_++;
        }
      } else if (event.type == SDL_KEYDOWN &&
                 (event.key.keysym.scancode == SDL_SCANCODE_UP ||
                  event.key.keysym.scancode == SDL_SCANCODE_W)) {
        if (selected_menu_item_ - 1 >= 0) selected_menu_item_--;
      }
    }

    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
      if (!in_levels_menu_) {
        switch (selected_menu_item_) {
          case 0:  // Start / Resume
            in_menu_ = false;
            game_in_progress_ = true;
            break;
          case 1:  // Exit
            in_menu_ = false;
            running_ = false;
            break;
          case 2:  // Levels
            in_levels_menu_ = true;
            selected_menu_item_ = 0;
            break;
          case 3:  // Reset
            in_menu_ = false;
            game_reset();
            break;
        }
      } else {
        switch (selected_menu_item_) {
          case 0:  // Level 1
            in_menu_ = false;
            in_levels_menu_ = false;
            game_in_progress_ = true;
            level_ = 1;
            game_reset();

            break;
          case 1:  // Level 2
            in_menu_ = false;
            in_levels_menu_ = false;
            game_in_progress_ = true;
            level_ = 2;
            game_reset();
            break;
          case 2:  // Level 3
            in_menu_ = false;
            in_levels_menu_ = false;
            game_in_progress_ = true;
            level_ = 3;
            game_reset();
            break;
          case 3:  // Level 4
            in_menu_ = false;
            in_levels_menu_ = false;
            game_in_progress_ = true;
            level_ = 4;
            game_reset();
            break;
          case 4:  // Back
            in_levels_menu_ = false;
            break;
        }
        selected_menu_item_ = 0;
      }
    }
  }
}

void Game::display_text(const std::string& text, int x, int y, int size,
                        SDL_Color color, bool centered) {
  if (font_) TTF_CloseFont(font_);
  font_ = TTF_OpenFont("assets/fonts/silkscreen.ttf", size);
  if (!font_) {
    std::cerr << "Failed to load font with new size: " << TTF_GetError()
              << std::endl;
  }
  SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), color);
  int text_width, text_height;
  if (TTF_SizeText(font_, text.c_str(), &text_width, &text_height) != 0)
    std::cerr << "TTF_SizeText Error: " << TTF_GetError() << std::endl;
  if (centered) x = x - (text_width / 2);
  SDL_Rect text_rect = {x, y, text_width, text_height};
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer_, texture, NULL, &text_rect);
  SDL_DestroyTexture(texture);
}

void Game::load_bckground() {
  SDL_Surface* surface_img = IMG_Load("assets/images/background.jpg");
  if (!surface_img) {
    std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    return;
  }
  background_texture_ = SDL_CreateTextureFromSurface(renderer_, surface_img);
  SDL_FreeSurface(surface_img);
  if (!background_texture_) {
    std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    return;
  }
}

void Game::render_score() {
  std::string a = "Score: " + std::to_string(ball_.get_score());
  display_text(a, 30, window_height_ - 30, 24, {255, 255, 255, 255}, false);
}

double Game::calculate_delta_time(Uint64 current_time, Uint64 last_time) {
  double delta_time = (double)((current_time - last_time) * 1000 /
                               (double)SDL_GetPerformanceFrequency());
  if (delta_time < 6.94) {
    SDL_Delay(static_cast<Uint32>(6.94 - delta_time));
  }
  delta_time = (double)((current_time - last_time) * 1000 /
                        (double)SDL_GetPerformanceFrequency());
  return delta_time;
}

void Game::quit() {
  if (!window_)
    SDL_DestroyWindow(window_);
  else if (font_)
    TTF_CloseFont(font_);
  SDL_FreeWAV(audio_buffer_);
  SDL_CloseAudio();
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void Game::game_reset() {
  delta_time_ = calculate_delta_time(SDL_GetPerformanceCounter(),
                                     SDL_GetPerformanceCounter());

  bricks_alive_ = NUM_OF_BRICKS;
  ball_.reset();
  paddle_.reset();
  for (auto& brick : bricks_) brick.reset();
  load_level(level_);
  render();
  display_text("Press anything to start again!", window_width_ / 2,
               window_height_ / 3 * 2, 30, {255, 255, 255}, true);

  SDL_RenderPresent(renderer_);
  SDL_Delay(500);

  SDL_Event event;
  bool start = false;
  while (SDL_PollEvent(&event)) {
    SDL_PumpEvents();
    SDL_FlushEvent(SDL_KEYDOWN);
  }
  while (!start) {
    while (SDL_PollEvent(&event)) {
      if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_SetWindowFullscreen(window_, 0);
      }
      if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        {
          SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
      }
      if (event.type == SDL_KEYDOWN) start = true;
    }
  }
  SDL_RenderClear(renderer_);
  delta_time_ = calculate_delta_time(SDL_GetPerformanceCounter(),
                                     SDL_GetPerformanceCounter());
}
void Game::game_over() {
  play_game_over_audio();

  display_text("Game over!", window_width_ / 2, window_height_ / 3 * 2, 50,
               {255, 255, 255}, true);
  SDL_RenderPresent(renderer_);
  SDL_Delay(1000);

  game_reset();
}

void Game::game_won() {
  display_text("Level WON!", window_width_ / 2, window_height_ / 3 * 2, 50,
               {255, 255, 255}, true);
  SDL_RenderPresent(renderer_);
  SDL_Delay(1500);
  level_++;
  if (level_ > 4) level_ = 4;

  game_reset();
}
bool Game::init_audio() {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << '\n';
    return false;
  }
  SDL_AudioSpec desiredSpec;
  SDL_zero(desiredSpec);
  desiredSpec.freq = 44100;
  desiredSpec.format = AUDIO_S16SYS;
  desiredSpec.channels = 2;
  desiredSpec.samples = 2048;
  desiredSpec.callback = nullptr;
  audio_device_ = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &audio_spec_, 0);
  if (audio_device_ == 0) {
    std::cerr << "SDL_OpenAudioDevice failed: " << SDL_GetError() << '\n';
    return false;
  }
  return true;
}

bool Game::load_wav_file(const char* file) {
  if (SDL_LoadWAV(file, &audio_spec_, &audio_buffer_, &audio_length_) ==
      nullptr) {
    std::cerr << "Failed to load WAV file: " << SDL_GetError() << '\n';
    return false;
  }
  return true;
}

void Game::play_game_over_audio() {
  if (!load_wav_file("assets/audio/game_over.wav")) return;

  SDL_QueueAudio(audio_device_, audio_buffer_, audio_length_);
  SDL_PauseAudioDevice(audio_device_, 0);
}
void Game::play_ball_hit_audio() {
  if (!load_wav_file("assets/audio/ball_hit.wav")) return;

  SDL_ClearQueuedAudio(audio_device_);
  SDL_QueueAudio(audio_device_, audio_buffer_, audio_length_);
  SDL_PauseAudioDevice(audio_device_, 0);
}
void Game::play_game_start_audio() {
  if (!load_wav_file("assets/audio/bleep.wav")) return;

  SDL_QueueAudio(audio_device_, audio_buffer_, audio_length_);
  SDL_PauseAudioDevice(audio_device_, 0);
}

void Game::load_level(int level) {
  int rows;
  int spacing;
  int row_height;

  int brick_width = bricks_[0].get_width();
  int brick_height = bricks_[0].get_height();
  int br_count = 0;
  int start_x;
  int start_y;
  int spacing_x;
  int spacing_y;

  switch (level) {
    case 1:
      rows = 9;
      spacing = 40;
      row_height = 10;

      start_x = window_width_ / 2 - (brick_width / 2);
      for (int i = 0; i < 3; i++) {
        for (int row = 0; row < rows; ++row) {
          int row_offset = row * (brick_width / 2 + spacing);
          int y = (rows - row - 1) * (brick_height + row_height);
          bricks_[br_count].set_health(1);
          bricks_[br_count].set_x(start_x - row_offset);
          bricks_[br_count++].set_y(y);
          bricks_[br_count].set_health(1);
          bricks_[br_count].set_x(start_x + row_offset);
          bricks_[br_count++].set_y(y);
        }
        rows -= 3;
      }
      break;
    case 2:
      rows = 7;
      spacing_x = 55;
      spacing_y = 15;
      br_count = 0;
      start_x = 30;
      start_y = 0;
      for (int col = 0; col < 6; col++) {
        for (int row = 0; row < rows; ++row) {
          int xPos = start_x + row * (spacing_x);
          int yPos = start_y + row * (brick_height + spacing_y);

          bricks_[br_count].set_x(xPos);
          bricks_[br_count].set_health(1);
          bricks_[br_count++].set_y(yPos);
        }
        start_x += brick_width + 150;
      }
      break;
    case 3:
      br_count = 0;
      start_x = 40;
      start_y = 0;
      spacing_x = brick_width;
      spacing_y = brick_height;
      rows = 10;
      for (int row = 0; row < rows; ++row) {
        bool isStaggered = (row % 2 == 1);
        int bricks_in_row = window_width_ / (brick_width + spacing_x);
        int rowStartX = start_x + (isStaggered ? spacing_x : 0);
        for (int brick = 0; brick < bricks_in_row; ++brick) {
          if (br_count >= bricks_.size()) break;

          int xPos = rowStartX + brick * (brick_width + spacing_x);
          int yPos = start_y + row * (brick_height);

          bricks_[br_count].set_health(1);
          bricks_[br_count].set_x(xPos);
          bricks_[br_count++].set_y(yPos);
        }
      }
      break;
    case 4:
      int x = 10, y = 10;
      rows = 0;
      for (auto& brick : bricks_) {
        brick.set_x(x);
        brick.set_y(y);
        brick.set_health(1);
        x += brick.get_width() + 10;

        if (x + brick.get_width() - (10 * rows + 1) >=
            window_width_ - (float(brick.get_width()) / 1.5 * rows)) {
          x = (float(brick.get_width()) / 1.5 * (rows + 1)) - (10 * rows + 1);
          y += brick.get_height() + 5;
          rows++;
        }
      }
  }
}