// Application1.h
// BApplication Object header for Matching Columns Game

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
