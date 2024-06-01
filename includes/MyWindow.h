#pragma 


// for not needed but just in case 😂
#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <gtkmm.h>
#include "ScreenStack.h"


class MyWindow : public Gtk::Window
{
public:
  MyWindow();
  int height;
  int width;
  ScreenStack screenStack;
  bool on_timeout();
};

#endif