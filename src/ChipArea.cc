
#include "ChipArea.h"
#include "ScreenStack.h"
#include <X11/Xlib.h>
#include <windows.h>
#include <iostream>

#define margin 30

ChipArea::ChipArea(ScreenStack *stack)
{
    this->stack = stack;
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    Gtk::Box *wrapper = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 0));
    wrapper->add_css_class({"chip-wrapper"});
    wrapper->set_vexpand(true);
    wrapper->set_hexpand(true);

    container = Gtk::manage(new Gtk::Grid());
    container->add_css_class({"chip-area"});
    container->set_vexpand(true);
    container->set_hexpand(true);
    // container->set_size_request(width, height);

    // creationArea
    creationArea = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 0));
    creationArea->add_css_class({"creation-area"});
    container->attach(*creationArea, 0, 0, 1, 3);
    creationArea->set_vexpand(true);
    creationArea->set_hexpand(true);

    // canvas
    canvas = Gtk::manage(new Gtk::DrawingArea());
    canvas->add_css_class({"chip-canvas"});
    creationArea->append(*canvas);
    canvas->set_vexpand(true);
    canvas->set_hexpand(true);

    m_GestureDrag = Gtk::GestureDrag::create();
    m_GestureDrag->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    m_GestureDrag->signal_drag_begin().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_begin));
    m_GestureDrag->signal_drag_end().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_end));
    m_GestureDrag->signal_drag_update().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_update));
    add_controller(m_GestureDrag);

    // connect it with canvas

    // now call the function to draw the chip
    canvas->set_draw_func(sigc::mem_fun(*this, &ChipArea::draw_on_canvas));

    draggedChip = nullptr;

    chips = new std::vector<Chip>();
    create_chip(0);

    // chipSelector
    chipSelector = Gtk::manage(new ChipSelectorUI());
    chipSelector->add_css_class({"chip-selector"});
    container->attach(*chipSelector, 0, 3, 1, 1);

    // add container to the
    wrapper->append(*container);
    set_child(*wrapper);
}

void ChipArea::on_my_drag_begin(double start_x, double start_y)
{
    std::cout << "Drag Begin" << std::endl;
    for (int i = 0; i < chips->size(); i++)
    {

        bool isInside = (*chips)[i].isMouseInside(start_x - margin, start_y - margin);
        std::cout << "Is Inside: " << isInside << std::endl;
        if (isInside)
        {
            draggedChip = &(*chips)[i];

            // print start_x and start_y
            std::cout << "Start X: " << start_x - margin << " Start Y: " << start_y - margin << std::endl;


            MouseOffset offset = (*chips)[i].getMouseOffset(start_x - margin, start_y - margin);
            //print the mouse offset
            std::cout << "Offset X: " << offset.x << " Offset Y: " << offset.y << std::endl;

            double new_x = (start_x - margin) - (*chips)[i].structure->boundingBox->x - offset.x;
            double new_y =  (start_y - margin) - (*chips)[i].structure->boundingBox->y - offset.y;
            // print new_x and new_y
            std::cout << "New X: " << new_x << " New Y: " << new_y << std::endl;
            (*chips)[i].structure->boundingBox->x += new_x;
            (*chips)[i].structure->boundingBox->y += new_y;

            (*chips)[i].structure->boundingBox->intial_x += new_x;
            (*chips)[i].structure->boundingBox->intial_y += new_y;
            break;
        }
    }
    canvas->queue_draw();
}

void ChipArea::on_my_drag_update(double offset_x, double offset_y)
{

    // offset_x	X offset, relative to the start point.
    // offset_y	Y offset, relative to the start point.

    std::cout << "Drag Update" << std::endl;
    // std::cout << "Dragged Chip [update]: " << draggedChip << std::endl;
    if(draggedChip == nullptr){
        return;
    }
    std::cout << "Offset X: " << offset_x << " Offset Y: " << offset_y << std::endl;
    std::cout << "Mouse X: " << draggedChip->structure->boundingBox->intial_x << " Mouse Y: " << draggedChip->structure->boundingBox->intial_y << std::endl;

    double new_x = draggedChip->structure->boundingBox->intial_x + offset_x;
    double new_y = draggedChip->structure->boundingBox->intial_y + offset_y;


    draggedChip->structure->boundingBox->x = new_x;
    draggedChip->structure->boundingBox->y = new_y;

    canvas->queue_draw();
}

