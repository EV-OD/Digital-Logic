#include <gtkmm.h>
#include "MenuScreen.h"

MenuScreen::MenuScreen()
{
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
  Gtk::Button *button2 = Gtk::manage(new Gtk::Button("Open Project"));
  button2->set_css_classes({"menu-button"});
  Gtk::Button *button3 = Gtk::manage(new Gtk::Button("Exit"));
  button3->set_css_classes({"menu-button"});
  menuBox->append(*button1);
  menuBox->append(*button2);
  menuBox->append(*button3);

  menuBox->set_valign(Gtk::Align::CENTER);
  menuBox->set_halign(Gtk::Align::CENTER);

  box->append(*menuBox);
  set_child(*box);
}
