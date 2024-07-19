#include <gtkmm.h>
#include "MenuScreen.h"
#include "ScreenStack.h"
#include <iostream>
#include <filesystem>


void print_hello()
{
  g_print("Hello, World!\n");
}

  
MenuScreen::MenuScreen(ScreenStack *stack)
{
  this->stack = stack;
  overlay = Gtk::manage(new Gtk::Overlay());
  this->newProjectDialog = new NewProjectD();
  this->newProjectDialog->saveBtn->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::new_chip));

  this->openProjectDialog = new OpenProjectDialog();
  this->openProjectDialog->saveBtn->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::open_chip));
  // wrapper
  Gtk::Box *box = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 1));
  box->set_css_classes({"menu-area"});
  box->set_valign(Gtk::Align::CENTER);
  box->set_halign(Gtk::Align::CENTER);

  // brand box
  Gtk::Box *brandBox = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 0));
  brandBox->set_css_classes({"brand-box"});
  // brand label box
  Gtk::Box *brandLabelBox = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 0));
  brandLabelBox->set_css_classes({"brand-label-box"});
  // brand label
  Gtk::Label *brandLabel = Gtk::manage(new Gtk::Label("Digital Logic"));
  brandLabel->set_css_classes({"brand-label"});
  // brand sub label
  Gtk::Label *brandSubLabel = Gtk::manage(new Gtk::Label("Simulator"));
  brandSubLabel->set_css_classes({"brand-sub-label"});
  // brand desc label
  Gtk::Label *brandDescLabel = Gtk::manage(new Gtk::Label("Play and Learn"));
  brandDescLabel->set_css_classes({"brand-desc-label"});
  brandLabelBox->append(*brandLabel);
  brandLabelBox->append(*brandSubLabel);
  brandLabelBox->append(*brandDescLabel);

  // brand image
  Gtk::Image *brandImage = Gtk::manage(new Gtk::Image("./assets/logo.png"));
  brandImage->set_pixel_size(200);
  brandBox->append(*brandLabelBox);
  brandBox->append(*brandImage);
  brandBox->set_valign(Gtk::Align::CENTER);
  brandBox->set_halign(Gtk::Align::CENTER);

  box->append(*brandBox);
  

  // make a menu box 
  Gtk::Box *menuBox = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 0));
  menuBox->set_css_classes({"menu-box"});
  menuBox->set_size_request(400, 400);

  // buttons
  Gtk::Button *button1 = Gtk::manage(new Gtk::Button("New Project"));
  button1->set_css_classes({"menu-button"});
  std::cout << "Value of this: " << this << std::endl;
  button1->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::open_new_project_dialog));



  Gtk::Button *button2 = Gtk::manage(new Gtk::Button("Open Project"));
  button2->set_css_classes({"menu-button"});
  button2->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::open_open_project_dialog));

  Gtk::Button *button3 = Gtk::manage(new Gtk::Button("Exit"));
  button3->set_css_classes({"menu-button"});
  // button3->signal_clicked().connect(sigc::mem_fun(stack, &ScreenStack::exit_app));

  menuBox->append(*button1);
  menuBox->append(*button2);
  menuBox->append(*button3);

  menuBox->set_valign(Gtk::Align::CENTER);
  menuBox->set_halign(Gtk::Align::CENTER);

  box->append(*menuBox);
  overlay->set_child(*box);
  overlay->add_overlay(*newProjectDialog);
  overlay->add_overlay(*openProjectDialog);
  set_child(*overlay);
}


void MenuScreen::new_chip()
{
  this->stack->chipArea->clear_all();
  if(stack->chipArea->currentDirName == ""){
    newProjectDialog->error->set_visible(true);
    return;
  }
  stack->chipArea->currentDirName = newProjectDialog->entry->get_text();
  // find if folder exists
  std::string path = "."; // current directory
  std::string dirName = stack->chipArea->currentDirName;
  std::string fullPath = path + "/" + dirName;
  std::filesystem ::path folder(fullPath);
  if(std::filesystem::exists(folder)){
    newProjectDialog->showError("Project already exists");
    return;
  }
  // create new folder
  std::filesystem::create_directory(folder);
  newProjectDialog->hideUI();
  stack->show_chip_area();
  stack->chipArea->load_all_chips();
}

void MenuScreen::open_new_project_dialog(){
  newProjectDialog->showUI();
}

void MenuScreen::close_new_project_dialog(){
  newProjectDialog->hideUI();
}

void MenuScreen::open_open_project_dialog(){
  openProjectDialog->showUI();
}

void MenuScreen::close_open_project_dialog(){
  openProjectDialog->hideUI();
}

void MenuScreen::open_chip()
{
  this->stack->chipArea->clear_all();
  auto selected_pos = this->openProjectDialog->selection_model->get_selected();
  //GTK_INVALID_LIST_POSITION 
  if(selected_pos != GTK_INVALID_LIST_POSITION){
    auto selected = this->openProjectDialog->m_StringList->get_string(selected_pos);
    stack->chipArea->currentDirName = selected;
    stack->show_chip_area();
    stack->chipArea->load_all_chips();
    openProjectDialog->hideUI();
  }
}



