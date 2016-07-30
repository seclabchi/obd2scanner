#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Button.H>

#include <iostream>
#include <vector>

#include "Logger.h"
#include "Exception.h"

#include "CommInterface.h"

using namespace Tonekids;
using namespace std;

Fl_Window *window = 0;

static int runCommandLineMode();

void exitButtonPressed(Fl_Widget* w, void* v)
{
    if(NULL != window)
    {
        delete window;
    }
    else
    {
        LOGE("Tried to close main window but the pointer is NULL.");
    }
}

int main(int argc, char **argv) 
{
	int no_gui_flag = 0;
	int option_index = 0;
	int c = 0;
	int run_result = 0;
	
	static struct option long_options[] =
	{
		//these options set a flag
		{"no-gui", no_argument, &no_gui_flag, true},
		{0, 0, 0, 0}
	};
	
	while ((c = getopt_long (argc, argv, "", long_options, &option_index)) != -1)
	{
		switch (c)
		{
			case 0:
			  /* If this option set a flag, do nothing else now. */
			  if (long_options[option_index].flag != 0)
				break;
			  printf ("option %s", long_options[option_index].name);
			  if (optarg)
				printf (" with arg %s", optarg);
			  printf ("\n");
			  break;
			default:
				abort();
		}
	}
	
	if(no_gui_flag)
	{
		LOGI("Running in command line mode.");
	}
	else
	{
		LOGI("Running in GUI mode.");
	}
	
	
  LOGI("Hello and welcome to the OBD2 Scanner.");
  
  if(no_gui_flag)
  {
	  run_result = runCommandLineMode();
  }
  else
  {
	  window = new Fl_Window(320,240);
	  Fl_Box *title_box = new Fl_Box(5,5,300,20,"OBD-II SCANNER");
	  title_box->box(FL_NO_BOX);
	  title_box->labelfont(FL_BOLD);
	  title_box->labelsize(14);
	  
	  window->fullscreen();
	  
	  int X, Y, W, H;
	  
	  int& Xref = X;
	  int& Yref = Y;
	  int& Wref = W;
	  int& Href = H;
	  
	  Fl::screen_work_area(Xref, Yref, Wref, Href);
	  
	  LOGD("Screen work area: %d, %d, %d, %d", X, Y, W, H);
	  
	  Fl::screen_xywh(Xref, Yref, Wref, Href);
	  
	  LOGD("Screen xywh: %d, %d, %d, %d", X, Y, W, H);
	  
	  window->border(0);
	  
	  Fl_Text_Display* serialLogWin = new Fl_Text_Display(5, 25, 310, 180);
	  Fl_Text_Buffer* serialLogWinBuf = new Fl_Text_Buffer(8192);
	  serialLogWin->buffer(serialLogWinBuf);
	  
	  Fl_Button* exitButton = new Fl_Button(5, 210, 50, 20, "EXIT");
	  exitButton->callback(exitButtonPressed);

	  window->end();
	  window->show(argc, argv);
	  
	  CommInterface* ci = 0;
	  
	  try
	  {
		  ci = new CommInterface(string("/dev/ttyUSB0"), 19200);
		  vector<string> resetResponse = ci->reset();
		  vector<string>::iterator it;
		  
		  for(it = resetResponse.begin(); it != resetResponse.end(); it++)
		  {
			  cout << *it << endl;
		  }
	  }
	  catch(Exception e)
	  {
		  LOG_EXCEPTION(e);
	  }
	  
	  delete ci;
	  
	  run_result = Fl::run();
  
  }
  
  LOGI("Goodbye.");
  
  return run_result;
}

int runCommandLineMode()
{
	return 0;
}
