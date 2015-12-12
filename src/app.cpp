#include <AppKit.h>

#include "dashboard.hpp"

class Voxophone : public BApplication{
	public:
		Voxophone() : BApplication("application/x-voxophone")
		{
			Dashboard* dash=new Dashboard();
			dash->Show();
		}
};

int main(int argc, char** argv)
{
	Voxophone app;
	app.Run();
	return 0;
}
