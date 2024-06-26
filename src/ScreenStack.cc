#include "ScreenStack.h"
#include "SplashScreen.h"
#include "MenuScreen.h"
#include "dialog.h"
#include "ChipArea.h"
#include <gtkmm.h>
#include <X11/Xlib.h> // Include the X11 header file
#include <windows.h>
#include <iostream>


ScreenStack::ScreenStack()
{
  stack = Gtk::manage(new Gtk::Stack());
  stack->set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT);
  stack->set_transition_duration(800);
  stack->set_css_classes({"screen-stack"});

  splash = SplashScreen();
  menu = new MenuScreen(this);
  chipArea = new ChipArea(this);


  stack->add(splash, "splash");
  stack->add(*menu, "menu");
  stack->add(*chipArea, "chipArea");

  stack->set_vexpand(true);
  stack->set_hexpand(true);
  overlay = Gtk::Overlay();
  overlay.set_child(*stack);


  // frame
  Dframe = new Dialog();


  // overlay.add_overlay(*Dframe);
  // Dframe->show();

  
  // fixed->put(*overlay_box,0,0);

  set_child(overlay);

}

void ScreenStack::show_splash()
{
  stack->set_visible_child(splash);
}

void ScreenStack::show_menu()
{
  stack->set_visible_child(*menu);
}

void ScreenStack::show_chip_area()
{
  stack->set_visible_child(*chipArea);
}

void ScreenStack::show_dialog()
{
  // g_print("Showing dialog\n");
  // if (Dframe == nullptr) {
  //   // Dframe = new Dialog();
  //   std::cout << "[null]" <<this->Dframe << std::endl;
  // }

  Dframe->showUI();
}