void ChipArea::on_my_drag_end(double offset_x, double offset_y)
{
    std::cout << "Drag End" << std::endl;
    // use draggedChip
    if(draggedChip == nullptr){
        return;
    }
    double new_x = draggedChip->structure->boundingBox->intial_x + offset_x;
    double new_y = draggedChip->structure->boundingBox->intial_y + offset_y;
    draggedChip->structure->setLoc(new_x, new_y);
    // clear draggedChip
    draggedChip = nullptr;
    // (*chips)[i].isDragging = false;
    // double new_x = (*chips)[i].structure->boundingBox->x + offset_x - (*chips)[i].structure->boundingBox->intial_x;
    // double new_y = (*chips)[i].structure->boundingBox->y + offset_y - (*chips)[i].structure->boundingBox->intial_y;
    // (*chips)[i].structure->setLoc(new_x, new_y);

    canvas->queue_draw();
}

void ChipArea::create_chip(int index)
{
    // let create a new chip
    ChipStructure *structure = new ChipStructure(new ChipBoundingBox{100, 100, 0, 0});
    std::vector<InputPin> *inputPins = new std::vector<InputPin>();
    std::vector<OutputPin> *outputPins = new std::vector<OutputPin>();
    inputPins->push_back(InputPin("A", 0));
    inputPins->push_back(InputPin("B", 1));
    outputPins->push_back(OutputPin("Y", 0));
    Chip *chip = new Chip(structure, inputPins, outputPins, "AND");
    chips->push_back(*chip);

    // create another chip
    ChipStructure *structure2 = new ChipStructure(new ChipBoundingBox{100, 100, 200, 0});
    std::vector<InputPin> *inputPins2 = new std::vector<InputPin>();
    std::vector<OutputPin> *outputPins2 = new std::vector<OutputPin>();
    inputPins2->push_back(InputPin("A", 0));
    inputPins2->push_back(InputPin("B", 1));
    outputPins2->push_back(OutputPin("Y", 0));
    Chip *chip2 = new Chip(structure2, inputPins2, outputPins2, "OR");
    chips->push_back(*chip2);
    canvas->queue_draw();
}

void ChipArea::clear_canvas(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill();
}

void ChipArea::draw_on_canvas(const Cairo::RefPtr<Cairo::Context> &cr,
                              int width, int height)
{
    clear_canvas(cr);
    for (int i = 0; i < chips->size(); i++)
    {
        (*chips)[i].draw(cr);
    }
}

ChipSelectorUI::ChipSelectorUI()
{
    set_css_classes({"chip-selector"});
    set_orientation(Gtk::Orientation::HORIZONTAL);
    set_spacing(0);

    menu = Gtk::manage(new Gtk::Button());
    menu->set_label("Menu");
    menu->set_size_request(50, 50);
    menu->set_css_classes({"chip-menu-btn", "chip-btn"});
    append(*menu);

    for (int i = 0; i < 5; i++)
    {
        chips[i] = Gtk::manage(new Gtk::Button());
        chips[i]->set_label("Chip " + std::to_string(i));
        chips[i]->set_size_request(50, 50);
        chips[i]->set_css_classes({"chip-btn"});
        // chips[i]->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &ChipSelectorUI::on_chip_selected), i));
        append(*chips[i]);
    }
}

void ChipSelectorUI::on_chip_selected(int index)
{
    selected_chip = index;
}

// Bind::Bind(OutputPin *output, InputPin *input){
//     this->output = output;
//     this->input = input;
// }

InputPin::InputPin(std::string name, int index)
{
    this->name = name;
    this->index = index;
}

void InputPin::setCord(int x,int y){
    this->x = x;
    this->y = y;
}
    
// void InputPin::isInside(int mouseX, int mouseY){

// }

OutputPin::OutputPin(std::string name, int index)
{
    this->name = name;
    this->index = index;
}

void OutputPin::setCord(int x,int y){
    this->x = x;
    this->y = y;
}

ChipStructure::ChipStructure(ChipBoundingBox *boundingBox)
{
    this->boundingBox = boundingBox;
    this->boundingBox->intial_x = boundingBox->x;
    this->boundingBox->intial_y = boundingBox->y;
}
void ChipStructure::setBoundingBox(int width, int height, int x, int y)
{
    this->boundingBox->width = width;
    this->boundingBox->height = height;
    this->boundingBox->x = x;
    this->boundingBox->y = y;

    this->boundingBox->intial_x = x;
    this->boundingBox->intial_y = y;
}

void ChipStructure::setLoc(int x, int y)
{
    this->boundingBox->x = x;
    this->boundingBox->y = y;
    this->boundingBox->intial_x = x;
    this->boundingBox->intial_y = y;
}

