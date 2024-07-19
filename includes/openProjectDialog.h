#pragma once

#include <gtkmm.h>

class OpenProjectDialog : public Gtk::Box
{
public:
    OpenProjectDialog();
    bool is_visible = false;
    void showUI();
    void hideUI();
    void showError();
    void showError(std::string message);
    void hideError();
    Glib::RefPtr<Gtk::SingleSelection> selection_model;
    Gtk::Frame *frame;
    Glib::ustring getEntryText();
    Gtk::Button *saveBtn;
    Gtk::ListView *listView = nullptr;
    Glib::RefPtr<Gtk::StringList> m_StringList;
    void on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item);
    void on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item);
};