#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <AppKit.h>
#include <InterfaceKit.h>

class Dashboard : public BWindow{
	public:
						Dashboard();
		bool 			QuitRequested();
		void 			MessageReceived(BMessage* msg);
	private:
		BTextControl*	urlForm;
			
};

#endif
