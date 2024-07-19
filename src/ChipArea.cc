#include "ChipArea.h"
#include "ScreenStack.h"
#include <X11/Xlib.h>
#include <windows.h>
#include <iostream>
#include "dialog.h"
#include <filesystem>

#define margin 30
#define globalPinGap 40

#include <string>
#include <algorithm>
#include <vector>

bool isValidFilename(const std::string& filename) {
    // List of invalid characters for Windows filenames
    const std::string invalidChars = "<>:\"/\\|?*";

    // Check for leading spaces
    if (!filename.empty() && filename.front() == ' ') {
        return false;
    }

    // Check for invalid characters
    for (char c : invalidChars) {
        if (filename.find(c) != std::string::npos) {
            return false;
        }
    }

    // Check for reserved filenames
    const std::vector<std::string> reservedNames = {
        "CON", "PRN", "AUX", "NUL",
        "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
        "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };
    
    std::string upperFilename = filename;
    std::transform(upperFilename.begin(), upperFilename.end(), upperFilename.begin(), ::toupper);

    if (std::find(reservedNames.begin(), reservedNames.end(), upperFilename) != reservedNames.end()) {
        return false;
    }

    // Check for trailing spaces or periods
    if (filename.back() == ' ' || filename.back() == '.') {
        return false;
    }

    // Check for empty filename
    if (filename.empty()) {
        return false;
    }

    // Optionally check the length of the filename (e.g., on Windows, the max path length is 260 characters)
    const size_t maxPathLength = 260;
    if (filename.length() > maxPathLength) {
        return false;
    }

    return true;
}


namespace fs = std::filesystem;
// Function to get filenames with a specific extension in a directory
std::vector<std::string> getFilesWithExtension(const std::string &directory, const std::string &extension)
{
    std::vector<std::string> filenames;

    try
    {
        fs::path dirPath(directory);

        // Check if the directory exists
        if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
        {
            std::cerr << "Error: Directory " << directory << " does not exist or is not a directory." << std::endl;
            return filenames;
        }

        // Iterate through directory contents
        for (const auto &entry : fs::directory_iterator(dirPath))
        {
            // Check if file path extension matches
            if (entry.path().extension() == extension)
            {
                filenames.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const fs::filesystem_error &ex)
    {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    return filenames;
}

void draw_wire_between(Bind *bind, const Cairo::RefPtr<Cairo::Context> &cr)
{
    // bind->wire
    if (bind->wire != nullptr)
    {
        if (bind->wire->breakPoints->size() != 0)
        {
            if (bind->gInput != nullptr)
            {
                cr->move_to(bind->gInput->x, bind->gInput->y);
                bind->wire->breakPoints->at(0).x = bind->gInput->x;
                bind->wire->breakPoints->at(0).y = bind->gInput->y;
            }
            else if (bind->output != nullptr)
            {
                cr->move_to(bind->output->x, bind->output->y);
                bind->wire->breakPoints->at(0).x = bind->output->x;
                bind->wire->breakPoints->at(0).y = bind->output->y;
            }
            if (bind->wire->breakPoints->size() != 2)
            {
                cr->line_to(bind->wire->breakPoints->at(1).x, bind->wire->breakPoints->at(1).y);
                cr->stroke();
                for (int i = 1; i < bind->wire->breakPoints->size() - 2; i++)
                {
                    cr->move_to(bind->wire->breakPoints->at(i).x, bind->wire->breakPoints->at(i).y);
                    cr->line_to(bind->wire->breakPoints->at(i + 1).x, bind->wire->breakPoints->at(i + 1).y);
                    cr->stroke();
                }
                // input pin line drawing
                cr->move_to(bind->input.x, bind->input.y);
                int index = bind->wire->breakPoints->size() - 2;
                cr->line_to(bind->wire->breakPoints->at(index).x, bind->wire->breakPoints->at(index).y);
                cr->stroke();
            }
            else
            {
                cr->line_to(bind->input.x, bind->input.y);
                cr->stroke();
            }
            int gIndex = bind->wire->breakPoints->size() - 1;
            bind->wire->breakPoints->at(gIndex).x = bind->input.x;
            bind->wire->breakPoints->at(gIndex).y = bind->input.y;
        }
    }
}

void draw_wire_between(BindToGlobalOutPut *bind, const Cairo::RefPtr<Cairo::Context> &cr)
{
    // bind->wire
    if (bind->wire != nullptr)
    {
        if (bind->wire->breakPoints->size() != 0)
        {
            if (bind->gInput != nullptr)
            {
                cr->move_to(bind->gInput->x, bind->gInput->y);
                bind->wire->breakPoints->at(0).x = bind->gInput->x;
                bind->wire->breakPoints->at(0).y = bind->gInput->y;
            }
            else if (bind->localOutput != nullptr)
            {
                cr->move_to(bind->localOutput->x, bind->localOutput->y);
                bind->wire->breakPoints->at(0).x = bind->localOutput->x;
                bind->wire->breakPoints->at(0).y = bind->localOutput->y;
            }
            if (bind->wire->breakPoints->size() != 2)
            {
                cr->line_to(bind->wire->breakPoints->at(1).x, bind->wire->breakPoints->at(1).y);
                cr->stroke();
                for (int i = 1; i < bind->wire->breakPoints->size() - 2; i++)
                {
                    cr->move_to(bind->wire->breakPoints->at(i).x, bind->wire->breakPoints->at(i).y);
                    cr->line_to(bind->wire->breakPoints->at(i + 1).x, bind->wire->breakPoints->at(i + 1).y);
                    cr->stroke();
                }
                // GlobalOutputPin line drawing
                cr->move_to(bind->output.x, bind->output.y);
                int index = bind->wire->breakPoints->size() - 2;
                cr->line_to(bind->wire->breakPoints->at(index).x, bind->wire->breakPoints->at(index).y);
                cr->stroke();
            }
            else
            {
                cr->line_to(bind->output.x, bind->output.y);
                cr->stroke();
            }
            int gIndex = bind->wire->breakPoints->size() - 1;
            bind->wire->breakPoints->at(gIndex).x = bind->output.x;
            bind->wire->breakPoints->at(gIndex).y = bind->output.y;
        }
    }
}

void ChipArea::call_save_popup()
{
    ActionMenu->save_circuit();
}

ChipArea::ChipArea(ScreenStack *stack)
{
    this->stack = stack;
    this->save_popup = new Dialog();
    this->save_popup->saveBtn->signal_clicked().connect(sigc::mem_fun(*this, &ChipArea::call_save_popup));
    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height = GetSystemMetrics(SM_CYSCREEN);

    overlay = Gtk::manage(new Gtk::Overlay());
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

    auto motion_controller = Gtk::EventControllerMotion::create();
    motion_controller->signal_motion().connect(sigc::mem_fun(*this, &ChipArea::on_my_motion));

    add_controller(motion_controller);

    auto key_controller = Gtk::EventControllerKey::create();
    key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &ChipArea::on_my_key_pressed), false);

    add_controller(key_controller);

    creationArea->append(*canvas);
    canvas->set_vexpand(true);
    canvas->set_hexpand(true);

    m_GestureDrag = Gtk::GestureDrag::create();
    m_GestureDrag->set_propagation_phase(Gtk::PropagationPhase::CAPTURE);
    m_GestureDrag->signal_drag_begin().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_begin));
    m_GestureDrag->signal_drag_end().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_end));
    m_GestureDrag->signal_drag_update().connect(sigc::mem_fun(*this, &ChipArea::on_my_drag_update));
    add_controller(m_GestureDrag);

    // m_GestureClick = Gtk::GestureClick::create();
    // m_GestureClick->set_propagation_phase(Gtk::PropagationPhase::BUBBLE);
    // m_GestureClick->signal_pressed().connect(sigc::mem_fun(*this, &ChipArea::onMyLeftClick));
    // add_controller(m_GestureClick);

    // connect it with canvas

    // now call the function to draw the chip
    canvas->set_draw_func(sigc::mem_fun(*this, &ChipArea::draw_on_canvas));

    draggedChip = nullptr;

    chips = new std::vector<Chip *>();
    int radius = 40;
    int bottomPos = height - 190 - radius - globalPinGap;
    int rightPos = width - margin - radius - globalPinGap;
    globalInputPinPlusAction = new PinPlusAction(radius, 40);
    globalInputPinMinusAction = new PinMinusAction(radius, bottomPos);

    globalOutputPinPlusAction = new PinPlusAction(rightPos, radius);
    globalOutputPinMinusAction = new PinMinusAction(rightPos, bottomPos);

    globalInputPins = new std::vector<GlobalInputPin *>();
    globalOutputPins = new std::vector<GlobalOutputPin *>();

    GlobalInputPin *globalInputPin = new GlobalInputPin(0, 100);
    globalInputPin->state = 1;
    GlobalInputPin *globalInputPin2 = new GlobalInputPin(1, 150);
    globalInputPin2->state = 0;

    GlobalOutputPin *globalOutputPin = new GlobalOutputPin(0, 100);

    globalInputPins->push_back(globalInputPin);
    globalInputPins->push_back(globalInputPin2);

    globalOutputPins->push_back(globalOutputPin);

    // chipSelector UI
    ActionMenu = new ChipSelectorMenu(width, height, stack);
    chipSelector = Gtk::manage(new ChipSelectorUI(this, ActionMenu));

    ActionMenu->hide();
    ActionMenu->visible = false;

    chipSelector->add_css_class({"chip-selector"});
    container->attach(*chipSelector, 0, 3, 1, 1);

    overlay->add_overlay(*ActionMenu);
    overlay->add_overlay(*save_popup);

    wrapper->append(*container);
    overlay->set_child(*wrapper);
    set_child(*overlay);
}

