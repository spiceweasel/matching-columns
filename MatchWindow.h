/* MatchWindow.h
   Copyright 2017 Scott Jacovidis
   License http://opensource.org/licenses/mit-license.php MIT License
*/

#include <Window.h>
#include <Bitmap.h>
#include <Message.h>
#include <View.h>
#include <Bitmap.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Application.h>
#include "MatchView.h"
#include "ScoreView.h"

#ifndef _MatchWindow_

class MatchWindow : public BWindow
{
public:
	/* Constructors & Destructor */
	MatchWindow ();
	~MatchWindow (void);

	void ScoreViewer(void);
	/* Hooks */

	virtual bool QuitRequested(void);
	virtual void MessageReceived(BMessage *message);
private:
    void ChangeView(int cng);
	MatchView *matchView;
	ScoreView *scoreView;
	int whichView;
	BMenuBar *menuBar;
	BMenu *mnuFile;
	BMenuItem *itmFileExit;
	BMenuItem *itmFileNew;
	BMenuItem *itmFileAbout;
	BMenuItem *itmFileScores;
	BMenu *mnuOptions;
	BMenuItem *itmOptionUndo;
	BMenuItem *itmOptionLast;
	//BMenuItem *itmOptionSize;

//	virtual void FrameMoved(BPoint origin);
//	virtual void FrameResized(float width, float height);
//	virtual void WindowActivated(bool active);
//	virtual void WorkspaceActivated(int32 workspace, bool active);
//	virtual void WorkspacesChanged(uint32 oldWorkspaces, uint32 newWorkspaces);
//	virtual void Minimize(bool minimize);
//	virtual void Zoom(BPoint origin, float width, float height);
//	virtual void Show(void);
//	virtual void Hide(void);
//	virtual void ScreenChanged(BRect frame, color_space mode);
//	virtual void MenusBeginning(void);
//	virtual void MenusEnded(void);
//	virtual void DispatchMessage(BMessage *message, BHandler *target);
//	virtual BHandler * ResolveSpecifier(BMessage *msg, int32 index,	BMessage *specifier, int32 form,const char *property);
//	virtual status_t GetSupportedSuites(BMessage *data);
//	virtual status_t Perform(perform_code d, void *arg);
	//ScoreView *scoreView;
	
};

#define _MatchWindow_
#endif











//END
