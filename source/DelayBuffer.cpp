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
// Advance the delay one sample, get delayed sample
float DelayBuffer::advance ( float value )
{
	mBuffer [ mWritePosition++ ] = value;
	float x = mBuffer [ mReadPosition++ ];

	if ( mWritePosition >= bufferSize )
		mWritePosition = 0;

	if ( mReadPosition >= bufferSize )
		mReadPosition = 0;

	return x;
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
void DelayBuffer::setDelay ( int delay )
{
	if ( delay < 0 || delay >= bufferSize )
		return;

	mDelay = delay;
	mReadPosition = ( bufferSize + mWritePosition - delay ) % bufferSize;


}