bool ChipArea::isHoveringLine(CordDouble mousePos, CordDouble A, CordDouble B, double tolerance)
{
    int minDiff = 10;
    double dx = B.x - A.x;
    double dy = B.y - A.y;
    bool isWithinXbounds = (mousePos.x > A.x && mousePos.x < B.x) || (mousePos.x > B.x && mousePos.x < A.x) || (abs(dx) < minDiff);
    bool isWithinYbounds = (mousePos.y > A.y && mousePos.y < B.y) || (mousePos.y > B.y && mousePos.y < A.y) || (abs(dy) < minDiff);

    double distance = fabs(dy * mousePos.x - dx * mousePos.y + B.x * A.y - B.y * A.x) / sqrt(pow(dy, 2) + pow(dx, 2));
    return distance <= tolerance && isWithinXbounds && isWithinYbounds;
}

bool ChipArea::isHoveringWire(CordDouble MousePos, Wire *wire, double tolerance)
{
    if (wire->breakPoints->size() > 1)
    {

        for (int point = 0; point < wire->breakPoints->size() - 1; point++)
        {
            if (isHoveringLine(mousePos, wire->breakPoints->at(point), wire->breakPoints->at(point + 1), tolerance))
            {
                return true;
            };
        }
    }
    return false;
}

void ChipArea::updateHoveringChipsPins(CordDouble mousePos)
{
    for (int i = 0; i < chips->size(); i++)
    {
        // for chips
        if (chips->at(i)->isHovered != chips->at(i)->isMouseHover(mousePos.x, mousePos.y))
        {
            chips->at(i)->isHovered = chips->at(i)->isMouseHover(mousePos.x, mousePos.y);
            // shouldQueueDraw = true; //commented
        }
        else
        {
        }

        // for input pins
        for (int j = 0; j < chips->at(i)->inputPins.size(); j++)
        {
            if (chips->at(i)->inputPins[j]->isHovered != chips->at(i)->inputPins[j]->isHovering(mousePos.x, mousePos.y))
            {
                chips->at(i)->inputPins[j]->isHovered = chips->at(i)->inputPins[j]->isHovering(mousePos.x, mousePos.y);
                shouldQueueDraw = true;
            };
        }

        // for output pins
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {
            if (chips->at(i)->outputPins[j]->isHovered != chips->at(i)->outputPins[j]->isHovering(mousePos.x, mousePos.y))
            {
                chips->at(i)->outputPins[j]->isHovered = chips->at(i)->outputPins[j]->isHovering(mousePos.x, mousePos.y);
                shouldQueueDraw = true;
            }
        }
    }
}

void ChipArea::updateHoveringWires(CordDouble mousePos)
{
    int tolerance = 10;
    for (int i = 0; i < chips->size(); i++)
    {
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {

            // hover for general wires
            if (chips->at(i)->outputPins[j]->binds->size() != 0)
            {
                for (int bind = 0; bind < (chips->at(i)->outputPins[j]->binds->size()); bind++)
                {
                    if (chips->at(i)->outputPins[j]->binds->at(bind)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->binds->at(bind)->isHovered != isHoveringWire(mousePos, chips->at(i)->outputPins[j]->binds->at(bind)->wire, tolerance))
                        {
                            chips->at(i)->outputPins[j]->binds->at(bind)->isHovered = isHoveringWire(mousePos, chips->at(i)->outputPins[j]->binds->at(bind)->wire, tolerance);
                            shouldQueueDraw = true;
                            break; // break after change to be made is found
                        }
                    }
                }
            }

            // wires from chip's outputs to globaloutputs
            if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size() > 0)
            {
                for (int bindToGlobalOutput = 0; bindToGlobalOutput < (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size()); bindToGlobalOutput++)
                {

                    if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isHovered != isHoveringWire(mousePos, chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire, tolerance))
                        {
                            chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isHovered = isHoveringWire(mousePos, chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire, tolerance);
                            shouldQueueDraw = true;
                            break; // break after change to be made is found
                        }
                    }
                }
            }

            // wires from global input pins
            for (int pin = 0; pin < globalInputPins->size(); pin++)
            {
                for (int bind = 0; bind < globalInputPins->at(pin)->binds->size(); bind++)
                {
                    if (globalInputPins->at(pin)->binds->at(bind)->isHovered != isHoveringWire(mousePos, globalInputPins->at(pin)->binds->at(bind)->wire, tolerance))
                    {
                        globalInputPins->at(pin)->binds->at(bind)->isHovered = isHoveringWire(mousePos, globalInputPins->at(pin)->binds->at(bind)->wire, tolerance);
                        shouldQueueDraw = true;
                        break;
                    }
                }
            }
        }
    }
}

void ChipArea::updateClickedWires(CordDouble mousePos)
{
    for (int i = 0; i < chips->size(); i++)
    {
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {

            // chips' outputs' wires
            if (chips->at(i)->outputPins[j]->binds->size() != 0)
            {
                for (int bind = 0; bind < (chips->at(i)->outputPins[j]->binds->size()); bind++)
                {
                    if (chips->at(i)->outputPins[j]->binds->at(bind)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->binds->at(bind)->isHovered)
                        {
                            chips->at(i)->outputPins[j]->binds->at(bind)->isClicked = true;
                        }
                        else
                        {
                            chips->at(i)->outputPins[j]->binds->at(bind)->isClicked = false;
                        }
                    }
                }
            }

            // wires from chip's outputs to globaloutputs
            if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size() > 0)
            {
                for (int bindToGlobalOutput = 0; bindToGlobalOutput < (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size()); bindToGlobalOutput++)
                {

                    if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isHovered)
                        {
                            chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isClicked = true;
                        }
                        else
                        {
                            chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isClicked = false;
                        }
                    }
                }
            }

            // wires from globalInputs
            for (int pin = 0; pin < globalInputPins->size(); pin++)
            {
                for (int bind = 0; bind < globalInputPins->at(pin)->binds->size(); bind++)
                {
                    if (globalInputPins->at(pin)->binds->at(bind)->isHovered)
                    {

                        globalInputPins->at(pin)->binds->at(bind)->isClicked = true;
                    }
                    else
                    {
                        globalInputPins->at(pin)->binds->at(bind)->isClicked = false;
                    }
                }
            }
        }
    }
}

void ChipArea::on_my_motion(double x, double y)
{
    mousePos = CordDouble{x - margin, y - margin};
    shouldQueueDraw = false;

    updateHoveringChipsPins(mousePos);
    updateHoveringWires(mousePos);

    if (draggedWire != nullptr)
    {
        shouldQueueDraw = true;
    }

    if (shouldQueueDraw)
    {
        canvas->queue_draw();
    }
}
void ChipArea::clear_actions()
{
    draggedGlobalInputPin = nullptr;
    draggedOutputPin = nullptr;
    draggedGlobalOutputPin = nullptr;
    draggedInputPin = nullptr;
    if (draggedWire != nullptr)
    {
        draggedWire->breakPoints->clear();
        draggedWire = nullptr;
    }
    canvas->queue_draw();
}

