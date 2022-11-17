// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <chrono>
#include <thread>

using namespace std ; 

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.

  SDL_Surface* load_surface_for(const std::string& path/*,
                                SDL_Surface* window_surface_ptr*/) {

    // Helper function to load a png for a specific surface
    // See SDL_ConvertSurface

    SDL_Surface* img = IMG_Load(path.c_str()) ;
    if (!img)
      throw std::runtime_error("Could not load image");

    return img ;
  }

  SDL_Point get_new_direction(){
    int x = (int)(rand()%frame_width) ;
    int y = (int)(rand()%frame_height) ;
    
    if(x > frame_width-frame_boundary) x = frame_width-frame_boundary ;
    //else if(x < frame_boundary) x = frame_boundary ; // Commenter car l'écran est trop petit 

    if(y > frame_height-frame_boundary) y = frame_height-frame_boundary ;
    //else if(y < frame_boundary) y = frame_boundary ; // Commenter car l'écran est trop petit 
    
    return {x, y} ;
  }

  wolf* is_sheep_close_to_a_wolf(sheep* sheep, list<wolf*> list){

    for(wolf* s : list){
      int dist = s->get_distance(sheep); 
      if(dist == 0) continue ;
      if(dist < 140) return s ;
    }
    return NULL ;

  }

  template<typename Base, typename T>
  inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
  }


} // namespace

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));


  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));

  /* initialize random seed: */
  srand (time(NULL));

} 


application::application(unsigned n_sheep, unsigned n_wolf){

  
  this->window_ptr_ = SDL_CreateWindow("SDL2 Window",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     frame_width, frame_height,
                                     0);

  if (!this->window_ptr_)
    throw std::runtime_error(std::string(SDL_GetError()));

  this->window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);

  if (!this->window_surface_ptr_)
    throw std::runtime_error(std::string(SDL_GetError()));

  SDL_UpdateWindowSurface(this->window_ptr_);

  ground* gr = new ground(this->window_surface_ptr_);
  this->ground_ = gr ;

  for(int i=0 ; i < (int) n_sheep ; i++ ) {
    sheep* sh = new sheep(this->window_surface_ptr_) ;
    sh->move_to_position(100*i, 600) ;
    sh->set_sexe(i%2 == 0 ? SEXE_ANIMAL::FEMALE : SEXE_ANIMAL::MALE) ;
    this->ground_->add_entity(sh) ;
  }
  for(int i=0 ; i < (int) n_wolf ; i++ ) {
    wolf* wo = new wolf(this->window_surface_ptr_) ;
    wo->move_to_position(300*i, 100) ;
    this->ground_->add_entity(wo) ;
  }

  
  player* pl = new player(this->window_surface_ptr_) ;
  this->ground_->set_player(pl) ;
  this->ground_->add_entity(pl) ;
  pl->add_new_dog() ;


}

application::~application(){}

int application::loop(unsigned period){

  int time_application = (int) period * 1000 ;
  cout << time_application << endl ;
  auto start = SDL_GetTicks();
  bool quit = false;

  while (!quit && (SDL_GetTicks() - start < time_application)) {
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 0, 0));
    while (SDL_PollEvent(&window_event_)) {
        if (window_event_.type == SDL_QUIT) {
            quit = true;
            break;
        }
    }
    this->ground_->update() ;

    SDL_UpdateWindowSurface(window_ptr_);

    this_thread::sleep_for(chrono::milliseconds((int)1000/(int)frame_rate)) ;
  }

  return this->ground_->number_sheeps() ;

}

ground::ground(SDL_Surface* window_surface_ptr){
  this->window_surface_ptr_ = window_surface_ptr ;
  this->list_entities = {} ;
  this->list_sheep = {} ;
  this->list_wolf = {} ;
}

void ground::add_entity(entity* a){
    this->list_entities.push_back(a) ;

    if(instanceof<sheep>(a)) this->list_sheep.push_back((sheep*) a) ;
    if(instanceof<wolf>(a)) this->list_wolf.push_back((wolf*) a) ;
} 

void ground::remove_entity(entity* a){
    this->list_entities.remove(a) ;
    if(instanceof<sheep>(a)) this->list_sheep.remove((sheep*) a) ;
    if(instanceof<wolf>(a)) this->list_wolf.remove((wolf*) a) ;
}

int ground::number_sheeps(){
  return this->list_sheep.size();
}

void ground::set_player(struct player* pl){ this->player = pl ; } 
struct player* ground::get_player(){ return this->player ; }

#define DISTANCE_BETWEEN_DOG_WOLF 100

