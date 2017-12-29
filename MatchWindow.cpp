/* MatchWindow.cpp 
   Copyright 2017 Scott Jacovidis
   License http://opensource.org/licenses/mit-license.php MIT License
*/

#include <Application.h>
#include <Message.h>
#include <Alert.h>
#include "message_labels.h"
#include "MatchWindow.h"


MatchWindow::MatchWindow()
 : BWindow(BRect(100, 100, 701, 426), "Matching Columns", B_TITLED_WINDOW, B_NOT_H_RESIZABLE | B_NOT_V_RESIZABLE, B_CURRENT_WORKSPACE)
{
    BRect xRect(0,20,610,350);
	scoreView = new ScoreView(xRect, 25, 12);
	scoreView->Hide();
	matchView = new MatchView(xRect);
	whichView = 1;  // 1 = matchView  2 = scoreView
	
	//New Menubar for the Window
	menuBar = new BMenuBar(BRect(0, 0, 1695, 18), "MenuBar", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_ITEMS_IN_ROW, true);
	//File
	mnuFile = new BMenu("File", B_ITEMS_IN_COLUMN);
	itmFileExit = new BMenuItem("Exit", new BMessage(MNUF_EXIT), 0, 0);
	itmFileNew 	= new BMenuItem("New", new BMessage(MNUF_NEW), 'N' , B_CONTROL_KEY);
	itmFileAbout 	= new BMenuItem("About", new BMessage(MNUF_ABOUT), 0, 0);
	itmFileScores	= new BMenuItem("High Scores", new BMessage(MNUF_SCORES), 0, 0);
	
	//Options
	mnuOptions = new BMenu("Options", B_ITEMS_IN_COLUMN);
	itmOptionUndo = new BMenuItem("Undo", new BMessage(MNUO_UNDO), 'U', B_CONTROL_KEY);
	itmOptionLast = new BMenuItem("Last Game", new BMessage(MNUO_LAST), 'L', B_CONTROL_KEY);
	
				
	AddChild(matchView);
	AddChild(scoreView);
	AddChild(menuBar);
	menuBar->AddItem(mnuFile);
	menuBar->AddItem(mnuOptions);
	mnuFile->AddItem(itmFileNew);
	mnuFile->AddItem(itmFileScores);
	mnuFile->AddItem(itmFileAbout);
	mnuFile->AddItem(itmFileExit);
	mnuOptions->AddItem(itmOptionUndo);
	mnuOptions->AddItem(itmOptionLast);
	
	Show();
}

MatchWindow::~MatchWindow(void)
{
}

bool
MatchWindow::QuitRequested(void)
{
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void
MatchWindow::ScoreViewer(void)
{

} // end of ScoreViewer()

void
MatchWindow::ChangeView(int cng)
{
 if (cng == 1)  // matchView
 {
	whichView = 1;
	if (scoreView->IsHidden() == false)
		scoreView->Hide();
	if (matchView->IsHidden() == true)
		matchView->Show();
    return;
 }
 
 if (cng == 2) // scoreView 
 {
	whichView = 1;
	if (matchView->IsHidden() == false)
		matchView->Hide();
	if (scoreView->IsHidden() == true)
		scoreView->Show();
 }
} // end of ChangeView()

void
MatchWindow::MessageReceived(BMessage *message)
{
    
	switch (message->what)
	{
		case MAIN_VIEW:
		{
			ChangeView(1);  // matchView->Show
			break;
		}
		case GAME_OVER:
		{
			ChangeView(2); // scoreView->Show
			scoreView->Display(1, matchView->GetScore());
			break;
		}
		case MNUF_NEW:
		{
			if (whichView == 2) 
			{
			  if (scoreView->IsDismissed() == true)
			  {
			   ChangeView(1);
			  }
			  else
			  { break; }
			} 
			matchView->NewBoard();
			break;
		}
		case MNUF_SCORES:
		{
			ChangeView(2);
			scoreView->Display(2);
			break;
		}
		case MNUF_ABOUT:
		{
			BAlert *myAlert = new BAlert("Matching Game About Box", 
			/**/	"Matching Game v1.0  (Same Game Clone)\nBy: Scott Jacovidis  2001\nEMail: bigscott@cyberservices.com\nSee ReadMe for details!",
			/**/    "OK");
			myAlert->SetShortcut(0, B_ESCAPE);
			myAlert->Go();
			
			break;
		}
		case MNUO_LAST:
		{
			if (whichView == 2) 
			{
			  if (scoreView->IsDismissed() == true)
			  {
			   ChangeView(1);
			  }
			  else
			  { break; }
			} 
			matchView->LastGame();
			break;
		}
		case MNUF_EXIT:
		{
			be_app->PostMessage(B_QUIT_REQUESTED);
			break;
		}
		case MNUO_UNDO:
		{	if (whichView == 2) break;
			if (matchView->gameOver == true) break;
			matchView->Undo();
			break;
		}
		case HS_SAVE:
		{
			scoreView->AddToHighScore();
			scoreView->Display(2);
			break;
		}
		case HS_CANCEL:
		{
			scoreView->Display(2);
			break;
		}
		case HS_OK:
		{
			BMessage m(MAIN_VIEW);
			MessageReceived(&m);
			break;
		}
		case DEFAULT_MSG:
		{
			//Nothing here :)
			break;
		}
		
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}	
	}

}

