#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include <iostream>

#include "Logger.h"
#include "Exception.h"

using namespace Tonekids;
using namespace std;

int main(int argc, char **argv) 
{
  LOGI("Hello and welcome to the OBD2 Scanner.");
  
  Fl_Window *window = new Fl_Window(320,240);
  Fl_Box *title_box = new Fl_Box(5,5,300,20,"OBD-II SCANNER");
  title_box->box(FL_NO_BOX);
  title_box->labelfont(FL_BOLD);
  title_box->labelsize(14);
  //box->labeltype(FL_SHADOW_LABEL);
  window->border(0);
  
  
  
  
  
  
  window->end();
  window->show(argc, argv);
  
  int runResult = Fl::run();
  
  LOGI("Goodbye.");
  
  return runResult;
}