void ground::update(){

    list<wolf*> list_wolf_will_die = {} ;

    for(wolf* wo : this->list_wolf){

      if(wo->is_starve_and_die()){
        list_wolf_will_die.push_back(wo) ;
        continue ;
      }

      int dist = this->player->get_distance(wo) ;
      if(dist < DISTANCE_BETWEEN_DOG_WOLF){
        wo->freeze_by_dog() ;
      }

      wo->set_closer_position_sheep(this->list_sheep) ;
      sheep* sh = wo->is_near_sheep(this->list_sheep) ;

      if(sh == NULL) continue ;
      wo->refresh_time_eat_sheep() ;
      this->remove_entity(sh) ;
    }

    for(wolf* w : list_wolf_will_die)
      this->remove_entity(w) ;

    for(entity* ent : this->list_entities){
      ent->move() ;
      ent->draw() ;
      /*
      if(!instanceof<animal>(ent)) {
        ent->draw() ;
        continue ;
      }
      animal* anim = dynamic_cast<animal*>(ent) ;
      anim->move() ;
      anim->draw() ;
      */
    }

    for(sheep* sh : this->list_sheep){

        wolf* wo = is_sheep_close_to_a_wolf(sh, this->list_wolf) ;
        if(wo != NULL){

          int x = wo->getPosition().x - sh->getPosition().x ;
          int y = wo->getPosition().y - sh->getPosition().y ;

          int x1 = 200 ;
          int y1 = 150 ;

          if(x > 0) x1 *= -1 ;
          if(y > 0) y1 *= -1 ;

          int x_fin = sh->getPosition().x + x1 ;
          if(x_fin < frame_boundary) x_fin = frame_boundary ;
          else if(x_fin > frame_width-frame_boundary) x_fin = frame_width-frame_boundary ;

          int y_fin = sh->getPosition().y + y1 ;
          if(y_fin < frame_boundary) y_fin = 0 ;
          else if(y_fin > frame_height-frame_boundary) y_fin = frame_height-frame_boundary ;

          sh->set_direction({x_fin, y_fin}) ;
          sh->set_speed(4) ;
        }

        sheep* she_near = sh->is_near_to_other_sheep(this->list_sheep) ;

        if(she_near == NULL) continue ;
        if(!sh->can_reproduct_with(she_near)) continue ;

        sheep* s = new sheep(this->window_surface_ptr_) ;
        s->move_to_position(sh->getPosition().x, sh->getPosition().y) ;
        this->add_entity(s) ;
        break ;
      
    }

} // todo: "refresh the screen": Move animals and draw them

entity::entity(const std::string& file_path, SDL_Surface* window_surface_ptr){

  if(file_path != "") this->image_ptr_ = load_surface_for(file_path) ;
  this->window_surface_ptr_ = window_surface_ptr ;

  this->image_position_ = {0,0,22,43} ;
    
  this->default_speed = 2 ;
  this->speed = 2 ;

}

SDL_Rect entity::get_image_position(){
  return this->image_position_ ;
}

int entity::getX() { return this->image_position_.x ; }
int entity::getY() { return this->image_position_.y ; }

player::player(SDL_Surface* window_surface_ptr) : entity::entity("", window_surface_ptr) {

  this->list_png.push_back(load_surface_for("media/player/player_1_front.png")) ;
  this->list_png.push_back(load_surface_for("media/player/player_2_front.png")) ;
  this->list_png.push_back(load_surface_for("media/player/player_3_front.png")) ;
  this->list_png.push_back(load_surface_for("media/player/player_4_front.png")) ;

  this->list_dogs = {} ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/1.png")) ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/2.png")) ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/3.png")) ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/4.png")) ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/5.png")) ;
  this->list_png_dogs.push_back(load_surface_for("media/dogs/6.png")) ;
  

  this->default_speed = 3 ;
  this->speed = 3 ;
  this->state_png = 0 ;
  this->last_interaction_dogs = 0 ;

  this->texture_help_dogs = load_surface_for("media/dogs/help.png") ;
  this->texture_help_dogs_position = {(frame_width-626)/2, frame_height-60,626, 46} ;

}

void player::add_new_dog(){
  int size = this->list_dogs.size() ;
  if(size >= 6) return ;
  dog* d = new dog(this->list_png_dogs[size], this->window_surface_ptr_) ;
  d->set_angle(size*60) ;
  this->list_dogs.push_back(d) ;
}

void player::remove_dog(){
  if(this->list_dogs.size() == 0) return ;
  this->list_dogs.pop_back() ;
}

