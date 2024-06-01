#include "ScreenStack.h"
#include "SplashScreen.h"
#include "MenuScreen.h"
#include <gtkmm.h>

ScreenStack::ScreenStack()
{
  stack = Gtk::manage(new Gtk::Stack());
  stack->set_transition_type(Gtk::StackTransitionType::CROSSFADE);
  stack->set_transition_duration(800);
  stack->set_css_classes({"screen-stack"});

  splash = SplashScreen();
  menu = MenuScreen();

  stack->add(splash, "splash");
  stack->add(menu, "menu");
  set_child(*stack);
}

void ScreenStack::show_splash()
{
  stack->set_visible_child(splash);
}

void ScreenStack::show_menu()
{
  stack->set_visible_child(menu);
}