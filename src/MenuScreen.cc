#include <gtkmm.h>
#include "MenuScreen.h"
#include "ScreenStack.h"
#include <iostream>


void print_hello()
{
  g_print("Hello, World!\n");
}

  
MenuScreen::MenuScreen(ScreenStack *stack)
{
  this->stack = stack;
  std::cout << "[initial stack]" << this->stack << std::endl;
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
  button1->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::new_chip));



  Gtk::Button *button2 = Gtk::manage(new Gtk::Button("Open Project"));
  button2->set_css_classes({"menu-button"});
  // button2->signal_clicked().connect(sigc::mem_fun(*this, &MenuScreen::open_chip));

  Gtk::Button *button3 = Gtk::manage(new Gtk::Button("Exit"));
  button3->set_css_classes({"menu-button"});
  // button3->signal_clicked().connect(sigc::mem_fun(stack, &ScreenStack::exit_app));

  menuBox->append(*button1);
  menuBox->append(*button2);
  menuBox->append(*button3);

  menuBox->set_valign(Gtk::Align::CENTER);
  menuBox->set_halign(Gtk::Align::CENTER);

  box->append(*menuBox);
  set_child(*box);
}

void MenuScreen::new_chip()
{
  stack->show_chip_area();

}

void MenuScreen::open_chip()
{
  stack->show_dialog();
}



