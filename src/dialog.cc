#include "dialog.h"

Dialog::Dialog(){

    set_halign(Gtk::Align::CENTER);
    set_valign(Gtk::Align::CENTER);
    set_css_classes({"frame"});

    entry = Gtk::manage(new Gtk::Entry());
    entry->set_css_classes({"dialog-entry"});

    frame = Gtk::manage(new Gtk::Frame());

    // label 
    Gtk::Label *label = Gtk::manage(new Gtk::Label());
    label->set_text("Enter chip name");
    label->set_halign(Gtk::Align::START);
    label->set_css_classes({"dialog-label"});

    error = Gtk::manage(new Gtk::Label());
    error->set_text("Chip name already exists");
    error->set_css_classes({"dialog-error"});
    error->set_visible(false);

    Gtk::Box *box = Gtk::manage(new Gtk::Box());
    box->set_css_classes({"dialog-box"});
    box->set_orientation(Gtk::Orientation::VERTICAL);
    box->append(*label);
    box->append(*entry);
    box->append(*error);

    Gtk::Button *cancelBtn = Gtk::manage(new Gtk::Button());
    cancelBtn->set_label("Cancel");
    cancelBtn->set_css_classes({"dialog-cancel"});
    cancelBtn->signal_clicked().connect(sigc::mem_fun(*this, &Dialog::hideUI));

    saveBtn = Gtk::manage(new Gtk::Button());
    saveBtn->set_css_classes({"dialog-save"});
    saveBtn->set_label("Save");

    // action box
    Gtk::Box *actionBox = Gtk::manage(new Gtk::Box());
    actionBox->set_orientation(Gtk::Orientation::HORIZONTAL);
    actionBox->set_direction(Gtk::TextDirection::RTL);
    actionBox->append(*cancelBtn);
    actionBox->append(*saveBtn);

    box->append(*actionBox);

    frame->set_child(*box);

    frame->set_size_request(400, 150);
    append(*frame);

    hideUI();
}

void Dialog::showUI(){
    is_visible = true;
    set_visible(true);
    entry->set_text("");
    hideError();
}

void Dialog::showError(){
    error->set_visible(true);
}

void Dialog::showError(std::string message){
    error->set_text(message);
    error->set_visible(true);
}

void Dialog::hideError(){
    error->set_visible(false);
}

Glib::ustring Dialog::getEntryText(){
    return entry->get_text();
}


void Dialog::hideUI(){
    is_visible = false;
    set_visible(false);
}