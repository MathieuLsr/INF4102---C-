// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <list>
#include <cmath>

using namespace std ;

#define TIME_WOLF_DIE_AFTER_LAST_SHEEP 15000
#define TIME_BETWEEN_TWO_REPRODUCTION_SHEEP 5000
#define DISTANCE_BETWEEN_WOLF_AND_SHEEP 70
#define DISTANCE_BETWEEN_TWO_SHEEPS 140
#define COOLDOWN_BETWEEN_INTERACTION_WITH_DOGS 2000
#define TIME_WOLF_FREEZE_BY_DOG 4000

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1200; // Width of window in pixel
constexpr unsigned frame_height = 700; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;

enum SEXE_ANIMAL{FEMALE,MALE} ;

// Helper function to initialize SDL
void init();

class entity {    
                           
  protected:
    SDL_Surface* window_surface_ptr_;          
    SDL_Surface* image_ptr_; 
    struct SDL_Rect image_position_;
    int speed ;
    int default_speed ;

  public:
    entity(const std::string& file_path, SDL_Surface* window_surface_ptr) ;
    ~entity(){};
    void move_to_position(int x, int y) ;
    SDL_Rect get_image_position() ;
    int get_distance(entity* entity) ;
    int getX() ;
    int getY() ;
    virtual void move(){};
    virtual void draw(){};

  
} ;

class dog : public entity {

  private :
    int angle ;

  protected:

  public :
    dog(SDL_Surface* image_ptr_, SDL_Surface* window_surface_ptr) ;
    ~dog(){};
    void draw();
    int get_angle() ;
    void implement_angle() ;
    void set_angle(int angle) ;

} ;

class player : public entity {

  private :
    vector<SDL_Surface*> list_png ;
    int state_png ;

    vector<SDL_Surface*> list_png_dogs ;
    vector<dog*> list_dogs ;
    Uint32 last_interaction_dogs ;
    SDL_Rect texture_help_dogs_position ;
    SDL_Surface* texture_help_dogs ;

  public:
    player(SDL_Surface* window_surface_ptr) ;
    void draw();
    void move();  
    void add_new_dog() ;
    void remove_dog() ;
    bool is_cooldown_interaction_dogs() ;

} ;

class animal : public entity {

protected:
  struct SDL_Point image_direction_;

public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr) ;
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  ~animal(){}; // todo: Use the destructor to release memory and "clean up
               // behind you"

  virtual void draw(); // todo: Draw the animal on the screen <-> window_surface_ptr.
                 // Note that this function is not virtual, it does not depend
                 // on the static type of the instance
  void set_speed(int speed) ;
  int get_default_speed() ;
 
  void set_direction(SDL_Point pt) ;
  SDL_Rect getPosition() ;

  virtual void move();     // todo: Animals move around, but in a different
                             // fashion depending on which type of animal
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {

  SDL_Surface* image_cannot_reproduct_ptr_; 
  Uint32 last_reproduction ;
  SEXE_ANIMAL sexe_animal ;

  public:
    sheep(SDL_Surface* window_surface_ptr) ;

    void draw() ;
    void move() ;
    sheep* is_near_to_other_sheep(list<sheep*> list) ;
    bool can_reproduct_with(sheep* sh) ;
    void set_reproduct(bool b) ;
    void set_sexe(SEXE_ANIMAL sexe) ;
    
  // todo
  // Ctor
  // Dtor
  // implement functions that are purely virtual in base class
};

// Insert here:
// class wolf, derived from animal
// Use only sheep at first. Once the application works
// for sheep you can add the wolves
class wolf : public animal {

  private:
    Uint32 time_last_eat_sheep ;
    Uint32 time_last_freeze_by_dog ;

  public:
    wolf(SDL_Surface* window_surface_ptr) ;
    void move() ;
    void set_closer_position_sheep(std::list<sheep*> list_sheep) ;
    sheep* is_near_sheep(std::list<sheep*> list_sheep) ;
    Uint32 get_time_last_eat_sheep() ;
    void refresh_time_eat_sheep() ;
    bool is_starve_and_die() ;
    void freeze_by_dog() ;
    bool is_freeze_by_dog() ;

  // todo
  // Ctor
  // Dtor
  // implement functions that are purely virtual in base class
};

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface* window_surface_ptr_;

  // Some attribute to store all the wolves and sheep
  // here
  player* player ;
  std::list<entity*> list_entities ;
  std::list<sheep*> list_sheep ;
  std::list<wolf*> list_wolf ;

public:
  ground(SDL_Surface* window_surface_ptr); // todo: Ctor
  ~ground(){}; // todo: Dtor, again for clean up (if necessary)
  void add_entity(entity* a); // todo: Add an animal
  void remove_entity(entity* a); 
  void update(); // todo: "refresh the screen": Move animals and draw them
  int number_sheeps() ;
  void set_player(struct player* pl) ;
  struct player* get_player() ;
  // Possibly other methods, depends on your implementation

}; 

// The application class, which is in charge of generating the window
class application {
private:
  // The following are OWNING ptrs
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;

  // Other attributes here, for example an instance of ground
  ground* ground_;
  

public:
  application(unsigned n_sheep, unsigned n_wolf) ; // Ctor
  ~application();                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};