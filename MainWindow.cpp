#include "MainWindow.h"

#include <Application.h>
#include <Button.h>
#include <String.h>
#include <StringView.h>
#include <LayoutBuilder.h>
#include <TimeFormat.h>
#include <MessageRunner.h>

#include <Bitmap.h>
#include <TranslationUtils.h>

#include <Entry.h>
#include <File.h>
#include <Path.h>

#include <View.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
	TICK = 'ctik'
};

class BackView : public BView
{

public:
	BackView();
	void Draw(BRect);

	BBitmap *BackPic;
};

BackView::BackView()
	: BView(BRect(0, 0, 200, 200), "BackView", B_FOLLOW_NONE, B_WILL_DRAW | B_TRANSPARENT_BACKGROUND)
{

	BackPic = BTranslationUtils::GetBitmapFile("JankaClockDial_300.png");
	this->ResizeTo(BackPic->Bounds().Width(), BackPic->Bounds().Height());
}

void BackView::Draw(BRect)
{

	DrawBitmap(BackPic);
}

class LongHandView : public BView
{

public:
	LongHandView();
	void Draw(BRect);

	BAffineTransform transformLHV;
	BBitmap *LongHandPic;
};

LongHandView::LongHandView()
	: BView(BRect(20, 60, 100, 100), "LHView", B_FOLLOW_NONE, B_WILL_DRAW | B_TRANSPARENT_BACKGROUND)
{

	LongHandPic = BTranslationUtils::GetBitmapFile("NagymutatóV2.png");
}

void LongHandView::Draw(BRect)
{

	DrawBitmap(LongHandPic);
}

class ShortHandView : public BView
{

public:
	ShortHandView();
	void Draw(BRect);

	BAffineTransform transformSHV;
	BBitmap *ShortHandPic;
};

ShortHandView::ShortHandView()
	: BView(BRect(20, 60, 100, 100), "SHView", B_FOLLOW_NONE, B_WILL_DRAW | B_TRANSPARENT_BACKGROUND)
{

	ShortHandPic = BTranslationUtils::GetBitmapFile("KismutatóV2.png");
}

void ShortHandView::Draw(BRect)
{

	DrawBitmap(ShortHandPic);
}

class FastHandView : public BView
{

public:
	FastHandView();
	void Draw(BRect);

	BAffineTransform transformFHV;
	BBitmap *FastHandPic;
};

FastHandView::FastHandView()
	: BView(BRect(20, 60, 100, 100), "FHView", B_FOLLOW_NONE, B_WILL_DRAW | B_TRANSPARENT_BACKGROUND)
{

	FastHandPic = BTranslationUtils::GetBitmapFile("MásodpercmutatóV2.png");
}

void FastHandView::Draw(BRect)
{

	DrawBitmap(FastHandPic);
}

BackView *bv;
LongHandView *lhv;
ShortHandView *shv;
FastHandView *fhv;
BMessageRunner *tmrTick;

MainWindow::MainWindow(void)
	: BWindow(BRect(100, 100, 500, 400), "Czlock", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE)
{
	bv = new BackView();

	bv->SetDrawingMode(B_OP_ALPHA);
	bv->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	bv->SetViewColor(B_TRANSPARENT_32_BIT);

	AddChild(bv);

	this->ResizeTo(bv->Bounds().Width(), bv->Bounds().Height());

	lhv = new LongHandView();

	lhv->SetDrawingMode(B_OP_ALPHA);
	lhv->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	lhv->SetViewColor(B_TRANSPARENT_32_BIT);

	bv->AddChild(lhv);

	lhv->ResizeTo(bv->Bounds().Width(), bv->Bounds().Height());

	shv = new ShortHandView();

	shv->SetDrawingMode(B_OP_ALPHA);
	shv->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	shv->SetViewColor(B_TRANSPARENT_32_BIT);

	bv->AddChild(shv);

	shv->ResizeTo(bv->Bounds().Width(), bv->Bounds().Height());

	fhv = new FastHandView();

	fhv->SetDrawingMode(B_OP_ALPHA);
	fhv->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	fhv->SetViewColor(B_TRANSPARENT_32_BIT);

	bv->AddChild(fhv);

	fhv->ResizeTo(bv->Bounds().Width(), bv->Bounds().Height());

	tmrTick = new BMessageRunner(this, new BMessage(TICK), 10000);
}

void MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
	case TICK:
	{
		Tick();
	}
	break;
	default:
	{
		BWindow::MessageReceived(msg);
		break;
	}
	}
}

bool MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void MainWindow::Tick()
{

	time_t curTime = time(NULL);
	struct tm *loctime = localtime(&curTime);

	int tmpHour = loctime->tm_hour;
	loctime->tm_min;
	loctime->tm_sec;

	lhv->transformLHV.Reset();
	lhv->transformLHV.RotateBy(BPoint(125, 125), 360. / 60. * loctime->tm_min * M_PI / 180.);
	lhv->SetTransform(lhv->transformLHV);
	lhv->Invalidate();

	if (tmpHour > 12)
	{
		tmpHour -= 12;
	}

	shv->transformSHV.Reset();
	shv->transformSHV.RotateBy(BPoint(125, 125), 360. / 12. * tmpHour * M_PI / 180);
	shv->SetTransform(shv->transformSHV);
	shv->Invalidate();

	fhv->transformFHV.Reset();
	fhv->transformFHV.RotateBy(BPoint(125, 125), 360. / 60. * loctime->tm_sec * M_PI / 180.);
	fhv->SetTransform(fhv->transformFHV);
	fhv->Invalidate();

	BString newTitle("Czlock ");
	newTitle << loctime->tm_hour << ":" << loctime->tm_min << ":" << loctime->tm_sec;

	this->SetTitle(newTitle.String());
}
