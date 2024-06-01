#include <gtkmm.h>
#include <X11/Xlib.h>
#include <windows.h>

#include "MyWindow.h"

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  return app->make_window_and_run<MyWindow>(argc, argv);
}