#include <gtkmm.h>
#include "SplashScreen.h"


SplashScreen::SplashScreen()
{
  Gtk::Box *box = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 1));
  box->set_css_classes({"splash"});

  Gtk::Box *labelBox = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 0));
  labelBox->set_css_classes({"splash-label"});
  box->set_valign(Gtk::Align::CENTER);
  box->set_halign(Gtk::Align::CENTER);

  Gtk::Label *label = Gtk::manage(new Gtk::Label("Digital Logic"));
  label->set_css_classes({"splash-label-head"});
  Gtk::Label *label2 = Gtk::manage(new Gtk::Label("Simulator"));
  label2->set_css_classes({"splash-label-subhead"});

  Gtk::Label *label3 = Gtk::manage(new Gtk::Label("Play and Learn"));
  label3->set_css_classes({"splash-label-desc"});

  labelBox->append(*label);
  labelBox->append(*label2);
  labelBox->append(*label3);
  labelBox->set_valign(Gtk::Align::CENTER);
  labelBox->set_halign(Gtk::Align::CENTER);

  Gtk::Image *image = Gtk::manage(new Gtk::Image("./assets/logo.png"));
  image->set_pixel_size(150);
  box->append(*labelBox);
  box->append(*image);
  set_child(*box);
}