#pragma once

// Not needed but just in case 😂
#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <gtkmm.h>
class ScreenStack;


class MenuScreen : public Gtk::Frame
{
public:
  MenuScreen(ScreenStack *stack);
  ScreenStack *stack;
  void new_chip();
  void open_chip();
};
#endif