bool ChipArea::on_my_key_pressed(guint keyval, guint /*keycode*/, Gdk::ModifierType /*state*/)
{
    if (keyval == GDK_KEY_Escape)
    {
        clear_actions();
        return true; // Event has been handled
    }

    if (keyval == GDK_KEY_KP_Delete || keyval == GDK_KEY_Delete || keyval == GDK_KEY_BackSpace)
    {
        onMyDeleteKeyPressed();
    }

    return false; // Event has not been handled
}

void ChipArea::clear_all(){
    for (int i = 0; i < chips->size(); i++)
    {
        delete chips->at(i);
    }
    chips->clear();
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        delete globalInputPins->at(i);
    }
    globalInputPins->clear();
    for (int i = 0; i < globalOutputPins->size(); i++)
    {
        delete globalOutputPins->at(i);
    }
    globalOutputPins->clear();
    // delete all (except first three ) buttons of chipSelectorUI
    std::vector<Gtk::Widget *> childs = chipSelector->get_children();
    for (int i = 3; i < childs.size(); i++)
    {
        chipSelector->remove(*childs[i]);
    }
    clear_actions();

}
void ChipArea::onMyLeftClick(int n_press, double x, double y)
{
    std::cout << "left click registered" << std::endl;

    // if a wire is dragged
    if (draggedGlobalInputPin != nullptr)
    {
        if (draggedWire != nullptr)
        {
            bool isPinFound = false;
            // check global output pin
            for (int i = 0; i < globalOutputPins->size(); i++)
            {
                if (globalOutputPins->at(i)->isInside(x - margin, y - margin))
                {
                    // draggedGlobalInputPin->bindToGlobalOutput(*globalOutputPins->at(i));
                    // Bind bind(inputPin);
                    // bind.gInput = this;
                    // binds->push_back(bind);

                    BindToGlobalOutPut *bind = new BindToGlobalOutPut((*globalOutputPins->at(i)));
                    bind->gInput = draggedGlobalInputPin;
                    draggedGlobalInputPin->gbinds->push_back(bind);
                    isPinFound = true;
                    CordDouble c1 = CordDouble{globalOutputPins->at(i)->x, globalOutputPins->at(i)->y};
                    draggedWire->breakPoints->push_back(c1);
                    Wire *wire = new Wire();
                    // wire->breakPoints = draggedWire->breakPoints;
                    for (int i = 0; i < draggedWire->breakPoints->size(); i++)
                    {
                        wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                    }
                    bind->wire = wire;
                    globalOutputPins->at(i)->bindToGlobalOutput = bind;
                    clear_actions();
                    run();
                    return;
                }
            }

            // check input pin
            for (int i = 0; i < chips->size(); i++)
            {
                for (int j = 0; j < chips->at(i)->inputPins.size(); j++)
                {
                    if (chips->at(i)->inputPins[j]->isInside(x - margin, y - margin))
                    {
                        Bind *bind = new Bind(*chips->at(i)->inputPins[j]);
                        bind->gInput = draggedGlobalInputPin;
                        draggedGlobalInputPin->binds->push_back(bind);
                        isPinFound = true;
                        CordDouble c1 = CordDouble{chips->at(i)->inputPins[j]->x, chips->at(i)->inputPins[j]->y};
                        draggedWire->breakPoints->push_back(c1);
                        Wire *wire = new Wire();
                        for (int i = 0; i < draggedWire->breakPoints->size(); i++)
                        {
                            wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                        }
                        bind->wire = wire;
                        chips->at(i)->inputPins[j]->bind = bind;
                        clear_actions();
                        run();
                        return;
                    }
                }
            }

            if (isPinFound == false)
            {
                CordDouble c1 = CordDouble{x - margin, y - margin};
                draggedWire->breakPoints->push_back(c1);
            }
        }
    }
    else if (draggedOutputPin != nullptr)
    {
        if (draggedWire != nullptr)
        {
            // check global output pin
            for (int i = 0; i < globalOutputPins->size(); i++)
            {
                if (globalOutputPins->at(i)->isInside(x - margin, y - margin))
                {
                    draggedOutputPin->bindToGlobalOutput(*globalOutputPins->at(i));
                    CordDouble c1 = CordDouble{globalOutputPins->at(i)->x, globalOutputPins->at(i)->y};
                    draggedWire->breakPoints->push_back(c1);
                    Wire *wire = new Wire();
                    for (int i = 0; i < draggedWire->breakPoints->size(); i++)
                    {
                        wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                    }
                    draggedOutputPin->bindsToGlobalOutput->at(draggedOutputPin->bindsToGlobalOutput->size() - 1)->wire = wire;
                    clear_actions();
                    run();
                    return;
                }
            }

            // check input pins
            for (int i = 0; i < chips->size(); i++)
            {
                for (int j = 0; j < chips->at(i)->inputPins.size(); j++)
                {
                    if (chips->at(i)->inputPins[j]->isInside(x - margin, y - margin))
                    {
                        draggedOutputPin->bindTo(*chips->at(i)->inputPins[j]);
                        CordDouble c1 = CordDouble{chips->at(i)->inputPins[j]->x, chips->at(i)->inputPins[j]->y};
                        draggedWire->breakPoints->push_back(c1);
                        Wire *wire = new Wire();
                        for (int i = 0; i < draggedWire->breakPoints->size(); i++)
                        {
                            wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                        }
                        draggedOutputPin->binds->at(draggedOutputPin->binds->size() - 1)->wire = wire;
                        clear_actions();
                        run();
                        return;
                    }
                }
            }

            // no pin is found
            CordDouble c1 = CordDouble{x - margin, y - margin};
            draggedWire->breakPoints->push_back(c1);
        }
    }
    else if (draggedGlobalOutputPin != nullptr)
    {
        if (draggedWire != nullptr)
        {
            // check global input pins
            for (int i = 0; i < globalInputPins->size(); i++)
            {
                if (globalInputPins->at(i)->isInside(x - margin, y - margin))
                {
                    BindToGlobalOutPut *bind = new BindToGlobalOutPut(*draggedGlobalOutputPin);
                    bind->gInput = globalInputPins->at(i);
                    globalInputPins->at(i)->gbinds->push_back(bind);
                    draggedGlobalOutputPin->bindToGlobalOutput = bind;
                    CordDouble c1 = CordDouble{globalInputPins->at(i)->x, globalInputPins->at(i)->y};
                    draggedWire->breakPoints->push_back(c1);
                    Wire *wire = new Wire();
                    for (int i = draggedWire->breakPoints->size() - 1; i >= 0; i--)
                    {
                        wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                    }
                    bind->wire = wire;
                    clear_actions();
                    run();
                    return;
                }
            }

            // check output pins
            for (int i = 0; i < chips->size(); i++)
            {
                for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
                {
                    if (chips->at(i)->outputPins[j]->isInside(x - margin, y - margin))
                    {
                        BindToGlobalOutPut *bind = new BindToGlobalOutPut(*draggedGlobalOutputPin);
                        bind->localOutput = chips->at(i)->outputPins[j];
                        chips->at(i)->outputPins[j]->bindsToGlobalOutput->push_back(bind);
                        draggedGlobalOutputPin->bindToGlobalOutput = bind;
                        CordDouble c1 = CordDouble{chips->at(i)->outputPins[j]->x, chips->at(i)->outputPins[j]->y};
                        draggedWire->breakPoints->push_back(c1);
                        Wire *wire = new Wire();
                        for (int i = draggedWire->breakPoints->size() - 1; i >= 0; i--)
                        {
                            wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                        }
                        bind->wire = wire;
                        clear_actions();
                        run();
                        return;
                    }
                }
            }

            // no pin is found
            CordDouble c1 = CordDouble{x - margin, y - margin};
            draggedWire->breakPoints->push_back(c1);
        }
    }
    else if (draggedInputPin != nullptr)
    {
        if (draggedWire != nullptr)
        {
            // check global input pins
            for (int i = 0; i < globalInputPins->size(); i++)
            {
                if (globalInputPins->at(i)->isInside(x - margin, y - margin))
                {
                    globalInputPins->at(i)->bindTo(*draggedInputPin);
                    CordDouble c1 = CordDouble{globalInputPins->at(i)->x, globalInputPins->at(i)->y};
                    draggedWire->breakPoints->push_back(c1);
                    Wire *wire = new Wire();
                    for (int i = draggedWire->breakPoints->size() - 1; i >= 0; i--)
                    {
                        wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                    }
                    globalInputPins->at(i)->binds->at(globalInputPins->at(i)->binds->size() - 1)->wire = wire;
                    clear_actions();
                    run();
                    return;
                }
            }

            // check output pins
            for (int i = 0; i < chips->size(); i++)
            {
                for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
                {
                    if (chips->at(i)->outputPins[j]->isInside(x - margin, y - margin))
                    {
                        chips->at(i)->outputPins[j]->bindTo(*draggedInputPin);
                        CordDouble c1 = CordDouble{chips->at(i)->outputPins[j]->x, chips->at(i)->outputPins[j]->y};
                        draggedWire->breakPoints->push_back(c1);
                        Wire *wire = new Wire();
                        for (int i = draggedWire->breakPoints->size() - 1; i >= 0; i--)
                        {
                            wire->breakPoints->push_back(draggedWire->breakPoints->at(i));
                        }
                        chips->at(i)->outputPins[j]->binds->at(chips->at(i)->outputPins[j]->binds->size() - 1)->wire = wire;
                        clear_actions();
                        run();
                        return;
                    }
                }
            }

            // if no pin is found
            CordDouble c1 = CordDouble{x - margin, y - margin};
            draggedWire->breakPoints->push_back(c1);
        }
    }

    // wire is not dragging
    else
    {
        updateClickedWires(mousePos);
        bool isPinFound = false;

        // first check global input pins
        for (int i = 0; i < globalInputPins->size(); i++)
        {

            if (globalInputPins->at(i)->isInside(x - margin, y - margin))
            {
                draggedGlobalInputPin = globalInputPins->at(i);
                draggedWire = new Wire();
                draggedWire->breakPoints->push_back(CordDouble{double(draggedGlobalInputPin->x), double(draggedGlobalInputPin->y)});
                run();
                return;
            }
        }

        // now check all chips
        for (int i = 0; i < chips->size(); i++)
        {

            // check output pins
            for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
            {

                if (chips->at(i)->outputPins[j]->isInside(x - margin, y - margin))
                {
                    isPinFound = true;
                    draggedOutputPin = chips->at(i)->outputPins[j];
                    draggedWire = new Wire();
                    draggedWire->breakPoints->push_back(CordDouble{double(draggedOutputPin->x), double(draggedOutputPin->y)});
                    run();
                    std::cout << "here1123" << std::endl;
                    return;
                }
            }

            // check input pins
            if (!isPinFound)
            {
                for (int j = 0; j < chips->at(i)->inputPins.size(); j++)
                {
                    if (chips->at(i)->inputPins[j]->isInside(x - margin, y - margin))
                    {
                        isPinFound = true;
                        draggedInputPin = chips->at(i)->inputPins[j];
                        draggedWire = new Wire();
                        draggedWire->breakPoints->push_back(CordDouble{double(draggedInputPin->x), double(draggedInputPin->y)});
                        run();
                        return;
                    }
                }
            }

            // so that pin dragging of wire and chip is not simultaneous
            if (!isPinFound)
            {
                if (chips->at(i)->isMouseInside(x - margin, y - margin))
                {
                    chips->at(i)->isClicked = true;
                }
                else
                {
                    chips->at(i)->isClicked = false;
                }
            }
        }

        // now check global output pins
        for (int i = 0; i < globalOutputPins->size(); i++)
        {
            if (globalOutputPins->at(i)->isInside(x - margin, y - margin))
            {
                draggedGlobalOutputPin = globalOutputPins->at(i);
                draggedWire = new Wire();
                draggedWire->breakPoints->push_back(CordDouble{double(draggedGlobalOutputPin->x), double(draggedGlobalOutputPin->y)});
                run();
                return;
            }
        }
    }

    for (int i = 0; i < globalInputPins->size(); i++)
    {

        if (globalInputPins->at(i)->IsToggleBtnInside(x - margin, y - margin))
        {
            globalInputPins->at(i)->state = !globalInputPins->at(i)->state;
            run();
            return;
        }
    }

    if (globalInputPinPlusAction->isMouseInside(x - margin, y - margin))
    {
        int size = globalInputPins->size();
        int y = (size == 0 ? 100 : 100 + (size * 50));
        GlobalInputPin *globalInputPin = new GlobalInputPin(0, y);
        globalInputPins->push_back(globalInputPin);
        run();
        return;
    }
    else if (globalInputPinMinusAction->isMouseInside(x - margin, y - margin))
    {
        if (globalInputPins->size() > 0)
        {
            // if this pin has bind dont delete
            if (globalInputPins->at(globalInputPins->size() - 1)->binds->size() == 0 && globalInputPins->at(globalInputPins->size() - 1)->gbinds->size() == 0)
            {
                globalInputPins->pop_back();
            }
        }
        run();
        return;
    }
    if (globalOutputPinPlusAction->isMouseInside(x - margin, y - margin))
    {
        int size = globalOutputPins->size();
        int y = (size == 0 ? 100 : 100 + (size * 50));
        GlobalOutputPin *globalOutputPin = new GlobalOutputPin(size, y);
        globalOutputPins->push_back(globalOutputPin);
        run();
        return;
    }
    else if (globalOutputPinMinusAction->isMouseInside(x - margin, y - margin))
    {
        if (globalOutputPins->size() > 0)
        {
            // if this pin has bind dont delete
            if (globalOutputPins->at(globalOutputPins->size() - 1)->bindToGlobalOutput == nullptr)
            {
                globalOutputPins->pop_back();
            }
        }
        run();
        return;
    }
    run();
}

