// ScoreView.cpp
// The High Score View for MatchColumns.proj
// by: Scott Jacovidis  scottybe2012@gmail.com
// See ReadMe for program details.

#include <File.h>
#include <Path.h>
#include <Application.h>
#include <Roster.h>
#include <Message.h>
#include <Button.h>
#include <TextControl.h>
#include <String.h>
#include <List.h>
#include <Rect.h>
#include "ScoreView.h"
#include "message_labels.h"
#include <alloc.h>
#include <stdio.h>
#include <string.h>

ScoreView::ScoreView(BRect size, int totX, int totY)
 : BView(size, "ScoreView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE)
 {
   totalX = totX;
   totalY = totY;
   isDismissed = true;
   BRect rect(210, 265, 260, 285);
   btnSave = new BButton(rect, "HSSave", "Save", new BMessage(HS_SAVE), B_FOLLOW_LEFT, B_WILL_DRAW);
   rect.OffsetBy(80, 0);
   btnCancel = new BButton(rect, "HSCancel", "Cancel", new BMessage(HS_CANCEL), B_FOLLOW_LEFT, B_WILL_DRAW);
   rect.top = 190; rect.bottom = 220;
   rect.left = 260; rect.right = 320;
   btnOk = new BButton(rect, "HSOk", "Ok", new BMessage(HS_OK), B_FOLLOW_LEFT, B_WILL_DRAW);
   rect.left = 20;
   rect.top = 270;
   rect.right = 200;
   rect.bottom = 295;
   txcNameEntry = new BTextControl(rect, "HSTxtControl", "Enter Your Name", "Anonymous", new BMessage(DEFAULT_MSG), B_FOLLOW_LEFT, B_WILL_DRAW);
   AddChild(btnSave);
   AddChild(btnCancel);
   AddChild(btnOk);
   AddChild(txcNameEntry);
   txcNameEntry->SetDivider(80);
   btnSave->Hide();
   btnCancel->Hide();
   btnOk->Hide();
   txcNameEntry->Hide();
      
   // Find out the app directory so we can save and read high scores  
   status_t erra;
   app_info myInfo;
   erra = be_app->GetAppInfo(&myInfo); 
   //if (erra != B_OK)
   BEntry entry(&myInfo.ref);
   xPath = new BPath();
   entry.GetPath(xPath);
   xPath->GetParent(xPath);
   xPath->Append("High Scores.MCG");
   // Use The below Line to Load and save files  
   xFile = new BFile(xPath->Path(), B_READ_WRITE | B_CREATE_FILE);
   off_t xSize;
   xFile->GetSize(&xSize);
   delete xFile;
   if (xSize == 0) 
   		NewHSList();
   else
   		LoadScores();  
    	
 
 } // end of ScoreView() Constructor
 
 ScoreView::~ScoreView(void)
 {
   
      
 } // end of ScoreView() Destructor
 void
 ScoreView::NewHSList(void)
 {
  for (int g = 0; g <= 9; g++)
  {
    strcpy(Scores[g].name, "SameMaster");
    Scores[g].score = 100;
    Scores[g].boardX = 25;
    Scores[g].boardY = 12;
  }
  SaveScores();
 
 } // end of NewHSList()
 
 void
 ScoreView::LoadScores(void)
 {
   /*  I do not attempt to read the boardX and boardY that is saved to the
   file since I do not need them..  They are saved for future compatability
   that you might add..  */
   
   BMessage m;
   xFile = new BFile(xPath->Path(), B_READ_ONLY);  
   if (xFile->InitCheck() == B_OK)
   {
   		m.Unflatten(xFile);
   		const char *s;
   		s = m.FindString("N0");
   		m.FindInt32("S0", &Scores[0].score);
   		strcpy(Scores[0].name, s);
   		s = m.FindString("N1");
   		m.FindInt32("S1", &Scores[1].score);
   		strcpy(Scores[1].name, s);
   		s = m.FindString("N2");
   		m.FindInt32("S2", &Scores[2].score);
   		strcpy(Scores[2].name, s);
   		s = m.FindString("N3");
   		m.FindInt32("S3", &Scores[3].score);
   		strcpy(Scores[3].name, s);
   		s = m.FindString("N4");
   		m.FindInt32("S4", &Scores[4].score);
   		strcpy(Scores[4].name, s);
   		s = m.FindString("N5");
   		m.FindInt32("S5", &Scores[5].score);
   		strcpy(Scores[5].name, s);
   		s = m.FindString("N6");
   		m.FindInt32("S6", &Scores[6].score);
   		strcpy(Scores[6].name, s);
   		s = m.FindString("N7");
   		m.FindInt32("S7", &Scores[7].score);
   		strcpy(Scores[7].name, s);
   		s = m.FindString("N8");
   		m.FindInt32("S8", &Scores[8].score);
   		strcpy(Scores[8].name, s);
   		s = m.FindString("N9");
   		m.FindInt32("S9", &Scores[9].score);
   		strcpy(Scores[9].name, s);
   		
   		delete xFile;
   
   }
 
 } // end of LoadScores
 
 void 
 ScoreView::SaveScores(void)
 {
   BMessage m;
   m.AddInt32("BoardX", Scores[0].boardX);
   m.AddInt32("BoardY", Scores[0].boardY);
   
   m.AddString("N0", Scores[0].name);
   m.AddInt32("S0", Scores[0].score);
   m.AddString("N1", Scores[1].name);
   m.AddInt32("S1", Scores[1].score);
   m.AddString("N2", Scores[2].name);
   m.AddInt32("S2", Scores[2].score);
   m.AddString("N3", Scores[3].name);
   m.AddInt32("S3", Scores[3].score);
   m.AddString("N4", Scores[4].name);
   m.AddInt32("S4", Scores[4].score);
   m.AddString("N5", Scores[5].name);
   m.AddInt32("S5", Scores[5].score);
   m.AddString("N6", Scores[6].name);
   m.AddInt32("S6", Scores[6].score);
   m.AddString("N7", Scores[7].name);
   m.AddInt32("S7", Scores[7].score);
   m.AddString("N8", Scores[8].name);
   m.AddInt32("S8", Scores[8].score);
   m.AddString("N9", Scores[9].name);
   m.AddInt32("S9", Scores[9].score);
   
   xFile = new BFile(xPath->Path(), B_WRITE_ONLY | B_CREATE_FILE);
   m.Flatten(xFile);
   delete xFile;
 
 } // end of SaveScores
 bool 
 ScoreView::IsDismissed(void)
 {
  return(isDismissed);  
 } // end of IsDismissed()
 
 void 
 ScoreView::Display(int x, int32 xSc = -1)
 {
  if (xSc != -1) curScore = xSc;
     
  if (x == 1)   // Show Points Comparason and buttons
  {
   if (IsAHighScore(xSc))
   {
   	isDismissed = false;
   	if (btnOk->IsHidden() == false)
   	 	btnOk->Hide();
   	txcNameEntry->Show();
   	btnSave->Show();
   	btnCancel->Show();
    txcNameEntry->SetText("Anonymous");	
    txcNameEntry->MakeFocus(true);
    Invalidate();
    return;
   }
   else x = 2;
  }
  
  if (x == 2)  // Show high scores only!
  {
   isDismissed = true;
   btnOk->Show();
   if (txcNameEntry->IsHidden() == false)
   	txcNameEntry->Hide();
   if (btnSave->IsHidden() == false)
    btnSave->Hide();
   if (btnCancel->IsHidden() == false)
    btnCancel->Hide();
   Invalidate();
  }
 
 } // end of Display()
 
 bool
 ScoreView::IsAHighScore(int32 x)
 {
 	for (int g = 9; g >= 0; g--)
    {	if (x >= Scores[g].score)
   	   		return(true); 
    }
    return(false);
 } // end of IsAHighScore()
 
 void
 ScoreView::AddToHighScore(void)
 {
  const char *t;
  int insPos;
  t = txcNameEntry->Text();
  for (int g = 9; g >=0; g--)
  {
  	printf("%d", g);
  	if ((Scores[g].score >= curScore) || (g == 0))
  	{
  		if ((g == 0) && (Scores[g].score <= curScore))
  			insPos = 0;
  		else
  			insPos = g + 1;
  			
  		for (int x = 8; x >= insPos; x--)
  		{
  			Scores[x+1].name = Scores[x].name;
  			Scores[x+1].score = Scores[x].score;	  			
  		}
  	    size_t xLb, xLen;
  	    xLb = strlen(t);
  	    if (xLb >= 15)
  	    {
  	     xLen = 14;
  	     strncpy(Scores[insPos].name, t, xLen);
  	     Scores[insPos].name[15] = '\0';
  	    }
  	    else
  	    {
  	     xLen = 15;
  	     strncpy(Scores[insPos].name, t, xLen);
  	     
  	    }
  	    Scores[insPos].score = curScore;
  	    Scores[insPos].boardX = 25;
  	    Scores[insPos].boardY = 12;
  	    break;
  	}
  } 
  SaveScores();
  
 } // end of AddToHighScore()
 
 void
 ScoreView::Draw(BRect updateRect)
 {
   BPoint x;
   
   PrintHighScores();
   
   if (isDismissed == false)
   {
    x.x = 90; 
    x.y = 210;
    MovePenTo(x);       
    SetHighColor(0,0,0);
    SetFontSize(16);
    BString xSt;
    xSt = "";
    xSt << curScore;
    DrawString("You Have A High Score !!           ");
    DrawString(xSt.String());
    
   }
   else
   {
   
   }
    
 } // end of Draw()
void
ScoreView::PrintHighScores(void)
{
 BPoint xpt1;
 BString xString;
 //  Draw the border around the High Scores
 BRect xrect = Bounds();
 xrect.top += 40; xrect.left += 10;
 xrect.bottom -= 150; xrect.right -= 30;
 int col = 110;
 SetHighColor(col,col,col);
 StrokeRect(xrect);
 for (int g = 1; g <= 6; g++)
 {
  SetHighColor(col + g*7, col + g*10, col + g*10);
  xrect.top +=1; xrect.left +=1;
  xrect.bottom -=1; xrect.right -=1;
  StrokeRect(xrect);  
 } 
 // Print The First Column of Names and scores
 xpt1.x = 40; xpt1.y = 70;
 SetHighColor(0,0,0);
 SetFontSize(16);
 for  (int32 g = 0; g <= 9; g++)
 {
  if (g == 5) { xpt1.x = 320; xpt1.y = 70;}
  MovePenTo(xpt1);
  xString = "";
  xString << g +1 << ".  ";
  DrawString(xString.String());
  DrawString(Scores[g].name);
  xpt1.x += 150;
  MovePenTo(xpt1);
  xString = "";
  xString << Scores[g].score;
  DrawString(xString.String());
  xpt1.x -=150;
  xpt1.y +=22;
 }
 SetFont(be_bold_font);
 SetFontSize(30);
 xpt1.x = 210; xpt1.y = 30;
 MovePenTo(xpt1);
 SetHighColor(100,100,250);
 DrawString("High Scores");
 SetFont(be_plain_font);

} // end of PrintHighScores