#define private public
#define protected public
#include "player_manager.hpp"
#include <iostream>

/* MAIN IDEA
 - Read HTTP request in a separate thread
 - Produce audio buffer from HTTP request
 - Convert AUDIO
 - Remove the SIN
*/

class Prox : private BBufferProducer{
	Prox() : BBufferProducer(B_MEDIA_ENCODED_AUDIO){
		
	}
};

class StreamListener : public BUrlProtocolListener{
	public:
		void
		DataReceived(BUrlRequest* req, const char* data, off_t position, ssize_t size)
		{
			result->WriteAt(position, data, size);
		}
		BMallocIO* result;
};

/*class StreamListener : public BUrlProtocolAsynchronousListener{
	public:
		void MessageReceived(BMessage* msg){
			std::cout << msg->what << std::endl;
		}
};*/

PlayerManager::PlayerManager()
{
	if(!BMediaRoster::IsRunning()){
		BAlert* alert=new BAlert("Media server","Media server is not running",
			"OK",NULL,NULL,B_WIDTH_AS_USUAL,B_STOP_ALERT);
		alert->Go();
		be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}
	BMediaRoster* roster=BMediaRoster::Roster();
	roster->GetAudioOutput(audioOutput);
}

void
PlayerManager::PlayHTTPStream(BString urlString)
{
	BUrl url(urlString);
	StreamListener* listener=new StreamListener();
	BHttpRequest req(url,false,"HTTP",listener);
	req.Run();
	return;
}