void player::draw(){

  if (SDL_BlitSurface(this->list_png[(this->state_png/15)%4], NULL, this->window_surface_ptr_, &this->image_position_))
      throw std::runtime_error("Could not apply texture.");

  if(!this->is_cooldown_interaction_dogs())
    if (SDL_BlitSurface(this->texture_help_dogs, NULL, this->window_surface_ptr_, &this->texture_help_dogs_position ))
      throw std::runtime_error("Could not apply texture.");

  int radius = 70 ;
  int diff = 40 ;
  int last_angle = -1 ;

  for(dog* d : this->list_dogs){

    d->implement_angle() ;
    
    int a = last_angle == -1 ? d->get_angle() : last_angle + diff ;
    double angle = a*M_PI / 120 ;
    int x = radius * cos(angle) ;
    int y = radius * sin(angle) ;

    d->move_to_position(this->image_position_.x+x, this->image_position_.y+y) ;
    d->draw() ;
    last_angle = a ;
  }
    
}


dog::dog(SDL_Surface* image_ptr, SDL_Surface* window_surface_ptr) : entity("", window_surface_ptr) {
  this->image_ptr_ = image_ptr ;
}

int dog::get_angle(){
  return this->angle ;
}
void dog::implement_angle() {
  this->angle += 1 ;
}

void dog::draw(){
  if (SDL_BlitSurface(this->image_ptr_, NULL, this->window_surface_ptr_, &this->image_position_))
      throw std::runtime_error("Could not apply texture.");
}

void dog::set_angle(int angle){
  this->angle = angle ;
}

void player::move(){

  //if(event->type != SDL_KEYDOWN) return ;
  
  int x = this->image_position_.x ;
  int y = this->image_position_.y ;
  int spd = this->speed ;
  bool b = false ;
  
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_A]) {
    if(x - spd > 0) x -= spd ;
    b = true ;
  }
  if(state[SDL_SCANCODE_D]) {
    if(x + spd < frame_width)  x += spd ;
    b = true ;
  }
  if(state[SDL_SCANCODE_W]) {
    if(y - spd > 0) y -= spd ;
    b = true ;
  }
  if(state[SDL_SCANCODE_S]) {
    if(y + spd < frame_height) y += spd ;
    b = true ;
  }

  if(state[SDL_SCANCODE_SPACE]) {
    if(!this->is_cooldown_interaction_dogs()){
      this->last_interaction_dogs = SDL_GetTicks() ;
      this->add_new_dog() ;
    }
  }
  if(state[SDL_SCANCODE_LSHIFT]) {
    if(!this->is_cooldown_interaction_dogs()){
      this->last_interaction_dogs = SDL_GetTicks() ;
      this->remove_dog() ;
    }
  }

  this->state_png = b ? this->state_png+1 : 0 ;

  this->move_to_position(x,y) ;

}

bool player::is_cooldown_interaction_dogs(){
  return SDL_GetTicks() - this->last_interaction_dogs < COOLDOWN_BETWEEN_INTERACTION_WITH_DOGS || this->list_dogs.size() >= 6 ;
}

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr)
    : entity::entity(file_path, window_surface_ptr) {

      this->image_direction_ = get_new_direction() ;
      
}
/*
animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr){
  
  this->image_ptr_ = load_surface_for(file_path) ;
  this->window_surface_ptr_ = window_surface_ptr ;

  this->image_position_ = {0,0,22,43} ;
  this->image_direction_ = get_new_direction() ;
  
}
*/
int entity::get_distance(entity* animal){
  int x1 = this->image_position_.x ;
  int y1 = this->image_position_.y ;

  int x2 = animal->image_position_.x ;
  int y2 = animal->image_position_.y ;

  int dist = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)) ;
  return dist ;
}

void animal::draw(){
  if (SDL_BlitSurface(this->image_ptr_, NULL, this->window_surface_ptr_, &this->image_position_))
      throw std::runtime_error("Could not apply texture.");
}

void sheep::draw(){

  animal::draw() ;
}


void animal::set_direction(SDL_Point pt){
  this->image_direction_.x = pt.x ;
  this->image_direction_.y = pt.y ;
}

