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
class ChipSelectorMenu;
class Wire;

struct Cord
{
    int x;
    int y;
};
struct CordDouble
{
    double x;
    double y;
};


class ChipArea : public Gtk::Frame
{
public:
    ChipArea(ScreenStack *stack);
    int width;
    int height;

    Gtk::Box *chipSelector;
    Gtk::Box *creationArea;
    Gtk::Grid *container;
    ScreenStack *stack;
    Gtk::DrawingArea *canvas;
    CordDouble mousePos;

    Gtk::Overlay *overlay;

    std::vector<Chip> *chips;
    Chip *draggedChip;
    GlobalInputPin *draggedGlobalInputPin = nullptr;
    OutputPin *draggedOutputPin = nullptr;
    
    GlobalOutputPin *draggedGlobalOutputPin = nullptr;
    InputPin *draggedInputPin = nullptr;


    Wire *draggedWire = nullptr;

    void run();

    //   GlobalInputPin *draggedGlobalInputPin;
    //   GlobalOutputPin *draggedGlobalOutputPin;
    // it's vector
    std::vector<GlobalInputPin *> *globalInputPins;
    std::vector<GlobalOutputPin *> *globalOutputPins;


    ChipSelectorMenu *ActionMenu;

    Glib::RefPtr<Gtk::GestureDrag> m_GestureDrag;
    Glib::RefPtr<Gtk::GestureClick> m_GestureClick;

    void on_my_drag_begin(double start_x, double start_y);
    void on_my_drag_end(double offset_x, double offset_y);
    void on_my_drag_update(double offset_x, double offset_y);

    bool isHoveringLine(CordDouble MousePos, CordDouble A, CordDouble B, double tolerance);
    bool isHoveringWire(CordDouble MousePos, Wire * wire, double tolerance);

    void updateHoveringChipsPins(CordDouble mousePos);
    void updateHoveringWires(CordDouble mousePos);

    bool shouldQueueDraw = false;


    void onMyLeftClick(int n_press, double x, double y);

    void draw_on_canvas(const Cairo::RefPtr<Cairo::Context> &cr,
                        int width, int height);
    
    void on_my_motion(double x, double y);
    bool on_my_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);


    void clear_canvas(const Cairo::RefPtr<Cairo::Context> &cr);
    void createAndChip(int index, int posX, int posY);
    void createNotChip(int index, int posX, int posY);



    void clear_actions();
};

class ChipSelectorUI : public Gtk::Box
{
public:
    ChipSelectorUI(ChipArea *area, ChipSelectorMenu *menu);
    ChipSelectorMenu *menu;
    Gtk::Button *chips[5];
    Gtk::Button *menu_btn;
    
    int test;
    int selected_chip = -1;

    void on_chip_selected(int index);
};

struct ChipBoundingBox
{
    int width;
    int height;
    int x;
    int y;
    int intial_x = 0;
    int intial_y = 0;
};

#include "ChipArea.h" // Add this line

void clear_canvas(const Cairo::RefPtr<Cairo::Context> &cr);



class Wire{
    public:
    std::vector<CordDouble> *breakPoints;
    Wire(){
        breakPoints = new std::vector<CordDouble>();
    }
};

class Bind
{
public:
    Bind(InputPin &input); // Accept a reference to InputPin
    void printConnection();
    InputPin &input; // Store input as a reference
    OutputPin *output = nullptr;
    GlobalInputPin *gInput = nullptr;
    Wire *wire=nullptr;
    bool selected = false;
    bool isHovered = false;
};

class BindToGlobalOutPut
{
public:
    BindToGlobalOutPut(GlobalOutputPin &output); // Accept a reference to OutputPin
    GlobalOutputPin &output;                     // Store output as a reference
    OutputPin *localOutput = nullptr;
    GlobalInputPin *gInput = nullptr;
    Wire *wire=nullptr;
    bool selected = false;
    bool isHovered = false;
};



class Pin
{
public:
    Pin(std::string name, int index);
    std::string name;
    int index;
    int x;
    int y;
    void setCord(int x, int y);
    bool isInside(int mouseX, int mouseY);
    bool isHovering(int mouseX, int mouseY);
    int hoverRange = 0;
    int radius;
    void setRadius(int radius);
    void printCord();
    Cord getCord();
    Chip *chip;
    int state = 0;
    bool isHovered = false; 
    void drawHovered(const Cairo::RefPtr<Cairo::Context> &cr);
    void drawNormal(const Cairo::RefPtr<Cairo::Context> &cr);
};