Chip::Chip(ChipStructure *structure, std::vector<InputPin> *inputPins, std::vector<OutputPin> *outputPins, std::string name)
{
    this->structure = structure;
    this->inputPins = inputPins;
    this->outputPins = outputPins;
    this->name = name;
};

bool Chip::isMouseInside(int x, int y)
{

    std::cout << "X: " << x << " Y: " << y << " Chip X: " << structure->boundingBox->x << " Chip Y: " << structure->boundingBox->y << " Chip Width: " << structure->boundingBox->width << " Chip Height: " << structure->boundingBox->height << std::endl;
    return x >= structure->boundingBox->x && x <= structure->boundingBox->x + structure->boundingBox->width && y >= structure->boundingBox->y && y <= structure->boundingBox->y + structure->boundingBox->height;
}

MouseOffset Chip::getMouseOffset(int x, int y)
{
    return MouseOffset{static_cast<double>(x) - structure->boundingBox->x, static_cast<double>(y) - structure->boundingBox->y};
}

void clear_canvas(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill();
};

void Chip::draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    clear_canvas(cr);
    // draw the chip
    // draw rectange
    // draw input and output pins
    // draw the name of the chip

    cr->set_source_rgb(38 / 255.0, 38 / 255.0, 38 / 255.0);
    // calculate height using the max(height of input pins, height of output pins)
    // int gapper = 40;
    // int inputHeight = inputPins->size() * gapper;
    // int outputHeight = outputPins->size() * gapper;
    // int height = inputHeight > outputHeight ? inputHeight : outputHeight;
    // cr->set_source_rgb(6/255, 39/255, 92/255);
    // cr->rectangle(0, 0, 100, height);
    // cr->fill();

    int x = structure->boundingBox->x;
    int y = structure->boundingBox->y;

    int gapper = 25;
    int width = 150;
    int inputHeight = inputPins->size() * gapper;
    int outputHeight = outputPins->size() * gapper;
    int height = inputHeight > outputHeight ? inputHeight : outputHeight;

    structure->boundingBox->width = width;
    structure->boundingBox->height = height;

    cr->set_source_rgb(200 / 255.0, 39 / 255.0, 92 / 255.0);
    cr->rectangle(x, y, width, height);
    cr->fill();

    // draw the input pins

    int eachPinSpace = (height / (inputPins->size() > outputPins->size() ? inputPins->size() : outputPins->size())) / 2;

    for (int i = 0; i < inputPins->size(); i++)
    {

        cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
        // cr->arc(20, i * gapper + gapper / 2, 5, 0, 2 * M_PI);
        int inputPinEachHeight = height / inputPins->size();
        cr->arc(x, y + i * (inputPinEachHeight) + (inputPinEachHeight / 2), eachPinSpace - 3, 0, 2 * M_PI);

        cr->fill();

        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->move_to(x + 15, y + i * (inputPinEachHeight) + (inputPinEachHeight / 2) + 3);
        cr->set_font_size(14);
        cr->show_text((*inputPins)[i].name);
        (*inputPins)[i].setCord(x, y + i * (inputPinEachHeight) + (inputPinEachHeight / 2));
    }

    // draw the output pins
    for (int i = 0; i < outputPins->size(); i++)
    {
        cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
        // cr->arc(80, i * gapper + gapper / 2, 5, 0, 2 * M_PI);
        int outputPinEachHeight = height / outputPins->size();
        cr->arc(x + width, y + i * (outputPinEachHeight) + (outputPinEachHeight / 2), eachPinSpace - 3, 0, 2 * M_PI);
        cr->fill();

        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->move_to(x + width - 25, y + i * (outputPinEachHeight) + (outputPinEachHeight / 2) + 3);
        cr->set_font_size(14);
        cr->show_text((*outputPins)[i].name);
        (*outputPins)[i].setCord(x + width, y + i * (outputPinEachHeight) + (outputPinEachHeight / 2));
    }

    // draw the name of the chip
    cr->set_source_rgb(1.0, 1.0, 1.0);
    int font_size = 20;
    int length = name.length();
    int nameLengthInPixels = length * font_size;
    cr->set_font_size(font_size);

    cr->move_to(x + (width / 2) - 20, y + (height / 2) + 7);
    cr->show_text(name);
    cr->fill();
}

GlobalInputPin::GlobalInputPin(int index, int y)
{
    this->index = index;
    this->y = y;
}

GlobalOutputPin::GlobalOutputPin(int index, int y)
{
    this->index = index;
    this->y = y;
}
