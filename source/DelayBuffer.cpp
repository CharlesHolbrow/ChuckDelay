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
float DelayBuffer::advance ( float value )
{
	mBuffer [ mWritePosition++ ] = value;
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
	setDelay ( 40000 );
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