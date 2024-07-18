#pragma once

#include <gtkmm.h>

class Dialog : public Gtk::Box
{
public:
    Dialog();
    bool is_visible = false;
    void showUI();
    void hideUI();
    void showError();
    void showError(std::string message);
    void hideError();
    Gtk::Frame *frame;
    Gtk::Entry *entry = nullptr;
    Gtk::Label *error = nullptr;
    Glib::ustring getEntryText();
    Gtk::Button *saveBtn;
};