void ChipArea::onMyDeleteKeyPressed()
{
    for (int i = 0; i < chips->size(); i++)
    {
        // CHIPS //
        if (chips->at(i)->isClicked)
        {
            auto it = chips->begin() + i;
            delete chips->at(i);
            chips->erase(it);
            run();
            canvas->queue_draw();
            return;
        }

        // WIRES//

        // wires from outputs
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {

            // outputs to other's input
            if (chips->at(i)->outputPins[j]->binds->size() != 0)
            {
                for (int bind = 0; bind < (chips->at(i)->outputPins[j]->binds->size()); bind++)
                {
                    if (chips->at(i)->outputPins[j]->binds->at(bind)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->binds->at(bind)->isClicked)
                        {
                            delete chips->at(i)->outputPins[j]->binds->at(bind);

                            // find here
                            chips->at(i)->outputPins[j]->binds->at(bind)->input.bind = nullptr;
                            auto it = chips->at(i)->outputPins[j]->binds->begin() + bind;
                            chips->at(i)->outputPins[j]->binds->erase(it);
                            canvas->queue_draw();
                            return;
                        }
                        else
                        {
                        }
                    }
                }
            }

            // wires from chip's outputs to globaloutputs
            if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size() > 0)
            {
                for (int bindToGlobalOutput = 0; bindToGlobalOutput < (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size()); bindToGlobalOutput++)
                {

                    if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire != nullptr)
                    {
                        if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isClicked)
                        {
                            chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->output.bindToGlobalOutput = nullptr;
                            delete chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput);
                            auto it = chips->at(i)->outputPins[j]->bindsToGlobalOutput->begin() + bindToGlobalOutput;
                            chips->at(i)->outputPins[j]->bindsToGlobalOutput->erase(it);
                            canvas->queue_draw();
                            return;
                        }
                        else
                        {
                        }
                    }
                }
            }

            // wires from globalInputs
            for (int pin = 0; pin < globalInputPins->size(); pin++)
            {
                for (int bind = 0; bind < globalInputPins->at(pin)->binds->size(); bind++)
                {
                    if (globalInputPins->at(pin)->binds->at(bind)->isClicked)
                    {
                        globalInputPins->at(pin)->binds->at(bind)->input.bind = nullptr;
                        delete globalInputPins->at(pin)->binds->at(bind);
                        auto it = globalInputPins->at(pin)->binds->begin() + bind;
                        globalInputPins->at(pin)->binds->erase(it);
                        canvas->queue_draw();
                        return;
                    }
                    else
                    {
                    }
                }
            }
        }
    }
}

