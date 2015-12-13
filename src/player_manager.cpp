#include "player_manager.hpp"
#include <iostream>
#include <MediaDecoder.h>
#include <SoundPlayer.h>
#include <SupportKit.h>

media_format* CreateMp3MediaFormat();
media_format CreateRawMediaFormat();

class StreamListener : public BUrlProtocolListener, public BMediaDecoder{
	public:
		off_t pos;
		size_t size;
		BMallocIO result;
		
		status_t 
		GetNextChunk(const void** chunkData, size_t* chunkLen, media_header* mh)
		{
			size=32768;
			*chunkLen=result.ReadAt(pos,&chunkData,size);
			std::cout << "GetNextChunk" << std::endl;
		}
		void
		DataReceived(BUrlRequest* req, const char* data, off_t position, ssize_t size)
		{
			result.WriteAt(position, data, size);
		}
		virtual ~StreamListener(){};
		
};

StreamListener listener;

class Player{
	public:
		BSoundPlayer* spl;
		media_format format;
		Player();
};

void
PlayFunction(void* cookie, void* buffer, long unsigned int size, const media_raw_audio_format& format)
{
	std::cout << "PlayFunction OK "<< std::endl;
	int64 frameCount;
	media_header mh;
	media_decode_info info;
	status_t status=listener.Decode(buffer,&frameCount,&mh,&info);
	if(status == B_OK) std::cout << "AllRight" << std::endl;
	if(status == B_NO_INIT) std::cout << "Not initialized" << std::endl;
	if(status != B_OK && status != B_NO_INIT) std::cout << "ERROR #" << status << std::endl;
	if(frameCount < 0){
		// STOP
	}
}

Player::Player()
{
	media_format format=CreateRawMediaFormat();
	spl=new BSoundPlayer(&format.u.raw_audio,NULL,&PlayFunction,NULL,NULL); // COOKIE IS LAST PARAMETER
	if(spl->InitCheck() == B_OK){
		std::cout << "BSoundPlayer OK" << std::endl;
	}
	spl->SetVolume(1.0f);
	spl->SetHasData(true);
	spl->Start();
}






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
	//BMediaRoster* roster=BMediaRoster::Roster();
	//status_t status=roster->GetAudioOutput(audioOutput);
}

void
PlayerManager::PlayHTTPStream(BString urlString)
{
	BUrl url(urlString);
	BHttpRequest req(url,false,"HTTP",&listener);
	req.Run();
	while(req.IsRunning()) snooze(100);
	media_format* mp3=CreateMp3MediaFormat();
	status_t status=listener.SetTo(mp3);
	switch(status){
		case B_OK: std::cout << "SetTo OK" << std::endl;break;
		case B_MEDIA_BAD_FORMAT: std::cout << "BAD MEDIA FORMAT" << std::endl;break;
		case B_BAD_VALUE: std::cout << "BAD VALUE" << std::endl;break;
		case B_ERROR: std::cout << "Unknown ERROR" << std::endl;break;
	}
	if(listener.InitCheck() != B_OK)
		std::cout << "ERROR on InitCheck" << std::endl;
	Player* player=new Player();
	return;
}


media_format*
CreateMp3MediaFormat()
{
	// copy 'n' paste from src/add-ons/media/media-add-ons/dvb/MediaFormat.cpp:
	// GetHeaderFormatMpegAudio()
	status_t status;
	media_format_description desc;
	desc.family = B_MISC_FORMAT_FAMILY;
	desc.u.misc.file_format = 'ffmp';
	desc.u.misc.codec = 0x15001; // CODEC_ID_MP3
	static media_format* sNoMp3MediaFormat = NULL;

	BMediaFormats formats;
	status = formats.InitCheck();
	if (status < B_OK) {
		printf("formats.InitCheck failed, error %lu\n", status);
		return sNoMp3MediaFormat;
	}

	media_format* mp3MediaFormat
		= static_cast<media_format*>(malloc(sizeof(media_format)));
	memset(mp3MediaFormat, 0, sizeof(media_format));
	status = formats.GetFormatFor(desc, mp3MediaFormat);
	if (status < B_OK) {
		printf("formats.GetFormatFor failed, error %lu\n", status);
		return sNoMp3MediaFormat;
	}

	return mp3MediaFormat;
}


media_format
CreateRawMediaFormat()
{
	media_format rawMediaFormat;
	memset(&rawMediaFormat, 0, sizeof(media_format));

	rawMediaFormat.type = B_MEDIA_RAW_AUDIO;
	rawMediaFormat.u.raw_audio.frame_rate = 48000;
	rawMediaFormat.u.raw_audio.channel_count = 2;
	rawMediaFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_SHORT;
	rawMediaFormat.u.raw_audio.byte_order = B_MEDIA_HOST_ENDIAN;
	rawMediaFormat.u.raw_audio.buffer_size = 32768;
		// comment from src/add-ons/media/media-add-ons/dvb/
		// DVBMediaNode.cpp::InitDefaultFormats(): when set to anything
		// different from 32768 haiku mixer has problems

	return rawMediaFormat;
}
