#pragma once

#include <gtkmm.h>

class Dialog : public Gtk::Box
{
public:
    Dialog();
    bool is_visible = false;
    void showUI();
    void hideUI();
    Gtk::Frame *frame;
};