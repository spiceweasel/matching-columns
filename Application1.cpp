/* Application1.cpp
   Copyright 2017 Scott Jacovidis
   License http://opensource.org/licenses/mit-license.php MIT License
*/
#include <Application.h>
#include "message_labels.h"
#include "Application1.h"


Application1::Application1()
 : BApplication("application/x-vnd.SCMatchGame")
{

}

Application1::~Application1(void)
{
}

void
Application1::ReadyToRun(void)
{

	MatchWin = new MatchWindow();
}

bool
Application1::QuitRequested(void)
{

	// Make sure no windows are still open and close them..
	while (CountWindows()>0) WindowAt(0)->Quit();
	return true;
}