void ChipArea::on_my_drag_begin(double start_x, double start_y)
{
    std::cout << "drag begins" << std::endl;
    onMyLeftClick(1, start_x, start_y);
    for (int i = 0; i < chips->size(); i++)
    {

        bool isDragged = (*chips)[i]->isMouseInside(start_x - margin, start_y - margin) && (*chips)[i]->isClicked;
        if (isDragged)
        {
            // draggedChip = &(*chips)[i];
            draggedChip = chips->at(i);

            MouseOffset offset = chips->at(i)->getMouseOffset(start_x - margin, start_y - margin);
            // print the mouse offset

            double new_x = (start_x - margin) - chips->at(i)->structure->boundingBox->x - offset.x;
            double new_y = (start_y - margin) - chips->at(i)->structure->boundingBox->y - offset.y;

            // print new_x and new_y
            chips->at(i)->structure->boundingBox->x += new_x;
            chips->at(i)->structure->boundingBox->y += new_y;

            chips->at(i)->structure->boundingBox->intial_x += new_x;
            chips->at(i)->structure->boundingBox->intial_y += new_y;
            break;
        }
    }
    canvas->queue_draw();
}

void ChipArea::on_my_drag_update(double offset_x, double offset_y)
{

    // offset_x	X offset, relative to the start point.
    // offset_y	Y offset, relative to the start point.

    if (draggedChip == nullptr)
    {
        return;
    }

    double new_x = draggedChip->structure->boundingBox->intial_x + offset_x;
    double new_y = draggedChip->structure->boundingBox->intial_y + offset_y;

    draggedChip->structure->boundingBox->x = new_x;
    draggedChip->structure->boundingBox->y = new_y;

    canvas->queue_draw();
}

void ChipArea::on_my_drag_end(double offset_x, double offset_y)
{
    // use draggedChip
    if (draggedChip == nullptr)
    {
        return;
    }
    double new_x = draggedChip->structure->boundingBox->intial_x + offset_x;
    double new_y = draggedChip->structure->boundingBox->intial_y + offset_y;
    draggedChip->structure->setLoc(new_x, new_y);
    // clear draggedChip
    draggedChip = nullptr;
    // chips->at(i)->isDragging = false;
    // double new_x = chips->at(i)->structure->boundingBox->x + offset_x - chips->at(i)->structure->boundingBox->intial_x;
    // double new_y = chips->at(i)->structure->boundingBox->y + offset_y - chips->at(i)->structure->boundingBox->intial_y;
    // chips->at(i)->structure->setLoc(new_x, new_y);

    canvas->queue_draw();
}

void ChipArea::createAndChip(int index, int posX, int posY)
{
    ChipStructure *structureAND = new ChipStructure(new ChipBoundingBox{100, 100, posX, posY});
    std::vector<InputPin *> inputPins;
    std::vector<OutputPin *> outputPins;
    InputPin *inputPinAND_A = new InputPin("A", 0);
    InputPin *inputPinAND_B = new InputPin("B", 1);
    OutputPin *outputPinAND_Y = new OutputPin("Y", 0);

    inputPins.push_back(inputPinAND_A);
    inputPins.push_back(inputPinAND_B);
    outputPins.push_back(outputPinAND_Y);

    Chip *chipAND = new Chip(structureAND, inputPins, outputPins, "AND");
    chipAND->setChipType(ChipType::AND);
    addChip(chipAND);
    canvas->queue_draw();
}

void ChipArea::createNotChip(int index, int posX, int posY)
{
    ChipStructure *structureNOT = new ChipStructure(new ChipBoundingBox{100, 100, posX, posY});

    InputPin *inputPinNOT_A = new InputPin("A", 0);
    OutputPin *outputPinNOT_Y = new OutputPin("Y", 0);

    std::vector<InputPin *> inputPins;
    std::vector<OutputPin *> outputPins;

    inputPins.push_back(inputPinNOT_A);
    outputPins.push_back(outputPinNOT_Y);

    Chip *chiNOT = new Chip(structureNOT, inputPins, outputPins, "NOT");
    chiNOT->setChipType(ChipType::NOT);
    addChip(chiNOT);
    canvas->queue_draw();
}

void ChipArea::clear_canvas(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->fill();
}

void ChipArea::run()
{

    std::vector<Chip *> toRunChips = std::vector<Chip *>();

    for (int i = 0; i < globalInputPins->size(); i++)
    {
        for (int j = 0; j < globalInputPins->at(i)->binds->size(); j++)
        {
            globalInputPins->at(i)->binds->at(j)->input.state = globalInputPins->at(i)->state;
            if (globalInputPins->at(i)->binds->at(j)->input.chip != nullptr)
            {
                toRunChips.push_back(globalInputPins->at(i)->binds->at(j)->input.chip);
            }
        }
        for (int j = 0; j < globalInputPins->at(i)->gbinds->size(); j++)
        {
            globalInputPins->at(i)->gbinds->at(j)->output.state = globalInputPins->at(i)->state;
        }
    }

    for (int i = 0; i < toRunChips.size(); i++)
    {
        toRunChips[i]->run();
    }
}

void ChipArea::draw_on_canvas(const Cairo::RefPtr<Cairo::Context> &cr,
                              int width, int height)
{
    clear_canvas(cr);
    // draw globalInputPinPlusAction
    globalInputPinPlusAction->draw(cr);
    globalInputPinMinusAction->draw(cr);

    globalOutputPinPlusAction->draw(cr);
    globalOutputPinMinusAction->draw(cr);
    // draw dragged wire
    if (draggedWire != nullptr)
    {
        cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
        cr->set_line_width(4);

        if (draggedWire->breakPoints->size() != 0)
        {
            for (int i = 0; i < draggedWire->breakPoints->size() - 1; i++)
            {
                cr->move_to(draggedWire->breakPoints->at(i).x, draggedWire->breakPoints->at(i).y);
                cr->line_to(draggedWire->breakPoints->at(i + 1).x, draggedWire->breakPoints->at(i + 1).y);
                cr->stroke();
            }
            int index = draggedWire->breakPoints->size();
            cr->move_to(draggedWire->breakPoints->at(index - 1).x, draggedWire->breakPoints->at(index - 1).y);
            cr->line_to(mousePos.x, mousePos.y);
            cr->stroke();
        }
    }

    // global output pins
    for (int i = 0; i < globalOutputPins->size(); i++)
    {

        globalOutputPins->at(i)->radius = 20;
        cr->set_source_rgb(200 / 255.0, 39 / 255.0, 92 / 255.0);
        cr->arc(width - globalOutputPins->at(i)->radius - globalPinGap, globalOutputPins->at(i)->y, globalOutputPins->at(i)->radius, 0, 2 * M_PI);
        cr->fill();

        if (globalOutputPins->at(i)->state == 1)
        {
            cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
        }
        else
        {
            cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
        }
        cr->arc(width - globalOutputPins->at(i)->radius - globalPinGap, globalOutputPins->at(i)->y, globalOutputPins->at(i)->radius / 2, 0, 2 * M_PI);
        cr->fill();
        globalOutputPins->at(i)->x = width - globalOutputPins->at(i)->radius - globalPinGap;
    }
    for (int i = 0; i < chips->size(); i++)
    {
        chips->at(i)->draw(cr);
    }

    // global input pins
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        globalInputPins->at(i)->radius = 20;
        cr->set_source_rgb(200 / 255.0, 39 / 255.0, 92 / 255.0);
        cr->arc(globalInputPins->at(i)->radius + globalPinGap, globalInputPins->at(i)->y, globalInputPins->at(i)->radius, 0, 2 * M_PI);
        cr->fill();

        if (globalInputPins->at(i)->state == 1)
        {
            cr->set_source_rgb(2550 / 255.0, 255 / 255.0, 255 / 255.0);
        }
        else
        {
            cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
        }

        cr->arc(globalInputPins->at(i)->radius + globalPinGap, globalInputPins->at(i)->y, globalInputPins->at(i)->radius / 2, 0, 2 * M_PI);
        cr->fill();
        globalInputPins->at(i)->x = globalInputPins->at(i)->radius + globalPinGap;

        // rectange before the global input pin
        if (globalInputPins->at(i)->state == 1)
        {
            cr->set_source_rgb(200 / 255.0, 50 / 255.0, 100 / 255.0);
        }
        else
        {
        }
        int rectangeWidth = 20;
        int rectangeHeight = 20;
        // globalInputPins->at(i)->boundingBox->width = rectangeWidth;
        // globalInputPins->at(i)->boundingBox->height = rectangeHeight;
        // globalInputPins->at(i)->boundingBox->x = globalInputPins->at(i)->x - globalInputPins->at(i)->radius - rectangeWidth - 10;
        // globalInputPins->at(i)->boundingBox->y = globalInputPins->at(i)->y - (rectangeHeight / 2);

        globalInputPins->at(i)->boundingBox = new ChipBoundingBox{rectangeWidth, rectangeHeight, globalInputPins->at(i)->x - globalInputPins->at(i)->radius - rectangeWidth - 10, globalInputPins->at(i)->y - (rectangeHeight / 2)};
        cr->rectangle(globalInputPins->at(i)->boundingBox->x, globalInputPins->at(i)->boundingBox->y, rectangeWidth, rectangeHeight);
        cr->fill();
    }

    // draw line between GlobalInputPin and input pins
    for (int i = 0; i < globalInputPins->size(); i++)
    {

        for (int j = 0; j < globalInputPins->at(i)->binds->size(); j++)
        {
            if (globalInputPins->at(i)->binds->at(j)->isClicked)
            {
                cr->set_source_rgb(255.0 / 255.0, 0, 0);
                cr->set_line_width(8);
                draw_wire_between(globalInputPins->at(i)->binds->at(j), cr);
            }

            cr->set_line_width(4);
            if (globalInputPins->at(i)->binds->at(j)->isHovered)
            {
                cr->set_line_width(6);
            }
            else
            {
            }

            if (globalInputPins->at(i)->state == 1)
            {
                cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
            }
            else
            {
                cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
            }

            draw_wire_between(globalInputPins->at(i)->binds->at(j), cr);
        }
    }

    // draw line between globalInputPin and global output pins
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        for (int j = 0; j < globalInputPins->at(i)->gbinds->size(); j++)
        {
            if (globalInputPins->at(i)->state == 1)
            {
                cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
            }
            else
            {
                cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
            }
            cr->set_line_width(4);

            draw_wire_between(globalInputPins->at(i)->gbinds->at(j), cr);
        }
    }

    // chips
    for (int i = 0; i < chips->size(); i++)
    {
        chips->at(i)->draw(cr);
    }
}

