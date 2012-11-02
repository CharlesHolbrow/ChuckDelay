#include "DelayBuffer.h"

//================================================================// 
// Private:
//================================================================//

//----------------------------------------------------------------//
void DelayBuffer::clearBuffer ()
{
	// Fill the vector with zeros 
	vector<float>::iterator bb = mBuffer.begin();
	vector<float>::iterator be = mBuffer.end();
	while ( bb != be ) *bb++ = 0.0f ;
}

//================================================================// 
// Public:
//================================================================//

//----------------------------------------------------------------//
// Advance one sample, return delayed sample
float DelayBuffer::advance ( float value, float feedback )
{
	if ( abs(feedback) > 1.0f ) feedback = 0; //just in case

	// the new sample including feedback
	float newSample = value + feedback * mBuffer [ mReadPosition ];

	// what's the correct place to do this?
	if ( newSample > 1.0f ) newSample = 1.0f;
	if ( newSample < -1.0f ) newSample = -1.0f;

	// overwrite the value at write position, then increment 
	mBuffer [ mWritePosition++ ] = newSample;

	float delayedSample = mBuffer [ mReadPosition++ ];

	if ( mWritePosition >= bufferSize )
		mWritePosition = 0;

	if ( mReadPosition >= bufferSize )
		mReadPosition = 0;

	return delayedSample;
}

//----------------------------------------------------------------//
DelayBuffer::DelayBuffer ( void ) 
:mBuffer		( bufferSize ),
mReadPosition	( 0 ),
mWritePosition	( 0 )
{
	
	clearBuffer ();
	setDelay ( 0 );
}

//----------------------------------------------------------------//
DelayBuffer::~DelayBuffer(void)
{
}

//----------------------------------------------------------------//
int DelayBuffer::getBufferSize()
{
	return bufferSize;
}

//----------------------------------------------------------------//
void DelayBuffer::setDelay ( int delay )
{
	if ( delay < 0 || delay >= bufferSize )
		return;

	mDelay = delay;
	mReadPosition = ( bufferSize + mWritePosition - delay ) % bufferSize;

}