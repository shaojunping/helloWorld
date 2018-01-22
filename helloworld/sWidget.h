#pragma once
#ifndef SWIDGETH
#define SWIDGETH

enum MouseType
{
	Unknown,
	Left,
	Right,
	Middle,
};

class SWidget
{
public:
	SWidget(const unsigned int mW, const unsigned int mH);
	virtual ~SWidget();
	virtual void OnMouseMove(double x, double y);
	virtual void OnMousePress(MouseType type);
	virtual void OnMouseRelease(MouseType type);

protected:
	const unsigned int mW;
	const unsigned int mH;
	bool mIsMousePressed[4];
};
#endif // ! SWIDGETH