class InputPin : public Pin
{
public:
    InputPin(std::string name, int index) : Pin(name, index){};
    Bind *bind = nullptr;
};

class OutputPin : public Pin
{
public:
    OutputPin(std::string name, int index) : Pin(name, index)
    {
        binds = new std::vector<Bind*>();
        bindsToGlobalOutput = new std::vector<BindToGlobalOutPut*>();
    }
    // bind should be array
    std::vector<Bind *> *binds;
    std::vector<BindToGlobalOutPut *> *bindsToGlobalOutput;
    void bindTo(InputPin &input);
    void bindToGlobalOutput(GlobalOutputPin &output);
};
class ChipStructure
{
public:
    ChipStructure(ChipBoundingBox *boundingBox);
    ChipBoundingBox *boundingBox;
    void setBoundingBox(int width, int height, int x, int y);
    void setLoc(int x, int y);
};

struct MouseOffset
{
    double x;
    double y;
};

enum ChipType
{
    AND,
    NOT,
    CUSTOM
};

enum PinType
{
    outputPin,
    inputPin
};

enum activeStatus
{
    ON,
    OFF
};

class Chip
{
public:
    Chip(ChipStructure *structure, std::vector<InputPin *> inputPins, std::vector<OutputPin *> outputPins, std::string name);

    ChipStructure *structure;
    std::vector<InputPin *> inputPins;
    std::vector<OutputPin *> outputPins;
    std::string name;
    void draw(const Cairo::RefPtr<Cairo::Context> &cr);
    // void draw_hovered_pin(const Cairo::RefPtr<Cairo::Context> &cr,int x, int y, int PinEachHeight,int eachPinSpace, int n);
    // void draw_normal_pin(const Cairo::RefPtr<Cairo::Context> &cr,int x, int y, int PinEachHeight,int eachPinSpace, int n, int state);

    MouseOffset getMouseOffset(int x, int y);
    bool isMouseInside(int x, int y);
    bool isMouseHover(int x, int y);
    int hoverRange = 10;
    bool isHovered = false;
    bool isClicked = false;
    bool isDragging = false;
    ChipType type;
    void addInputPin(InputPin *inputPin);
    void addOutputPin(OutputPin *outputPin);
    void run();
    // static Chip* create_chip(ChipArea *chipArea);
    void setChipType(ChipType type)
    {
        this->type = type;
    }
};

class CustomChip{
    public:
    CustomChip(std::vector<GlobalInputPin*> *globalInputPins, std::vector<GlobalOutputPin*> *globalOutputPins);
    std::vector<GlobalInputPin*> *globalInputPins;
    std::vector<GlobalOutputPin*> *globalOutputPins;
};

class GlobalInputPin : public Pin
{
public:
    GlobalInputPin(int index, int y) : Pin("Input", index)
    {
        this->y = y;
        binds = new std::vector<Bind*>();
        gbinds = new std::vector<BindToGlobalOutPut*>();
        radius = 20;
    }
    // bind should be array
    std::vector<Bind*> *binds;
    std::vector<BindToGlobalOutPut*> *gbinds;

    void bindTo(InputPin &input);
    ChipBoundingBox *boundingBox;
    bool IsToggleBtnInside(double mouseX, double mouseY);
    
};

class GlobalOutputPin : public Pin
{
public:
    GlobalOutputPin(int index, int y) : Pin("Output", index)
    {
        this->y = y;
        radius = 20;
    }
    BindToGlobalOutPut *bindToGlobalOutput = nullptr;

};

class ChipSelectorMenu : public Gtk::Box
{
public:
    ChipSelectorMenu(int width, int height, ScreenStack *scrn_stack);
    bool visible = false;
    ScreenStack *scrn_stack;
    Gtk::Frame *ActionMenuFrame;
    Glib::RefPtr<Gtk::GestureClick> m_GestureClick;

    void hideMenu(int, int, int);
    void showMenu();
    void quit();
};


#endif