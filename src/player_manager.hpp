#ifndef PLAYER_MANAGER_HPP
#define PLAYER_MANAGER_HPP

#include <AppKit.h>
#include <InterfaceKit.h>
#include <media/MediaRoster.h>
#include <media/BufferProducer.h>
#include <MediaKit.h>
#include <NetworkKit.h>
#include <HttpRequest.h>
#include <UrlRequest.h>
#include <UrlSynchronousRequest.h>
#include <UrlProtocolAsynchronousListener.h>

class PlayerManager{
	public:
				PlayerManager();
		void	PlayHTTPStream(BString url);
	private:
		media_node*	audioOutput;	
};

#endif