void animal::move(){
  int i_dir_x = this->image_direction_.x ;
  int i_dir_y = this->image_direction_.y ;
  int i_ima_x = this->image_position_.x ;
  int i_ima_y = this->image_position_.y ;

  if(i_ima_x == i_dir_x && i_ima_y == i_dir_y){
    this->image_direction_ = get_new_direction() ;
    i_dir_x = this->image_direction_.x ;
    i_dir_y = this->image_direction_.y ;
    this->speed = this->get_default_speed() ;

    //cout << this->image_direction_.x << ", " << this->image_direction_.y << endl ; 

  }



  int x_ = i_dir_x - i_ima_x ;
  int x = x_ == 0 ? 0 : x_ > 0 ? this->speed : this->speed*-1 ;
  if(abs(x_) < this->speed) x = x_ ;

  int y_ = i_dir_y - i_ima_y ;
  int y = y_ == 0 ? 0 : y_ > 0 ? this->speed : this->speed*-1 ;
  if(abs(y_) < this->speed) y = y_ ;

  this->image_position_.x += (int)x ;
  this->image_position_.y += (int)y ;
}

void animal::set_speed(int speed){
  this->speed = speed ;
}

int animal::get_default_speed(){
  return this->default_speed ;
}

void entity::move_to_position(int x, int y) {
  this->image_position_.x = x ; 
  this->image_position_.y = y ; 
}

SDL_Rect animal::getPosition(){
  return this->image_position_ ;
}

void sheep::move(){
  animal::move() ;
}

sheep::sheep(SDL_Surface* window_surface_ptr) 
    : animal::animal("media/sheep/sheep_male.png", window_surface_ptr) {
      this->default_speed = 2 ;
      this->speed = 2 ;
      this->last_reproduction = SDL_GetTicks() ;

      
      
      
      int r = rand()%2 ;
      this->set_sexe(r == 0 ? SEXE_ANIMAL::MALE : SEXE_ANIMAL::FEMALE) ;

      

}

bool sheep::can_reproduct_with(sheep* sh){
  if(this->sexe_animal == sh->sexe_animal) return false ; 
  sheep* femele = sh->sexe_animal == SEXE_ANIMAL::FEMALE ? sh : this ;
  Uint32 time = SDL_GetTicks() ;
  if(time - femele->last_reproduction < TIME_BETWEEN_TWO_REPRODUCTION_SHEEP) return false ;
  femele->last_reproduction = time ;
  return true ;
}

sheep* sheep::is_near_to_other_sheep(list<sheep*> list){

    for(sheep* s : list){
      int dist = s->get_distance(this); 
      if(dist == 0) continue ;
      if(dist < DISTANCE_BETWEEN_TWO_SHEEPS) return s ;
    }

    return NULL ;

}

void sheep::set_sexe(SEXE_ANIMAL sexe){
  this->sexe_animal = sexe ;
  if(sexe == SEXE_ANIMAL::MALE) this->image_ptr_ = load_surface_for("media/sheep/sheep_male.png") ;
  else this->image_ptr_ = load_surface_for("media/sheep/sheep_feme.png") ;
}




wolf::wolf(SDL_Surface* window_surface_ptr)
    : animal::animal("media/wolf.png", window_surface_ptr) {
      this->default_speed = 2 ;
      this->speed = 2 ;
      this->time_last_eat_sheep = SDL_GetTicks() ;
}

void wolf::move(){
    if(this->is_freeze_by_dog()) return ;
    animal::move() ;
}

void wolf::set_closer_position_sheep(std::list<sheep*> list_sheep){

  int min = INT_MAX ;
  SDL_Point pt{0,0};

  if(list_sheep.size() == 0) return ;

  for(sheep* s : list_sheep){
    int a = this->get_distance(s) ;
    if(min > a){
      min = a ;
      pt.x = s->getPosition().x ;
      pt.y = s->getPosition().y ; 
    }
  }

  this->image_direction_ = pt ;
}

sheep* wolf::is_near_sheep(std::list<sheep*> list_sheep){

  for(sheep* sh : list_sheep){
    if(sh->get_distance(this) < DISTANCE_BETWEEN_WOLF_AND_SHEEP) return sh ;
  }

  return NULL ;
}

void wolf::refresh_time_eat_sheep(){
  this->time_last_eat_sheep = SDL_GetTicks() ;
}

Uint32 wolf::get_time_last_eat_sheep(){
  return this->time_last_eat_sheep ;
}

bool wolf::is_starve_and_die(){
  Uint32 tick = SDL_GetTicks() ;
  return tick - this->time_last_eat_sheep > TIME_WOLF_DIE_AFTER_LAST_SHEEP ;
}

void wolf::freeze_by_dog(){
  this->time_last_freeze_by_dog = SDL_GetTicks() ;
}

bool wolf::is_freeze_by_dog() {
  return SDL_GetTicks() - this->time_last_freeze_by_dog < TIME_WOLF_FREEZE_BY_DOG ;
}