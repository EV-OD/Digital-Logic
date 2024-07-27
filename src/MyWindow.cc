
#include "MyWindow.h"
#include <X11/Xlib.h> // Include the X11 header file
#include "ScreenStack.h"
#include <windows.h>


MyWindow::MyWindow()
{
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);
  set_title("Basic application");
  set_default_size(width, height);

  screenStack = new ScreenStack();
  screenStack->show_splash();
  screenStack->set_vexpand(true);
  screenStack->set_hexpand(true);

  set_child(*screenStack);

  auto css = Gtk::CssProvider::create();
  css->load_from_path("style.css");
  Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_USER);
  // after some delay show the menu
  screenStack->stack->set_transition_type(Gtk::StackTransitionType::CROSSFADE);
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &MyWindow::on_timeout), 1000);
  set_icon_name("./assets/logo.png");

  auto icon_theme = Gtk::IconTheme::create();
  // icon path is ./assets/logo.png
  // icon_theme->add_resource_path("/assets");
  icon_theme->add_search_path("./assets");
  if(icon_theme->has_icon("logo")){
    std::cout << "Icon found" << std::endl;
    set_default_icon_name("logo");
    set_icon_name("logo");
  }else{
    std::cout << "Icon not found" << std::endl;
  }

}

bool MyWindow::on_timeout()
{
  screenStack->show_home_menu();
  return false;
}
