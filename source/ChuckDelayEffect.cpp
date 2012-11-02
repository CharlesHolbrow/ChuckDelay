//-----------------------------------------------------------------------------
// VSTGUI: Graphical User Interface Framework for VST plugins : 
//
// Version 3.5       $Date: 2008-01-09 15:48:56 +0100 (Wed, 09 Jan 2008) $
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// © 2008, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

#include "ChuckDelayEffect.h"
#include "../vstgui/aeffguieditor.h"

//------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new ChuckDelayEffect (audioMaster);
}

//------------------------------------------------------------------------------------
ChuckDelayEffect::ChuckDelayEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParameters),
buffer0(),
buffer1()
{
	setUniqueID (CCONST('G', 'U', 'I', '0'));
	setNumInputs (2);
	setNumOutputs (2);

	parameters[kLeftVolumeParameter] = 0.4f;
	parameters[kRightVolumeParameter] = 0.4f;
	parameters[kLeftDelayTimeParameter] = 0.f;
	parameters[kRightDelayTimeParameter] = 0.f;

	extern AEffGUIEditor* createEditor (AudioEffectX*);
	setEditor (createEditor (this));
}

//------------------------------------------------------------------------------------
void ChuckDelayEffect::setParameter (VstInt32 index, float value)
{
	if (index < kNumParameters)
	{
		parameters[index] = value;
		if (editor)
			// cast editor to AEFFGUIEditor
			((AEffGUIEditor*)editor)->setParameter (index, value);
	}
}

//------------------------------------------------------------------------------------
float ChuckDelayEffect::getParameter (VstInt32 index)
{
	if (index < kNumParameters)
		return parameters[index];
	return 0.f;
}

//------------------------------------------------------------------------------------
void ChuckDelayEffect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	for (VstInt32 i = 0; i < sampleFrames; i++)
	{

		float feedback0 = parameters[kLeftVolumeParameter];
		float feedback1 = parameters[kRightVolumeParameter];

		// turn off feedback when delay time is 0;
		if ( parameters[kLeftDelayTimeParameter] < 0.0001f ) feedback0 = 0;
		if ( parameters[kRightDelayTimeParameter] < 0.0001f ) feedback1 = 0;

		float delayedSample0 = buffer0.advance ( inputs[0][i], feedback0 );
		float delayedSample1 = buffer1.advance ( inputs[1][i], feedback1 );

		buffer0.setDelay( 
			(int)( (float)buffer0.getBufferSize() * (float)parameters[kLeftDelayTimeParameter] ) 
		);

		buffer1.setDelay( 
			(int)( (float)buffer1.getBufferSize() * (float)parameters[kRightDelayTimeParameter] ) 
		);

		outputs[0][i] = delayedSample0 ;
		outputs[1][i] = delayedSample1 ;
	}
}

