/////////////////////////////////////////
//
//             OpenLieroX
//
// code under LGPL, based on JasonBs work,
// enhanced by Dark Charlie and Albert Zeyer
//
//
/////////////////////////////////////////


// Button
// Created 30/6/02
// Jason Boettcher


#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "InputEvents.h"
#include "GfxPrimitives.h"
#include "Cursor.h"


// Event types
enum {
	BTN_NONE=-1,
	BTN_MOUSEUP=0,
	BTN_MOUSEOVER
};

// Button types
enum {
	BUT_MENU,  // Used for menu buttons
	BUT_TWOSTATES, // A button with two states (up/down)
	BUT_THREESTATES // A button with three states (up/over/down)
};


class CButton : public CWidget {
public:
	// Constructor
	CButton() {
		bMouseOver = false;
		bMouseDown = false;
		iType = wid_Button;
        iGoodWidth = iWidth = 250;
		iButtonType = BUT_MENU;
		iHeight = 18;
	}

	CButton(int imgid, SmartPointer<SDL_Surface> image) {
		iImageID = imgid;
		bmpImage = image;
		bMouseOver = false;
		bMouseDown = false;
		iType = wid_Button;
        iGoodWidth = iWidth = 250;
		iButtonType = BUT_MENU;
		iHeight = image.get()->h;
	}

	CButton(const std::string& path) {
		iImageID = 0;
		bmpImage = LoadImage(path);
		bMouseOver = false;
		bMouseDown = false;
		iType = wid_Button;
        iGoodWidth = iWidth = 250;
		iButtonType = BUT_TWOSTATES;
		iHeight = bmpImage.get()->h;
	}


private:
	// Attributes

	bool		bMouseOver;
	bool		bMouseDown;
	SmartPointer<SDL_Surface> bmpImage;
	int			iImageID;
    int         iGoodWidth;
	int			iButtonType;
	CGuiSkin::CallbackHandler cClick;
	
	void initWidthHeight() {
		iWidth = iGoodWidth;
		if (iButtonType == BUT_MENU)
			iHeight = 18;
		else
			iHeight = bmpImage.get()->h;    
	}
	
public:
	// Methods

	void	Create(void);
	void	Destroy(void) { }

	//These events return an event id, otherwise they return -1
	int		MouseOver(mouse_t *tMouse)			{ bMouseOver = true; SetGameCursor(CURSOR_HAND); return BTN_MOUSEOVER; }
	int		MouseUp(mouse_t *tMouse, int nDown)		{ bMouseOver = true; bMouseDown = false; SetGameCursor(CURSOR_HAND); return BTN_MOUSEUP; }
	int		MouseDown(mouse_t *tMouse, int nDown)	{ bMouseOver = true; bMouseDown = true; SetGameCursor(CURSOR_HAND); return BTN_NONE; }
	int		MouseWheelDown(mouse_t *tMouse)		{ return BTN_NONE; }
	int		MouseWheelUp(mouse_t *tMouse)		{ return BTN_NONE; }
	int		KeyDown(UnicodeChar c, int keysym, const ModifiersState& modstate)	{ return BTN_NONE; }
	int		KeyUp(UnicodeChar c, int keysym, const ModifiersState& modstate)	{ return BTN_NONE; }

	DWORD SendMessage(int iMsg, DWORD Param1, DWORD Param2)	{ return 0; }
	DWORD SendMessage(int iMsg, const std::string& sStr, DWORD Param) { return 0; }
	DWORD SendMessage(int iMsg, std::string *sStr, DWORD Param)  { return 0; }

	// Draw the button
	void	Draw(SDL_Surface * bmpDest);
    void	Draw2(SDL_Surface * bmpDest);

	void	LoadStyle(void) {}

	int		getType()  { return iButtonType; }
	void	setType(int _t)  { iButtonType = _t; }

	static CWidget * WidgetCreator( const std::vector< CScriptableVars::ScriptVar_t > & p, CGuiLayoutBase * layout, int id, int x, int y, int dx, int dy )
	{
		CButton * w = new CButton( p[0].i, tMenu->bmpButtons );
		w->cClick.Init( p[1].s, w );
		layout->Add( w, id, x, y, dx, dy );
		return w;
	};

	static CWidget * WidgetCreator_Image( const std::vector< CScriptableVars::ScriptVar_t > & p, CGuiLayoutBase * layout, int id, int x, int y, int dx, int dy )
	{
		CButton * w = new CButton( p[0].s );
		w->cClick.Init( p[1].s, w );
		layout->Add( w, id, x, y, dx, dy );
		return w;
	};
	
	void	ProcessGuiSkinEvent(int iEvent) 
	{
		if( iEvent == BTN_MOUSEUP )
			cClick.Call();
	};

	void setImageID(int theValue) {
		iImageID = theValue;
		// TODO: send Update event here
	}
	

	int getImageID() const {
		return iImageID;
	}

	void setImage(SmartPointer<SDL_Surface> theValue) {
		bmpImage = theValue;
		initWidthHeight();
	}
	
};

#endif  //  __CBUTTON_H__
