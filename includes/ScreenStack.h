#pragma once


// for not needed but just in case 😂
#ifndef SCREENSTACK_H
#define SCREENSTACK_H

#include "SplashScreen.h"
#include "MenuScreen.h"
#include <gtkmm.h>

class ScreenStack : public Gtk::Frame
{
public:
  ScreenStack();
  SplashScreen splash;
  MenuScreen menu;
  void show_splash();
  void show_menu();
  Gtk::Stack *stack;

};

#endif