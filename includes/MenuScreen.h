#pragma once

// Not needed but just in case 😂
#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <gtkmm.h>
#include "NewProjectDialog.h"
#include "openProjectDialog.h"
class ScreenStack;


class MenuScreen : public Gtk::Frame
{
public:
  MenuScreen(ScreenStack *stack);
  ScreenStack *stack;
  NewProjectD *newProjectDialog = nullptr;
  OpenProjectDialog *openProjectDialog = nullptr;

  Gtk::Overlay *overlay = nullptr;
  void new_chip();
  void open_chip();
  void open_new_project_dialog();
  void close_new_project_dialog();

  void open_open_project_dialog();
  void close_open_project_dialog();
};
#endif