#pragma once

#include <vector>
using namespace std;

//================================================================// 
// DelayBuffer 
//================================================================//
class DelayBuffer
{

private:

	enum { 
		bufferSize = 96000 
	};

	vector<float>	mBuffer;
	int				mDelay;	// in samples
	int				mReadPosition;
	int				mWritePosition;

	//----------------------------------------------------------------//
	void			clearBuffer			();

public:
	 //----------------------------------------------------------------//
	float			advance				( float value );
					DelayBuffer			( void );
	virtual			~DelayBuffer		( void );
	int				getBufferSize		();
	void			setDelay			( int delay );

};

