//-----------------------------------------------------------------------------------
// VSTGUI: Graphical User Interface Framework for VST plugins : 
//
// Version 3.5       $Date: 2008-01-09 15:48:56 +0100 (Wed, 09 Jan 2008) $
//
//-----------------------------------------------------------------------------------
// VSTGUI LICENSE
// © 2008, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A  PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//-- This is an advanced version of the first tutorial. It shows you how to inherite
//-- from CKnob and alter it's drawing by changing the handle bitmap if the mouse is
//-- over the control or if the knob is being tracked.
//-----------------------------------------------------------------------------------

#include "ChuckDelayEditor.h"

//-----------------------------------------------------------------------------------
//-- This is the custom CKnob
//-- It takes another bitmap as parameter in its constructor which is the highlited handle
//-----------------------------------------------------------------------------------
class MyKnob : public CKnob
{
public:
	MyKnob (const CRect& size, CControlListener* listener, long tag, CBitmap* background, CBitmap* handle, CBitmap* highlightHandle);
	~MyKnob ();

	CMouseEventResult onMouseDown (CPoint &where, const long& buttons);
	CMouseEventResult onMouseUp (CPoint &where, const long& buttons);
	CMouseEventResult onMouseEntered (CPoint &where, const long& buttons);
	CMouseEventResult onMouseExited (CPoint &where, const long& buttons);
protected:
	CBitmap* handleBitmap;
	CBitmap* highlightHandleBitmap;
};

//-----------------------------------------------------------------------------------
AEffGUIEditor* createEditor (AudioEffectX* effect)
{
	return new ChuckDelayEditor (effect);
}

//-----------------------------------------------------------------------------------
ChuckDelayEditor::ChuckDelayEditor (void* ptr)
: AEffGUIEditor (ptr)
{
}

//-----------------------------------------------------------------------------------
bool ChuckDelayEditor::open (void* ptr)
{
	//-- first we create the frame with a size of 300, 300 and set the background to white
	CRect frameSize (0, 0, 300, 300);
	CFrame* newFrame = new CFrame (frameSize, ptr, this);
	newFrame->setBackgroundColor (kBlackCColor);

	//-- load some bitmaps we need
	CBitmap* knobBackground = new CBitmap ("KnobBackground.png");
	CBitmap* knobHandle = new CBitmap ("KnobHandle.png");
	CBitmap* knobHandleHighlight = new CBitmap ("KnobHandleHighlight.png");

	CBitmap* sliderBackground = new CBitmap ("SliderBackground.png");
	CBitmap* sliderHandle = new CBitmap ("SliderHandle.png");
	CBitmap* sliderHandleHighlight = new CBitmap ("SliderHandleHighlight.png");

	int knobXY = knobBackground->getWidth (); // knob is a square, 
	int sliderY = sliderBackground->getHeight(); // sliderX == knobXY
	static int gap = 5;

	//-- Left Volume Fader
	CRect sliderRectLeft (0, 0, knobXY, sliderY);
	sliderRectLeft.offset (gap, 2*gap + knobXY);
	
	CSlider* leftVolumeSlider = new CSlider (
		sliderRectLeft, 
		this, 
		kLeftVolumeParameter, 
		sliderRectLeft.top, // iminpos - position of TOP of the handle's range, relative to upper left corner of the frame
		sliderRectLeft.bottom - sliderHandle->getHeight(), // imaxpos - position of BOTTOM of the handle's range relative to the upper left corner of the frame
		sliderHandle, 
		sliderBackground, 
		VSTGUI::CPoint(0, 0), 
		kVertical|kBottom);

	newFrame->addView (leftVolumeSlider);

	//-- Right Volume Fader
	CRect sliderRectRight (0, 0, knobXY, sliderY);
	sliderRectRight.offset (2*gap + knobXY, 2*gap + knobXY);

	CVerticalSlider* rightVolumeSlider = new CVerticalSlider (
		sliderRectRight, 
		this, 
		kRightVolumeParameter, 
		sliderRectRight.top, 
		sliderRectRight.bottom - sliderHandle->getHeight(), 
		sliderHandle, 
		sliderBackground); 

	newFrame->addView (rightVolumeSlider);

	//-- creating another knob,
	CRect knobRect (0, 0, knobBackground->getWidth (), knobBackground->getHeight ());
	knobRect.offset (knobBackground->getWidth () + 10, 5);
	CKnob* knob3 = new MyKnob (knobRect, this, kDelayTimeParameter, knobBackground, knobHandle, knobHandleHighlight);
	newFrame->addView (knob3);

	//-- forget the bitmaps
	knobBackground->forget ();
	knobHandle->forget ();
	knobHandleHighlight->forget();
	sliderBackground->forget ();
	sliderHandle->forget ();
	sliderHandleHighlight->forget();

	//-- remember our controls so that we can sync them with the state of the effect
	controls[kLeftVolumeParameter] = leftVolumeSlider;
	controls[kRightVolumeParameter] = rightVolumeSlider;
	controls[kDelayTimeParameter] = knob3;

	//-- set the member frame to our frame
	frame = newFrame;

	//-- sync parameters
	for (int i = 0; i < kNumParameters; i++)
		setParameter (i, effect->getParameter (i));
	return true;
}

