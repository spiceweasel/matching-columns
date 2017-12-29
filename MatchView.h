/* MatchView.h
   Copyright 2017 Scott Jacovidis
   License http://opensource.org/licenses/mit-license.php MIT License
*/

#include <View.h>
#include <Bitmap.h>
#include <Bitmap.h>
#include <Message.h>


#ifndef _MatchView_

class MatchView : public BView
{
public:
	/* Constructors & Destructor*/
	MatchView (BRect frameRect);
	~MatchView (void);
	
	/* Hooks */	
	virtual void AttachedToWindow(void);
	virtual void AllAttached(void);
	virtual void MouseUp(BPoint point);
	virtual void MouseDown(BPoint point);
	virtual void Draw(BRect updateRect);
	/* Public Operations */
	void LastGame(void);
    void NewBoard(void);
    void Undo(void);
	int32 GetScore(void);
	bool gameOver;
	
private:
	/* Private Operations */
	void DrawSquare(BPoint startPt, int32 way);
    void DrawHilightBorder(void);
    void DrawPointsInfo();
    void UpdateBoard(void);
    void BuildList(int x, int y);  // of clicked items
    void ClearSelectList(void);
    void TranslatePoint(BPoint pt, int *x, int *y); // point to square
    bool ClickedOnHilighted(int x, int y);
    bool CheckColumn1(int x, int y, int sColor, int way);
    void KillPieces(void);
    void RemoveSpace(int x, int y);
    void TotalSelected(void);
    bool GameOver(void);
    void UpdateUndo(void);
    
    
	BRect* infoBox;
    bool clickStatus;
    int32 totalSelected;
    int32 mode;
    int32 pieces [26][13];
    int32 undoPieces [26][13];
    int32 selectList [26][13];
    int32 origPieces [26][13];
    int maxX, maxY;
    int32 points;
    int32 lastPoints;  // Used to decrement score if Undo
    int check;
    bool _bxl_attached;

};

#define _MatchView_
#endif
