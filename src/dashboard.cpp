#include "dashboard.hpp"
#include "player_manager.hpp"

#include <InterfaceKit.h>
#include <GroupLayout.h>
#include <Layout.h>
#include <AppKit.h>

const uint32 PLAY_HTTP=1;

PlayerManager* manager;

Dashboard::Dashboard() 
	: BWindow(BRect(50,50,250,450),"Voxophone - Online radio player",
		B_TITLED_WINDOW,0)
{
	manager=new PlayerManager();
	BGroupLayout* main_lay=new BGroupLayout(B_VERTICAL);
	SetLayout(main_lay);
	BGroupLayout* control=new BGroupLayout(B_VERTICAL);
	BView* main=new BView(NULL,B_WILL_DRAW,control);
	main->SetViewColor(220,220,220);
	main_lay->AddView(main);
	
	/* HTTP Stream */
	urlForm=new BTextControl(NULL,"HTTP stream","http://necta-relay.mnus.de:8080/necta192.mp3",NULL);
	control->AddView(urlForm);
	BButton* urlSubmit=new BButton(NULL,"Play HTTP stream",
		new BMessage(PLAY_HTTP));
	control->AddView(urlSubmit);
}

bool
Dashboard::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

void
Dashboard::MessageReceived(BMessage* msg)
{
	switch(msg->what){
		case PLAY_HTTP: {
			manager->PlayHTTPStream(urlForm->Text());
			break;
		}
		default:
			BWindow::MessageReceived(msg);
	}
}
