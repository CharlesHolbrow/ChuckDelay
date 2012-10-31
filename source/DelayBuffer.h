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
		bufferSize = 44100 
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
	void			setDelay			( int delay );

};

