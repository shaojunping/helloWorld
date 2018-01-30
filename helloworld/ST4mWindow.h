#pragma once
#ifndef T4MWINDOWH
#define T4MWINDOWH
#include "SWindow.h"


class ST4mWindow:public SWindow
{
public:
	ST4mWindow(const unsigned int mW, const unsigned int mH, const char *title);
	~ST4mWindow();
	
	virtual void Exec();

private:

};

#endif // !T4MWINDOWH

