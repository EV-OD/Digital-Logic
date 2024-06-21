#include "dialog.h"

Dialog::Dialog(){

    set_halign(Gtk::Align::CENTER);
    set_valign(Gtk::Align::CENTER);
    set_css_classes({"frame"});

    frame = Gtk::manage(new Gtk::Frame());
    frame->set_label("Dialog");

    Gtk::Button *button = Gtk::manage(new Gtk::Button());
    button->set_label("Quit");
    button->set_css_classes({"close_btn"});
    frame->set_child(*button);
    button->signal_clicked().connect(sigc::mem_fun(*this, &Dialog::hideUI));

    frame->set_size_request(400, 200);
    append(*frame);

    hideUI();
}

void Dialog::showUI(){
    is_visible = true;
    set_visible(true);
}

void Dialog::hideUI(){
    is_visible = false;
    set_visible(false);
}