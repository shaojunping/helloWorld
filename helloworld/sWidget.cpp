#include "sWidget.h"

SWidget::SWidget(const unsigned int W, const unsigned int H):mW(W), mH(H)
{
	mIsMousePressed[0] = mIsMousePressed[1] = mIsMousePressed[2] = mIsMousePressed[3] = false;
}

SWidget::~SWidget()
{

}

void SWidget::OnMouseMove(double x, double y)
{

}

void SWidget::OnMousePress(MouseType type)
{

}

void SWidget::OnMouseRelease(MouseType type)
{

}
