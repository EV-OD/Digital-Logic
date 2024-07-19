#include "openProjectDialog.h"
#include <filesystem>

OpenProjectDialog::OpenProjectDialog(){

    listView = Gtk::manage(new Gtk::ListView());
    set_halign(Gtk::Align::CENTER);
    set_valign(Gtk::Align::CENTER);
    set_css_classes({"frame"});


    frame = Gtk::manage(new Gtk::Frame());

    // label 
    Gtk::Label *label = Gtk::manage(new Gtk::Label());
    label->set_text("Projects");
    label->set_halign(Gtk::Align::START);
    label->set_css_classes({"dialog-label"});

    Gtk::Box *box = Gtk::manage(new Gtk::Box());
    box->set_css_classes({"dialog-box"});
    box->set_orientation(Gtk::Orientation::VERTICAL);
    box->append(*label);

    Gtk::ScrolledWindow m_ScrolledWindow;
    m_ScrolledWindow.set_child(*listView);
    m_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
    m_ScrolledWindow.set_expand();

    box->append(m_ScrolledWindow);

    m_StringList = Gtk::StringList::create({});
    selection_model = Gtk::SingleSelection::create(m_StringList);
    selection_model->set_autoselect(false);
    selection_model->set_can_unselect(true);

    listView->set_model(selection_model);
    listView->add_css_class("data-table");

    auto factory = Gtk::SignalListItemFactory::create();
    factory->signal_setup().connect(
    sigc::mem_fun(*this, &OpenProjectDialog::on_setup_label));
    factory->signal_bind().connect(
    sigc::mem_fun(*this, &OpenProjectDialog::on_bind_name));
    listView->set_factory(factory);


    Gtk::Button *cancelBtn = Gtk::manage(new Gtk::Button());
    cancelBtn->set_label("Cancel");
    cancelBtn->set_css_classes({"dialog-cancel"});
    cancelBtn->signal_clicked().connect(sigc::mem_fun(*this, &OpenProjectDialog::hideUI));

    saveBtn = Gtk::manage(new Gtk::Button());
    saveBtn->set_css_classes({"dialog-save"});
    saveBtn->set_label("Open");

    // action box
    Gtk::Box *actionBox = Gtk::manage(new Gtk::Box());
    actionBox->set_orientation(Gtk::Orientation::HORIZONTAL);
    actionBox->set_direction(Gtk::TextDirection::RTL);
    actionBox->append(*cancelBtn);
    actionBox->append(*saveBtn);

    box->append(*actionBox);

    frame->set_child(*box);

    frame->set_size_request(400, 500);
    append(*frame);

    hideUI();
}



void OpenProjectDialog::showUI(){
    std::string path = "."; // current directory
    // find all folder names in current directory except assets and CMakeFiles
    m_StringList = Gtk::StringList::create({});
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        if(entry.is_directory()){
            std::string folderName = entry.path().filename().generic_string();
            if(folderName != "assets" && folderName != "CMakeFiles"){
                // listView->append(folderName);
                m_StringList->append(folderName);
            }
        }
    }
    selection_model->set_model(m_StringList);
    is_visible = true;
    set_visible(true);
    hideError();
}

void OpenProjectDialog::on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
  list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
}

void OpenProjectDialog::on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
  auto pos = list_item->get_position();
  if (pos == GTK_INVALID_LIST_POSITION)
    return;
  auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
  if (!label)
    return;
  label->set_text(m_StringList->get_string(pos));
}

void OpenProjectDialog::showError(){
}

void OpenProjectDialog::showError(std::string message){
}

void OpenProjectDialog::hideError(){
}

Glib::ustring OpenProjectDialog::getEntryText(){
    return "";
}


void OpenProjectDialog::hideUI(){
    is_visible = false;
    set_visible(false);
}