//-----------------------------------------------------------------------------------
void ChuckDelayEditor::close ()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	delete oldFrame;
}

//-----------------------------------------------------------------------------------
void ChuckDelayEditor::valueChanged (CControl* pControl)
{
	//-- valueChanged is called whenever the user changes one of the controls in the User Interface (UI)

	//-- setParameterAutomated notifies the host that the parameter has been changed. 
	//-- this allows the host to record automation
	effect->setParameterAutomated (pControl->getTag (), pControl->getValue ());
}

//-----------------------------------------------------------------------------------
void ChuckDelayEditor::setParameter (VstInt32 index, float value)
{
	//-- setParameter is called when the host automates one of the effects parameter.

	//-- Update the UI accordingly to respond to host automation. 
	if (frame && index < kNumParameters)
	{
		// update the ui (Does not trigger callbacks)
		controls[index]->setValue (value); 
	}
}


//-----------------------------------------------------------------------------------
//-- MyKnob is just like CKnob except that it displays a different handle
//-- if the mouse is over it or if it is being tracked
//-----------------------------------------------------------------------------------
MyKnob::MyKnob (const CRect& size, CControlListener* listener, long tag, CBitmap* background, CBitmap* handle, CBitmap* highlightHandle)
: CKnob (size, listener, tag, background, handle, CPoint (0, 0))
, handleBitmap (handle)
, highlightHandleBitmap (highlightHandle)
{
	//-- we must remember both handle bitmaps, otherwise they might get destroyed in CKnob::setHandleBitmap
	handleBitmap->remember ();
	highlightHandleBitmap->remember ();
}

//-----------------------------------------------------------------------------------
MyKnob::~MyKnob ()
{
	//-- as we remembered the bitmaps, we also must forget them if we don't need them anymore
	handleBitmap->forget ();
	highlightHandleBitmap->forget ();
}

//-----------------------------------------------------------------------------------
CMouseEventResult MyKnob::onMouseDown (CPoint &where, const long& buttons)
{
	//-- we are being tracked so change the handle bitmap to the highlighted one
	setHandleBitmap (highlightHandleBitmap);

	//-- make sure we will get redrawn
	invalid ();

	//-- all other mouse handling stuff is done in the parent class
	return CKnob::onMouseDown (where, buttons);
}

//-----------------------------------------------------------------------------------
CMouseEventResult MyKnob::onMouseUp (CPoint &where, const long& buttons)
{
	//-- if the mouse is not over the knob
	if (!where.isInside (size))
	{
		//-- we change the handle to the default one
		setHandleBitmap (handleBitmap);

		//-- and again make sure we will get redrawn
		invalid ();
	}
	return CKnob::onMouseUp (where, buttons);
}

//-----------------------------------------------------------------------------------
CMouseEventResult MyKnob::onMouseEntered (CPoint &where, const long& buttons)
{
	//-- the mouse entered our knob, so we change the handle to the highlighted one
	setHandleBitmap (highlightHandleBitmap);

	//-- we must get redrawn
	invalid ();

	return CKnob::onMouseEntered (where, buttons);
}

//-----------------------------------------------------------------------------------
CMouseEventResult MyKnob::onMouseExited (CPoint &where, const long& buttons)
{
	//-- the mouse exited our knob, so change the handle to the default one
	setHandleBitmap (handleBitmap);

	//-- we must get redrawn
	invalid ();

	return CKnob::onMouseExited (where, buttons);
}

