/////////////////////////////////////////
//
//             OpenLieroX
//
// code under LGPL, based on JasonBs work,
// enhanced by Dark Charlie and Albert Zeyer
//
//
/////////////////////////////////////////


// Input box
// Created 30/3/03
// Jason Boettcher


#include "defs.h"
#include "LieroX.h"
#include "Menu.h"
#include "GfxPrimitives.h"


///////////////////
// Draw the input box
void CInputbox::Draw(SDL_Surface *bmpDest)
{
    Menu_redrawBufferRect(iX,iY, bmpImage->w,tLX->cFont.GetHeight());

	int y=0;
	if(iMouseOver)
		y=17;
	DrawImageAdv(bmpDest,bmpImage, 0,y, iX,iY, bmpImage->w,17);
	iMouseOver = false;
    tLX->cFont.DrawCentre(bmpDest, iX+25, iY+1, tLX->clWhite, sText);
}
