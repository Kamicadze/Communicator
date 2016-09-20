#include "IFrame.h"

class CFrame: public IFrame
{
	public:
		CFrame();
		~CFrame();
		void parseFrame(Frame*);
		void createFrame();

};