ChipSelectorUI::ChipSelectorUI(ChipArea *area, ChipSelectorMenu *menu)
{
    set_css_classes({"chip-selector"});
    set_orientation(Gtk::Orientation::HORIZONTAL);
    set_spacing(0);

    // menu button
    menu_btn = Gtk::manage(new Gtk::Button());
    menu_btn->set_label("MENU");
    menu_btn->set_size_request(100, 50);
    menu_btn->set_css_classes({"chip-menu-btn", "chip-btn"});
    menu_btn->signal_clicked().connect(sigc::mem_fun(*menu, ChipSelectorMenu::showMenu));
    append(*menu_btn);

    // and gate
    Gtk::Button *and_btn;
    and_btn = Gtk::manage(new Gtk::Button());
    and_btn->set_label("AND");
    and_btn->set_size_request(80, 50);
    and_btn->set_css_classes({"chip-btn"});
    and_btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*area, ChipArea::createAndChip), 0, 50, (area->height) - 300));
    append(*and_btn);

    // not gate
    Gtk::Button *not_btn;
    not_btn = Gtk::manage(new Gtk::Button());
    not_btn->set_label("NOT");
    not_btn->set_size_request(80, 50);
    not_btn->set_css_classes({"chip-btn"});
    not_btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*area, ChipArea::createNotChip), 0, 250, (area->height) - 300));
    append(*not_btn);

    // for (int i = 0; i < 5; i++)
    // {
    //     chips->at(i) = Gtk::manage(new Gtk::Button());
    //     chips->at(i)->set_label("Chip " + std::to_string(i));
    //     chips->at(i)->set_size_request(50, 50);
    //     chips->at(i)->set_css_classes({"chip-btn"});
    //     append(*chips->at(i));
    // }
};

void ChipSelectorUI::on_chip_selected(int index)
{
    selected_chip = index;
}

BindToGlobalOutPut::BindToGlobalOutPut(GlobalOutputPin &output) : output(output){};

void Bind::printConnection()
{
    // output name and output pin x, y
    // input name and input pin x, y
    // std::cout << "Output Name: " << input.x << " Output Pin X: " << input.x << " Output Pin Y: " << input.y << std::endl;
}

Pin::Pin(std::string name, int index)
{
    this->name = name;
    this->index = index;
}

// done
InputPin::~InputPin()
{

    if (bind != nullptr)
    {
        // from global inputs
        if (bind->gInput != nullptr)
        {
            for (int i = 0; i < bind->gInput->binds->size(); ++i)
            {
                if (&(bind->gInput->binds->at(i)->input) == this)
                {
                    auto it = bind->gInput->binds->begin() + i;
                    // delete bind->gInput->binds->at(i);
                    bind->gInput->binds->erase(it);
                    break; // wires are handled in the destructor
                }
            }
        }

        // from chip outputs
        if (bind->output != nullptr)
        {
            for (int i = 0; i < bind->output->binds->size(); ++i)
            {
                if (&(bind->output->binds->at(i)->input) == this)
                {
                    auto it = bind->output->binds->begin() + i;
                    // delete bind->output->binds->at(i);
                    bind->output->binds->erase(it);
                    break; // wires are handled in the destructor
                }
            }
        }

        delete bind;
    }
}

OutputPin::~OutputPin()
{

    // normal pins
    if (binds != nullptr)
    {
        auto it = binds->begin();
        for (int i = 0; i < binds->size(); i++)
        {
            binds->at(i)->input.state = 0;
            binds->at(i)->input.bind = nullptr;
            delete binds->at(i);
            binds->at(i) = nullptr;
        }
        delete binds;
    }

    // pins binded with global output
    if (bindsToGlobalOutput != nullptr)
    {
        auto it = bindsToGlobalOutput->begin();
        for (int i = 0; i < bindsToGlobalOutput->size(); i++)
        {
            bindsToGlobalOutput->at(i)->output.state = 0;
            bindsToGlobalOutput->at(i)->output.bindToGlobalOutput = nullptr;
            delete bindsToGlobalOutput->at(i);
        }
        delete bindsToGlobalOutput;
    }
}

BindToGlobalOutPut::~BindToGlobalOutPut()
{
    output.state = 0;
    if (wire != nullptr)
    {
        delete wire;
    }
}
// if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size() > 0)
// {
//     for (int bindToGlobalOutput = 0; bindToGlobalOutput < (chips->at(i)->outputPins[j]->bindsToGlobalOutput->size()); bindToGlobalOutput++)
//     {

//         if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->wire != nullptr)
//         {
//             if (chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput)->isClicked)
//             {
//                 delete chips->at(i)->outputPins[j]->bindsToGlobalOutput->at(bindToGlobalOutput);
//                 auto it = chips->at(i)->outputPins[j]->bindsToGlobalOutput->begin() + bindToGlobalOutput;
//                 chips->at(i)->outputPins[j]->bindsToGlobalOutput->erase(it);
//                 canvas->queue_draw();
//                 return;
//             }
//             else
//             {
//             }
//         }
//     }
// }
void Pin::printCord()
{
    // std::cout << "Pin X: " << x << " Pin Y: " << y << std::endl;
}

// InputPin::InputPin(std::string name, int index)
// {
//     this->name = name;
//     this->index = index;
// }

void Pin::setCord(int xP, int yP)
{
    this->x = xP;
    this->y = yP;
}

bool Pin::isInside(int mouseX, int mouseY)
{

    if ((mouseX >= (x - radius)) && (mouseX <= (x + radius)) && (mouseY >= (y - radius)) && (mouseY <= (y + radius)))
    {
        return true;
    }
    return false;
}

bool Pin::isHovering(int mouseX, int mouseY)
{
    return (mouseX >= (x - radius - hoverRange)) && (mouseX <= (x + radius + hoverRange)) && (mouseY >= (y - radius - hoverRange)) && (mouseY <= (y + radius + hoverRange));
}

void Pin::setRadius(int radius)
{
    this->radius = radius;
}

Cord Pin::getCord()
{
    return Cord{x, y};
}

void Pin::drawHovered(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
    cr->arc(x, y, radius + 3, 0, 2 * M_PI);
    cr->fill();
}

