// MatchView.cpp
// The Gameboard View for MatchColumns.proj
// by: Scott Jacovidis  scottybe2012@gmail.com
// See ReadMe for program details.

#include <Application.h>
#include <Window.h>
#include <String.h>
#include "message_labels.h"
#include "MatchView.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


MatchView::MatchView(BRect frameRect)
 : BView(frameRect, "MatchView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE)
{

	clickStatus = false; // to determine if board pieces are hilighted
	mode = 1;  // small mode is 1  large mode is 2
    maxX = 25; maxY = 12;  //The size of the board in mode 1
    totalSelected = 0;
    points = 0;
    infoBox = new BRect(0,0,1,1);
    gameOver = false;
	
	SetViewColor((rgb_color) {240,240,240,255});
	srand(time(NULL));
	NewBoard();
		
}

MatchView::~MatchView(void)
{
	delete infoBox;
}

void
MatchView::LastGame(void)
{ // These first 2 lines of code are for safety..
 if (origPieces[1][1] == 0)
 { NewBoard(); return; }

 for (int g = 1; g <= maxX; g += 1)
 {
 	for (int h = 1; h <= maxY; h += 1)
 	{
 		pieces[g][h] = undoPieces[g][h] = origPieces[g][h];
 	}
 }
 ClearSelectList();
 points = lastPoints = 0;
 gameOver = false;
 Invalidate();
 

} // end of LastGame()

void 
MatchView::MouseUp(BPoint point)
{

}

void 
MatchView::MouseDown(BPoint point)
{
 int a, b; // To be used by TranslatePoint
 bool xOver;
  
 if (clickStatus)
 {	//  The board is already hilighted
 	// Find out if the click is in the same place and kill if necesary
    TranslatePoint(point, &a, &b);
    if (ClickedOnHilighted(a, b))
    {  // then we would want to kill the block from the screen
     int32 xPt = totalSelected * totalSelected - totalSelected;
     lastPoints = xPt;
     points += xPt;
     UpdateUndo();
     KillPieces();
     ClearSelectList();
     Invalidate();
     clickStatus = false;
     xOver = GameOver();
     if (xOver == true)
     {
     	BWindow *win;
     	win = Window();
     	win->PostMessage(GAME_OVER);
     }
     return;
    }    
    else
    {  // the user intended not to kill the block so clear the list

     ClearSelectList();
     clickStatus = false;
     Invalidate();
     return;
    }
    
 }
 else
 {  // find out if we need to hilight the board 
 	
 	TranslatePoint(point, &a, &b);
    check = 0;
    BuildList(a, b);  // this Func toggles clickStatus
    if (clickStatus)
    {
    	TotalSelected();
	   	Invalidate();
    	return;
    }
 }
 
}

bool
MatchView::CheckColumn1(int x, int y, int sColor, int way)
{
 if (sColor == 0) return(false);
 	
 bool touched;
 touched = false;
 /* NOTE::  The Algorithm you see below finds all of the blocks
 	touching the one that was clicked on.  It is rather inefficient
 	but it does the job. Michael Wulff Nielsen (Naish@worldonline.dk)
 	programmed BeSame and has a different algorithm for the problem.
 	If this algorithm ever fails you try his version (It is freeware).
    It should plug in relativly easily.. 
 */ 
 
 // note:: up = 1; dn = 3; left = 2; right = 4 
 // Search Up **Search Up **Search Up **Search Up **Search Up **Search Up **
 if ((y - 1) != 0)	
 {	
 	if (pieces[x][y -1] == sColor)
 	{
 		selectList[x][y -1] += 1;
 		if (selectList[x][y -1] > 15)
 			 return(true);
 		 		
 		if (way != 3)
  		{
 			CheckColumn1(x, y -1, sColor, 1);
 			touched = true;
 		}	
 	}
 }
 // Search Right >>Search Right >>Search Right >>Search Right >>Search Right >>
 if ((x + 1) <= maxX)	
 {	
 	if (pieces[x + 1][y] == sColor)
 	{
		selectList[x + 1][y] += 1;
		if (selectList[x + 1][y] > 15)
 			 return(true);
 		if (way != 2)
 		{
 			CheckColumn1(x + 1, y, sColor, 4);
 			touched = true;
 		}	
 	}
 }
// Search Left << Search Left << Search Left << Search Left << Search Left <<
 if ((x -1) != 0)	
 {	
 	if (pieces[x -1][y] == sColor)
 	{
		selectList[x -1][y] += 1;
		if (selectList[x -1][y] > 15)
 			 return(true);
 		if (way != 4)
 		{
 			CheckColumn1(x -1, y, sColor, 2);
 			touched = true;
 		}	
 	}
 }
// Search Down '' Search Down '' Search down '' search down ''
 if ((y + 1) <= maxY)	
 {	
 	if (pieces[x][y + 1] == sColor)
 	{
		selectList[x][y + 1] += 1;
		if (selectList[x][y +1] > 15)
 			 return(true);
 		if (way != 1)
 		{
 			CheckColumn1(x, y + 1, sColor, 3);
 			touched = true;
 		}	
 	}
 }
 
 return (touched);
 
} // end of CheckColumn1()

void
MatchView::UpdateUndo(void)
{
  
  for (int g = 1; g<= maxX; g += 1)
  {
  	for (int h = 1; h <= maxY; h += 1)
  	{
  		undoPieces[g][h] = pieces[g][h];
  	}
  }

} // end of UpdateUndo()

void
MatchView::KillPieces(void)
{
 int found;
 found = 0;
 // turn pieces[][] to 0 by column
 for (int g = 1; g <= maxX; g += 1)
 {
 	for (int h = 1; h <= maxY; h += 1)
 	{
 		if (selectList[g][h] >= 1)
 			RemoveSpace(g, h);
 	}	
 }
 // If there are no pieces in a column we move them to the left..
 // Instead of making multiple passes through the main algo..
 // we will find the last column with blocks in it and mark it as 
 // the end point.
 int ab = 0;
 for (int c = maxX; c > 1; c -= 1)
 { 
 	if (pieces[c][maxY] != 0) 
 	{	ab = c; // + 1;
 		break;
 	}
 }
 
 for (int a = 1; a <= maxX; a += 1)
 {
  if (pieces[a][maxY] == 0)
  {
   ab -= 1; // every time a column is taken out the end must be also.
   for (int g = a; g <= (maxX -1); g += 1)
   {
     for (int h = 1; h <= maxY; h += 1)
     {
     	pieces[g][h] = pieces[g + 1][h];
        pieces[g + 1][h] = 0;
     }
   }
   if (a <= ab)
   		if (pieces[a][maxY] == 0) a -= 1;	
  }
 }
 

} // end of KillPieces()

bool
MatchView::GameOver(void)
{
 int32 curColor = 0;
 gameOver = false;  
 for (int g = 1; g <= maxX; g += 1)
 {
 	for (int h = maxY; h >= 1; h -= 1)
 	{ 
 		curColor = pieces[g][h];
 		if (curColor == 0) break;
 		if ((h - 1) == 0)  break;
 		if (curColor == pieces[g][h -1]) return (false);
 		if ((g + 1) > maxX) continue;
 		if (curColor == pieces[g + 1][h]) return(false);
 	}
 }
 gameOver = true;
 return(true);
} // end of GameOver()

void 
MatchView::RemoveSpace(int x, int y)
{
 int32 prevPiece;
 for (int h = y; h >= 1; h -= 1)
 {
 	if (h == 1) 
 		prevPiece = 0;
 	else
 		prevPiece = pieces[x][h -1];
 
	 pieces[x][h] = prevPiece;
 }  
} // end of RemoveSpace()

void 
MatchView::BuildList(int x, int y)
{ //  The Algo gets all in column then left then right of the clicked.
  selectList[x][y] = 1;
  int curColor;
  curColor = pieces[x][y]; // remember to readjust if none found
  bool wasFound;
  wasFound = CheckColumn1(x, y, curColor, 0);  
  if (wasFound == true)  
  {
  clickStatus = true;
  Invalidate();  
  }
  else
  {
   selectList[x][y] = 0;
  }
 	
} // end of BuildList()

bool
MatchView::ClickedOnHilighted(int x, int y)
{
 if (selectList[x][y] >=1)
 	return(true);
 	
 return (false);  
} // end of ClickedOnHilighted()

void 
MatchView::DrawSquare(BPoint startPt, int32 way)
{
 char ch;
 rgb_color colr;
 colr.alpha = 255;
 switch (way)
 {
  case 1:
  {
    ch = 'A';
    colr.red = 240; colr.green = 0; colr.blue = 50;
    break;
  }
  case 2:
  {
    ch = 'B';
    colr.red = 0; colr.green = 50; colr.blue = 240;
  	break;
  }
  case 3:
  {
    ch = 'C';
    colr.red = 0; colr.green = 200; colr.blue = 50;
  	break;
  }
  case 4:
  {
    ch = 'D';
    colr.red = 163; colr.green = 56; colr.blue = 208;
  	break;
  }  
  case 5:
  {
    ch = 'E';
    colr.red = 240; colr.green = 138; colr.blue = 40;
    break;
  }
  
 } // select(way)
 
 BPoint x, y, z, a;
 BRect n(startPt, startPt);
 n.right += 23; n.bottom += 23;
 SetHighColor (255,255,255);
 x.x = n.left; x.y = n.top;
 y.x = n.right; y.y = n.top;
 z.x = n.left; z.y = n.bottom;
 a.x = n.right; a.y = n.bottom;
 StrokeLine(x,y); StrokeLine(x,a);
 SetHighColor(170,170,170);
 StrokeLine(y,a); StrokeLine(a, z);
   
 n.top += 1;  n.left +=1;
 n.bottom -=1; n.right -=1;
 x.x = n.left; x.y = n.top;
 y.x = n.right; y.y = n.top;
 z.x = n.left; z.y = n.bottom;
 a.x = n.right; a.y = n.bottom;
 SetHighColor(220,220,220);
 StrokeLine(x,y); StrokeLine(x,a);
 SetHighColor(190,190,190);
 StrokeLine(y,a); StrokeLine(a, z);
 
 n.top += 1; n.left +=1;
 n.bottom -=1; n.right -=1;
 SetHighColor(colr);

 FillRect(n);
 SetHighColor(255,255,255);
 x.x = n.left;
 x.y = n.top;
 x.x += 3; x.y +=11;
 MovePenTo(x);
 SetLowColor(colr);
 DrawChar(ch);

} // end of DrawSquare

void 
MatchView::DrawHilightBorder(void)
{
 if (clickStatus == false)
  	return;
 
 BRect n;
 SetPenSize(2);
 SetHighColor(30,30,30);
 for (int g = 1; g <= maxX; g +=1)
 {
 	for (int h = 1; h <= maxY; h +=1)
 	{
 		if (selectList[g][h] >= 1)
 		{
 		 printf("%d,  %d,  %d\n", g, h, g * 24);
 		 n.left = g * 24 - 24; 
 		 n.right = n.left + 22;
 		 n.top = h * 24 - 24; 
 		 n.bottom = n.top + 22;

 		 StrokeRect(n);

 		}
 	}
 }
 
SetPenSize(1);
} // end of DrawHilightBorder

void
MatchView::DrawPointsInfo(void)
{
 //Code to Draw the Box at the bottom of the Screen
 rgb_color fadeColor;
 fadeColor.red = 255; fadeColor.green = 255;
 fadeColor.blue = 255; fadeColor.alpha = 255;
 float top, right, bottom;
 top = 24 * maxY + 2;
 right = 24 * maxX;
 bottom = top + 16;
 BRect n(0, top, right, bottom);
 SetHighColor(100,100,100);
 StrokeRect(n);
 BPoint x, y;
 x.x = 1; x.y = top + 1;
 y.x = right - 1; y.y = top + 1;
 for (int g = 1; g <= 15; g++)
 {
  SetHighColor(fadeColor);
  StrokeLine(x,y);
  x.y += 1; y.y += 1;
  fadeColor.red -= g;
  fadeColor.green -= g;
  fadeColor.blue -= g;
 
 }
 
 SetHighColor(0,0,0);
 SetLowColor(235,235,235);
 MovePenTo(15,top + 11);
 BString xString;
 xString = "";
  
 if (clickStatus == false)
 {
  xString << "Current play: " << (int32)0 
  		  << "  Points: " << (int32)0 
  		  << "                       Total: " << points; 
  DrawString(xString.String());	
 }
 else
 {
  int32 totPlay;
  totPlay = (totalSelected * totalSelected) - totalSelected;
  xString << "Current play: " << totalSelected 
  		  << "  Points: " << totPlay
  		  << "                       Total: " << points;
  DrawString(xString.String());
 }
} // end of DrawPointsInfo()


void
MatchView::UpdateBoard(void)
{
 BPoint pt;
 pt.x = 1; pt.y = 1;
 for (int g = 1; g <= maxX; g += 1)
 { 
    for (int h = 1; h <=maxY; h += 1)
	{
 		if (pieces[g][h] != 0)
 		{	
 			pt.x = (24 * g) -24;
 			pt.y = (24 * h) -24;

 			DrawSquare(pt, pieces[g][h]);
 		}
 	}
 }
} // end of UpdateBoard()

void 
MatchView::NewBoard(void)
{
    
    for (int g = 1; g <= maxX; g += 1)
	{
	  for (int h = 1; h <= maxY; h += 1)
	  {
	   pieces[g][h] = 1 + rand() % 5;
	   origPieces[g][h] = undoPieces[g][h] = pieces[g][h];
	  }
	}
	points = lastPoints = 0;
	gameOver = false;
	ClearSelectList();
	Invalidate();
	
} // end of NewBoard()

void 
MatchView::Undo(void)
{
    for (int g = 1; g <= maxX; g += 1)
	{
	  for (int h = 1; h <= maxY; h += 1)
	  {
	   pieces[g][h] = undoPieces[g][h];
	  }
	}
	points -= lastPoints;
	lastPoints = 0; 
	Invalidate();
} // end of Undo()

void
MatchView::TranslatePoint(BPoint pt, int *x, int *y)
{ // Turns a mouse point into matrix coordinates
	*x = (int)pt.x / 24;
 	*y = (int)pt.y / 24;
 	*x += 1;
 	*y += 1;
 	printf("Mouse Point %d, %d\n", *x, *y);
} // end of TranslatePoint()

void
MatchView::ClearSelectList(void)
{
    for (int g = 1; g <= maxX; g += 1)
	{
	  for (int h = 1; h <= maxY; h += 1)
	  {
	   selectList[g][h] = 0;
	  }
	}
} // end of ClearSelectList()

int32
MatchView::GetScore(void)
{
  return(points);
} // end of GetScore()

void
MatchView::TotalSelected(void)
{ 
 totalSelected = 0;
 for (int g = 1; g <= maxX; g += 1)
 {
 	for (int h = 1; h <= maxY; h += 1)
 	{
 		if (selectList[g][h] >= 1)
 			++totalSelected;
 	}
 }

} // end of TotalSelected()

void
MatchView::Draw(BRect updateRect)
{
 
 UpdateBoard();
 DrawHilightBorder();
 DrawPointsInfo();
}


void
MatchView::AttachedToWindow(void)
{
 SetViewColor((rgb_color) {230,230,230,255});
}

void
MatchView::AllAttached(void)
{

}

