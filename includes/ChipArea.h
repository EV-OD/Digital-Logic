#pragma once

// Not needed but just in case 😂
#ifndef CHIP_AREA
#define CHIP_AREA

#include <gtkmm.h>
class ScreenStack;

class Chip;

class ChipArea : public Gtk::Frame
{
public:
  ChipArea(ScreenStack *stack);
  Gtk::Box *chipSelector;
  Gtk::Box *creationArea;
  Gtk::Grid *container;
  ScreenStack *stack;
  Gtk::DrawingArea *canvas;
  
  std::vector<Chip> *chips;
  Chip *draggedChip;
    

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

// class Bind{
// public:
//   Bind(OutputPin *output, InputPin *input);
//   // self (output)
//   // next node (input)
//   // location []
//   // stroke
//   // color

//   OutputPin *output;
//   InputPin *input;
// };

class InputPin{
public:
    InputPin(std::string name, int index);
    std::string name;
    int index;
    int x;
    int y;
    void setCord(int x,int y);
    // void isInside(int mouseX, int mouseY);
};

class OutputPin{
public:
    OutputPin( std::string name, int index);
    std::string name;
    int index;
    int x;
    int y;
    void setCord(int x,int y);
    // void isInside(int mouseX, int mouseY);
    // Bind *bind;
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

class Chip{
  public:
  Chip(ChipStructure *structure, std::vector<InputPin> *inputPins, std::vector<OutputPin> *outputPins, std::string name);
  ChipStructure *structure;
  std::vector<InputPin> *inputPins;
  std::vector<OutputPin> *outputPins;
  std::string name;
  void draw(const Cairo::RefPtr<Cairo::Context>& cr);
  MouseOffset getMouseOffset(int x, int y);
  bool isMouseInside(int x, int y);
  bool isDragging = false;
};


class GlobalInputPin{
public:
    GlobalInputPin(int index, int y);
    std::string name = "A";
    int index;
    int y;
};

class GlobalOutputPin{
public:
    GlobalOutputPin(int index, int y);
    std::string name = "O";
    int index;
    int y;
};




#endif