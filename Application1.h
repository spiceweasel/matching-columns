/* Application1.h
   Copyright 2017 Scott Jacovidis
   License http://opensource.org/licenses/mit-license.php MIT License
*/
#include <Application.h>
#include <Window.h>
#include "MatchWindow.h"

#ifndef _Application1_

class Application1 : public BApplication
{
public:
	/* Constructors & Destructor*/
	Application1 ();
	~Application1 (void);

	/* Hooks */
	virtual void ReadyToRun(void);
	virtual bool QuitRequested(void);
	MatchWindow * MatchWin;
	
};

#define _Application1_
#endif
