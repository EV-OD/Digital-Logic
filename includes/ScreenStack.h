#pragma once


// Not needed but just in case 😂
#ifndef SCREENSTACK_H
#define SCREENSTACK_H

#include "SplashScreen.h"
#include <gtkmm.h>
#include "dialog.h"
#include "MenuScreen.h"
#include "ChipArea.h"

class MenuScreen;

class ScreenStack : public Gtk::Frame
{
public:
  ScreenStack();
  SplashScreen splash;
  MenuScreen *menu;
  ChipArea *chipArea;
  void show_splash();
  void show_menu();
  void show_chip_area();
  Gtk::Stack *stack;
  Dialog *Dframe;
  void show_dialog();
  Gtk::Overlay overlay;
  Gtk::Fixed *fixed;
};

#endif