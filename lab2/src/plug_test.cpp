
#include <windows.h>
#include <iostream>
#include <config_plugin.hpp>      // Задают параметры модели
#include <solver_plugin.hpp>      // Численные решатели
#include <widget_plugin.hpp>      // Выводят результат

using namespace std;

int main() {
  HMODULE dll;

  dll = LoadLibrary(".\\out\\euler_plugin.plg");
  ifc_BasePlugin* (*reg_plg_euler)() = (ifc_BasePlugin* (*)()) GetProcAddress(dll, "register_plugin");
  ifc_BasePlugin* curr_plug_p = reg_plg();
  FreeLibrary(dll);
  ifc_SolverPlugin*



  return 0;
}