void Pin::drawNormal(const Cairo::RefPtr<Cairo::Context> &cr)
{
    if (state == 1)
    {
        cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
    }
    else
    {
        cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
    }
    cr->arc(x, y, radius, 0, 2 * M_PI);
    cr->fill();
}

void OutputPin::bindTo(InputPin &inputPin)
{
    Bind *bind = new Bind(inputPin);
    bind->output = this;
    inputPin.bind = bind;
    binds->push_back(bind);
}

void OutputPin::bindToGlobalOutput(GlobalOutputPin &output)
{
    BindToGlobalOutPut *bind = new BindToGlobalOutPut(output);
    bind->localOutput = this;
    bindsToGlobalOutput->push_back(bind);
    output.bindToGlobalOutput = bind;
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

CustomChip::CustomChip(std::vector<GlobalInputPin *> *globalInputPins, std::vector<GlobalOutputPin *> *globalOutputPins)
{
    this->globalInputPins = globalInputPins;
    this->globalOutputPins = globalOutputPins;
}

Chip::Chip(ChipStructure *structure, std::vector<InputPin *> inputPins, std::vector<OutputPin *> outputPins, std::string name)
{
    this->structure = structure;
    this->inputPins = inputPins;
    this->outputPins = outputPins;
    this->name = name;
    for (int i = 0; i < outputPins.size(); i++)
    {
        outputPins[i]->chip = this;
    }
    for (int i = 0; i < inputPins.size(); i++)
    {
        inputPins[i]->chip = this;
    }
};

Chip::~Chip()
{

    // inputpins
    for (int n = 0; n < inputPins.size(); ++n)
    {
        delete inputPins[n];
    }

    for (int n = 0; n < outputPins.size(); ++n)
    {
        delete outputPins[n];
    }

    delete structure;

    // auto it_out = outputPins.begin();
    // for (int n = 0; n < outputPins.size(); ++n)
    // {
    //     outputPins.erase(it_out+n);
    // }
}
void Chip::addInputPin(InputPin *inputPin)
{
    inputPin->chip = this;
    inputPins.push_back(inputPin);
}

void Chip::addOutputPin(OutputPin *outputPin)
{
    outputPin->chip = this;
    outputPins.push_back(outputPin);
}

void Chip::setCustomChip(CustomChip *customChipT)
{
    if (type == AND || type == NOT)
    {
        std::cout << "It is not Custom Chip" << std::endl;
    }
    else
    {
        customChip = customChipT;
    }
}

void Chip::run()
{
    // check chip type
    // if AND then check all inputPins and update the outputPins
    // if NOT then check all inputPins and update the outputPins
    // if CUSTOM then little bit complex
    // if outputPins are connected to any other chip then update that chip
    // if outputPins are connected to globalOutputPins then update the globalOutputPins

    if (type == ChipType::AND)
    {
        for (int i = 0; i < outputPins.size(); i++)
        {
            outputPins[i]->state = inputPins[0]->state && inputPins[1]->state;
        }
    }
    else if (type == ChipType::NOT)
    {
        for (int i = 0; i < outputPins.size(); i++)
        {
            outputPins[i]->state = !inputPins[0]->state;
        }
    }
    else if (type == ChipType::CUSTOM)
    {
        // map all state of input pin to custom chip global input pins
        for (int i = 0; i < inputPins.size(); i++)
        {
            customChip->globalInputPins->at(i)->state = inputPins[i]->state;
        }
        customChip->run();

        // map all state of custom chip global output pins to output pins
        for (int i = 0; i < outputPins.size(); i++)
        {
            outputPins[i]->state = customChip->globalOutputPins->at(i)->state;
        }
    }

    // array of chips
    std::vector<Chip *> toRunChips;

    for (int i = 0; i < outputPins.size(); i++)
    {
        for (int j = 0; j < outputPins[i]->binds->size(); j++)
        {
            outputPins[i]->binds->at(j)->input.state = outputPins[i]->state;
            if (outputPins[i]->binds->at(j)->input.chip != nullptr)
            {
                toRunChips.push_back(outputPins[i]->binds->at(j)->input.chip);
            }
        }
    }

    for (int i = 0; i < outputPins.size(); i++)
    {
        for (int j = 0; j < outputPins[i]->bindsToGlobalOutput->size(); j++)
        {
            outputPins[i]->bindsToGlobalOutput->at(j)->output.state = outputPins[i]->state;
        }
    }

    for (int i = 0; i < toRunChips.size(); i++)
    {
        toRunChips[i]->run();
    }
}

bool Chip::isMouseInside(int x, int y)
{
    return x >= structure->boundingBox->x && x <= structure->boundingBox->x + structure->boundingBox->width && y >= structure->boundingBox->y && y <= structure->boundingBox->y + structure->boundingBox->height;
}

bool Chip::isMouseHover(int x, int y)
{
    return x >= (structure->boundingBox->x - hoverRange) && x <= (structure->boundingBox->x + structure->boundingBox->width + hoverRange) && y >= (structure->boundingBox->y - hoverRange) && y <= (structure->boundingBox->y + structure->boundingBox->height + hoverRange);
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
    // draw the chip
    // draw rectange
    // draw input and output pins
    // draw the name of the chip
    int length = name.length();
    int font_size = 20;
    Cairo::TextExtents extents;
    cr->get_text_extents(name, extents);

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
    if (length > 4)
    {
        width = 150 + (length - 4) * 7;
    }
    int inputHeight = inputPins.size() * gapper;
    int outputHeight = outputPins.size() * gapper;
    int height = inputHeight > outputHeight ? inputHeight : outputHeight;

    structure->boundingBox->width = width;
    structure->boundingBox->height = height;

    if (this->isClicked)
    {
        cr->set_source_rgb(74 / 255.0, 74 / 255.0, 74 / 255.0);
        cr->rectangle(x - hoverRange, y - hoverRange, width + 2 * hoverRange, height + 2 * hoverRange);
        cr->fill();
        cr->set_source_rgb(200 / 255.0, 39 / 255.0, 92 / 255.0);
        cr->rectangle(x, y, width, height);
        cr->fill();
    }

    else
    {
        cr->set_source_rgb(200 / 255.0, 39 / 255.0, 92 / 255.0);
        cr->rectangle(x, y, width, height);
        cr->fill();
    }

    // drawing the pins
    int eachPinSpace = (height / (inputPins.size() > outputPins.size() ? inputPins.size() : outputPins.size())) / 2;
    int inputPinEachHeight = height / inputPins.size();
    int outputPinEachHeight = height / outputPins.size();

    // hovered pins
    if (this->isHovered)
    {
        // input pins
        for (int n = 0; n < inputPins.size(); n++)
        {
            if (inputPins[n]->isHovered)
            {
                inputPins[n]->setCord(x, y + n * (inputPinEachHeight) + (inputPinEachHeight / 2));
                inputPins[n]->setRadius(eachPinSpace - 3);
                inputPins[n]->drawHovered(cr);
            }

            else
            {
                inputPins[n]->setCord(x, y + n * (inputPinEachHeight) + (inputPinEachHeight / 2));
                inputPins[n]->setRadius(eachPinSpace - 3);
                inputPins[n]->drawNormal(cr);
            }
        }

        // output pins
        for (int n = 0; n < outputPins.size(); n++)
        {
            if (outputPins[n]->isHovered)
            {
                outputPins[n]->setCord(x + width, y + n * (outputPinEachHeight) + (outputPinEachHeight / 2));
                outputPins[n]->setRadius(eachPinSpace - 3);
                outputPins[n]->drawHovered(cr);
            }
            else
            {
                outputPins[n]->setCord(x + width, y + n * (outputPinEachHeight) + (outputPinEachHeight / 2));
                outputPins[n]->setRadius(eachPinSpace - 3);
                outputPins[n]->drawNormal(cr);
            }
        }
    }

    // non_hover chips
    else
    {

        // input pins
        for (int n = 0; n < inputPins.size(); n++)
        {
            inputPins[n]->setCord(x, y + n * (inputPinEachHeight) + (inputPinEachHeight / 2));
            inputPins[n]->setRadius(eachPinSpace - 3);
            inputPins[n]->drawNormal(cr);
        }

        // output pins
        for (int n = 0; n < outputPins.size(); n++)
        {
            outputPins[n]->setCord(x + width, y + n * (outputPinEachHeight) + (outputPinEachHeight / 2));
            outputPins[n]->setRadius(eachPinSpace - 3);
            outputPins[n]->drawNormal(cr);
        }
    }

    // text for input pins
    for (int n = 0; n < inputPins.size(); n++)
    {
        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->move_to(x + 15, y + n * (inputPinEachHeight) + (inputPinEachHeight / 2) + 3);
        cr->set_font_size(14);
        cr->show_text(inputPins[n]->name);
    }

    // text for output pins
    for (int n = 0; n < outputPins.size(); n++)
    {
        cr->set_source_rgb(1.0, 1.0, 1.0);
        cr->move_to(x + width - 25, y + n * (outputPinEachHeight) + (outputPinEachHeight / 2) + 3);

        cr->set_font_size(14);
        cr->show_text(outputPins[n]->name);
    }

    // draw line between input and output pins
    for (int i = 0; i < outputPins.size(); i++)
    {
        for (int j = 0; j < outputPins[i]->binds->size(); j++)
        {
            if (outputPins[i]->binds->at(j)->isClicked)
            {
                cr->set_source_rgb(255.0 / 255.0, 0, 0);
                cr->set_line_width(8);
                draw_wire_between(outputPins[i]->binds->at(j), cr);
            }

            cr->set_line_width(4);
            if (outputPins[i]->binds->at(j)->isHovered)
            {
                cr->set_line_width(6);
            }
            else
            {
            }
            if (outputPins[i]->state == 1)
            {
                cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
            }

            else
            {
                cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
            }

            draw_wire_between(outputPins[i]->binds->at(j), cr);

            // cr->move_to(outputPins[i]->x, outputPins[i]->y);
            // cr->line_to(outputPins[i]->binds->at(j).input.x, outputPins[i]->binds->at(j).input.y);
            // cr->stroke();
        }
    }

    // draw line between output and Global output pins
    for (int i = 0; i < outputPins.size(); i++)
    {
        for (int j = 0; j < outputPins[i]->bindsToGlobalOutput->size(); j++)
        {
            if (outputPins[i]->bindsToGlobalOutput->at(j)->isClicked)
            {
                cr->set_source_rgb(255.0 / 255.0, 0, 0);
                cr->set_line_width(8);
                draw_wire_between(outputPins[i]->bindsToGlobalOutput->at(j), cr);
            }

            cr->set_line_width(4);
            if (outputPins[i]->bindsToGlobalOutput->at(j)->isHovered)
            {
                cr->set_line_width(6);
            }
            else
            {
            }

            if (outputPins[i]->state == 1)
            {
                cr->set_source_rgb(255 / 255.0, 255 / 255.0, 255 / 255.0);
            }
            else
            {
                cr->set_source_rgb(20 / 255.0, 20 / 255.0, 20 / 255.0);
            }
            // width - globalOutputPins->at(i)->radius - 10

            draw_wire_between(outputPins[i]->bindsToGlobalOutput->at(j), cr);
            // cr->move_to(outputPins[i]->x, outputPins[i]->y);
            // cr->line_to(outputPins[i]->bindsToGlobalOutput->at(j).output.x, outputPins[i]->bindsToGlobalOutput->at(j).output.y);
            // cr->stroke();
        }
    }

    // draw the name of the chip
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->set_font_size(font_size);
    Cairo::TextExtents extents2;
    cr->get_text_extents(name, extents2);
    cr->move_to(x + (width / 2) - (extents2.width / 2), y + (height / 2) + 7);
    cr->show_text(name);
    cr->fill();
}

void GlobalInputPin::bindTo(InputPin &inputPin)
{
    Bind *bind = new Bind(inputPin);
    bind->gInput = this;
    inputPin.bind = bind;
    binds->push_back(bind);
}

bool GlobalInputPin::IsToggleBtnInside(double mouseX, double mouseY)
{
    return mouseX >= boundingBox->x && mouseX <= boundingBox->x + boundingBox->width && mouseY >= boundingBox->y && mouseY <= boundingBox->y + boundingBox->height;
}

ChipSelectorMenu::ChipSelectorMenu(int width, int height, ScreenStack *scrn_stack)
{
    this->scrn_stack = scrn_stack;
    // set_css_classes({"action-menu-big-box"});
    Gtk::Fixed *ActionMenuFixed = Gtk::manage(new Gtk::Fixed());

    ActionMenuFrame = Gtk::manage(new Gtk::Frame);
    // ActionMenuFrame->set_size_request(200,200);

    Gtk::Box *ActionBox = Gtk::manage(new Gtk::Box());
    ActionBox->set_orientation(Gtk::Orientation::VERTICAL);
    ActionBox->set_css_classes({"action-menu-area"});

    Gtk::Button *quit = Gtk::manage(new Gtk::Button());
    quit->signal_clicked().connect(sigc::mem_fun(*this, ChipSelectorMenu::quit));

    quit->set_css_classes({"action-menu-btn"});
    quit->set_size_request(300, 50);
    quit->set_label("QUIT");

    Gtk::Button *library = Gtk::manage(new Gtk::Button());
    library->set_size_request(300, 50);
    library->set_css_classes({"action-menu-btn"});
    library->set_label("LIBRARY");

    Gtk::Button *save = Gtk::manage(new Gtk::Button());
    save->set_css_classes({"action-menu-btn"});
    save->set_size_request(300, 50);
    save->set_label("SAVE");
    save->signal_clicked().connect(sigc::mem_fun(*this, ChipSelectorMenu::show_save_popup));
    ActionBox->append(*quit);
    ActionBox->append(*library);
    ActionBox->append(*save);

    ActionMenuFrame->set_child(*ActionBox);
    ActionMenuFixed->put(*ActionMenuFrame, 0, height - 150 - 50 - 91);
    append(*ActionMenuFixed);

    this->m_GestureClick = Gtk::GestureClick::create();
    this->m_GestureClick->set_propagation_phase(Gtk::PropagationPhase::TARGET);
    this->m_GestureClick->signal_pressed().connect(sigc::mem_fun(*this, ChipSelectorMenu::hideMenu));
    add_controller(this->m_GestureClick);
}

void ChipSelectorMenu::hideMenu(int, int, int)
{
    hide();
    visible = false;
}

void ChipSelectorMenu::show_save_popup()
{
    scrn_stack->chipArea->save_popup->showUI();
}

void ChipSelectorMenu::showMenu()
{
    show();
    visible = true;
}

void ChipSelectorMenu::quit()
{
    scrn_stack->stack->set_transition_type(Gtk::StackTransitionType::SLIDE_RIGHT);
    scrn_stack->show_home_menu();
    hide();
    visible = false;

}
void ChipSelectorMenu::save_circuit()
{
    std::string currentDir = std::filesystem::current_path().generic_string();
    currentDir += "/" + scrn_stack->chipArea->currentDirName;
    Glib::OptionGroup::vecstrings chipFiles = getFilesWithExtension(currentDir, ".chip");
    std::string chipName = scrn_stack->chipArea->save_popup->getEntryText();
    std::string chipFilename = chipName + ".chip";
    std::cout << chipFilename << std::endl;
    // check if chipFiles contains chipName
    bool isPresent = false;
    for (int i = 0; i < chipFiles.size(); i++)
    {
        if (chipFiles[i] == chipFilename)
        {
            isPresent = true;
            break;
        }
    }
    if (isPresent)
    {
        scrn_stack->chipArea->save_popup->showError();
    }
    else if(!isValidFilename(chipName))
    {
        scrn_stack->chipArea->save_popup->showError("Invalid Chip Name");
    }
    else
    {
        scrn_stack->chipArea->save_circuit(chipName);
        scrn_stack->chipArea->save_popup->showUI();
        hide();
        visible = false;
        // scrn_stack->chipArea->load_all_chips();
        scrn_stack->chipArea->load_each_chip(chipFilename);
        scrn_stack->chipArea->save_popup->hideUI();
    }
}

// void deleteOutputPin()
// {
//     for(int i =0; i<binds->size();i++)
//     {
//         delete binds->at(i);
//     }

//     for(int i =0; i<bindsToGlobalOutput->size();i++)
//     {
//         delete bindsToGlobalOutput->at(0);
//     }
// }

// void Wire::myDelete()
// {
//     delete breakPoints;
// };

Bind::~Bind()
{
    input.state = 0;
    delete wire;
};
