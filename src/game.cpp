
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
    last_time = current_time;
  }
  quit();
}

void Game::render() {
  SDL_RenderClear(renderer_);

  SDL_RenderCopy(renderer_, background_texture_, NULL, NULL);

  render_score();

  for (int i = 0; i < NUM_OF_BRICKS; i++) {
    brick_[i].render();
  }

  paddle_.render();
  ball_.render();
  ball_.move(delta_time_);
}

void Game::initialize() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
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
  ball_ = Ball(window_width_, window_height_, &paddle_, &brick_, renderer_);
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
      { SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP); }
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
  int x = 10, y = 10;
  int health = 1;
  int brick_width = 100;

  int row = 0;
  for (int i = 0; i < NUM_OF_BRICKS; i++) {
    brick_[i] = Brick(renderer_, x, y, health);
    x += brick_[i].get_width() + 10;

    if (x + brick_[i].get_width() - (10 * row + 1) >=
        window_width_ - (float(brick_[i].get_width()) / 1.5 * row)) {
      x = (float(brick_[i].get_width()) / 1.5 * (row + 1)) - (10 * row + 1);
      y += brick_[i].get_height() + 5;
      row++;
    }
  }
}

void Game::render_menu() {
  SDL_RenderClear(renderer_);
  SDL_RenderCopy(renderer_, background_texture_, NULL, NULL);
  std::string a;
  a = "BRICK BREAKER";
  display_text(a, window_width_ / 2, window_height_ / 4, 80, {255, 255, 255},
               true);

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

  if (game_in_progress_) {
    if (selected_menu_item_ == 2)
      a = "> RESET GAME <";
    else
      a = "RESET GAME";
    display_text(a, window_width_ / 2, window_height_ / 2 + 100, 40,
                 {255, 255, 255}, true);
  }

  a = "- Berina Majdancic -";
  display_text(a, window_width_ / 2, window_height_ - 50, 30, {255, 255, 255},
               true);
}

void Game::handle_menu_input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
      SDL_SetWindowFullscreen(window_, 0);
    }
    if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
      { SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP); }
    }
    if (event.type == SDL_KEYUP &&
        (event.key.keysym.scancode == SDL_SCANCODE_DOWN ||
         event.key.keysym.scancode == SDL_SCANCODE_S)) {
      if (selected_menu_item_ + 1 <= 2) {
        selected_menu_item_++;
        if (!game_in_progress_) selected_menu_item_ = 1;
      }
    } else if (event.type == SDL_KEYUP &&
               (event.key.keysym.scancode == SDL_SCANCODE_UP ||
                event.key.keysym.scancode == SDL_SCANCODE_W)) {
      if (selected_menu_item_ - 1 >= 0) selected_menu_item_--;
    }
  }
  const Uint8* keystate = SDL_GetKeyboardState(nullptr);
  if (keystate[SDL_SCANCODE_RETURN]) {
    switch (selected_menu_item_) {
      case 0:  // Start / Resume
        in_menu_ = false;
        game_in_progress_ = true;
        break;
      case 1:  // Exit
        in_menu_ = false;
        running_ = false;
        break;
      case 2:  // Reset
        in_menu_ = false;
        game_over();
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
  if (delta_time < 16.67) {
    SDL_Delay(static_cast<Uint32>(16.67 - delta_time));
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
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void Game::game_over() {
  ball_.reset();
  paddle_.reset();
  for (int i = 0; i < NUM_OF_BRICKS; i++) brick_[i].reset();

  display_text("Game over!", window_width_ / 2, window_height_ / 3 * 2, 50,
               {255, 255, 255}, true);
  SDL_RenderPresent(renderer_);
  render();
  SDL_Delay(1000);
  display_text("Press anything to start again!", window_width_ / 2,
               window_height_ / 3 * 2, 30, {255, 255, 255}, true);

  SDL_RenderPresent(renderer_);

  SDL_Event event;
  bool start = false;
  while (!start) {
    while (SDL_PollEvent(&event)) {
      if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        SDL_SetWindowFullscreen(window_, 0);
      }
      if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
        { SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP); }
      }
      if (event.type == SDL_KEYDOWN) start = true;
    }
  }
  SDL_RenderClear(renderer_);
}