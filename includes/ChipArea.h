#pragma once

// Not needed but just in case 😂
#ifndef CHIP_AREA
#define CHIP_AREA

#include <gtkmm.h>
class ScreenStack;

class Chip;
class OutputPin;
class InputPin;
class GlobalInputPin;
class GlobalOutputPin;

class ChipArea : public Gtk::Frame
{
public:
  ChipArea(ScreenStack *stack);
  Gtk::Box *chipSelector;
  Gtk::Box *creationArea;
  Gtk::Grid *container;
  ScreenStack *stack;
  Gtk::DrawingArea *canvas;

  Gtk::Overlay *overlay;
  
  std::vector<Chip> *chips;
  Chip *draggedChip;
  void run();

//   GlobalInputPin *draggedGlobalInputPin;
//   GlobalOutputPin *draggedGlobalOutputPin;
// it's vector
   std::vector<GlobalInputPin *> *globalInputPins;
   std::vector<GlobalOutputPin *> *globalOutputPins;

  OutputPin *draggedOutputPin;
  InputPin *draggedInputPin;
    

  Glib::RefPtr<Gtk::GestureDrag> m_GestureDrag;


  void on_my_drag_begin(double start_x, double start_y);
  void on_my_drag_end(double offset_x, double offset_y);
  void on_my_drag_update(double offset_x, double offset_y);



  void draw_on_canvas(const Cairo::RefPtr<Cairo::Context>& cr,
  int width, int height);

  void clear_canvas(const Cairo::RefPtr<Cairo::Context>& cr);
  void create_chip(int index);
};


class ChipSelectorUI : public Gtk::Box{
public:
    ChipSelectorUI();
    Gtk::Button *chips[5];
    Gtk::Button *menu;
    int selected_chip = -1;
    void on_chip_selected(int index);
};

struct ChipBoundingBox{
    int width;
    int height;
    int x;
    int y;
    int intial_x = 0;
    int intial_y = 0;
};

#include "ChipArea.h" // Add this line

void clear_canvas(const Cairo::RefPtr<Cairo::Context>& cr);

class Bind {
public:
    Bind(InputPin &input);  // Accept a reference to InputPin
    void printConnection();
    InputPin &input;  // Store input as a reference
};

class BindToGlobalOutPut {
public:
    BindToGlobalOutPut(GlobalOutputPin &output);  // Accept a reference to InputPin
    GlobalOutputPin &output;  // Store input as a reference
};

struct Cord{
    int x;
    int y;
};



class Pin {
public:
    Pin(std::string name, int index);
    std::string name;
    int index;
    int x;
    int y;
    void setCord(int x, int y);
    bool isInside(int mouseX, int mouseY);
    int radius;
    void setRadius(int radius);
    void printCord();
    Cord getCord();
    int state = 0;
    Chip *chip;
};

class InputPin : public Pin {
public:
    InputPin(std::string name, int index) : Pin(name, index) {};
};

class OutputPin : public Pin {
public:
    OutputPin(std::string name, int index) : Pin(name, index) {
        binds = new std::vector<Bind>();
        bindsToGlobalOutput = new std::vector<BindToGlobalOutPut>();

    }
    // bind should be array
    std::vector<Bind> *binds;
    std::vector<BindToGlobalOutPut> *bindsToGlobalOutput;
    void bindTo(InputPin &input);
    void bindToGlobalOutput(GlobalOutputPin &output);
};
class ChipStructure{
public:
    ChipStructure(ChipBoundingBox *boundingBox);
    ChipBoundingBox *boundingBox;
    void setBoundingBox(int width, int height, int x, int y);
    void setLoc(int x, int y);
};

struct MouseOffset {
    double x;
    double y;
};

enum ChipType{
    AND,
    NOT,
    CUSTOM
};

class Chip{
  public:
  Chip(ChipStructure *structure, std::vector<InputPin *> inputPins, std::vector<OutputPin *> outputPins, std::string name);

  ChipStructure *structure;
  std::vector<InputPin *> inputPins;
  std::vector<OutputPin *> outputPins;
  std::string name;
  void draw(const Cairo::RefPtr<Cairo::Context>& cr);
  MouseOffset getMouseOffset(int x, int y);
  bool isMouseInside(int x, int y);
  bool isDragging = false;
  ChipType type;
  void run();
  void setChipType(ChipType type){
        this->type = type;
  }
};


class GlobalInputPin : public Pin{
public:
    GlobalInputPin(int index, int y): Pin("Input", index){
        this->y = y;
        binds = new std::vector<Bind>();
    }
    // bind should be array
    std::vector<Bind> *binds;
    void bindTo(InputPin &input);
    int y;
    int state = 0;
    int radius = 20;
};

class GlobalOutputPin : public Pin{
public:
    GlobalOutputPin(int index, int y): Pin("Output", index){
        this->y = y;
    }
    int y;
    int state = 0;
    int radius = 20;
};

class ChipSelectorMenu: public Gtk::Box
{
    private:
};


#endif