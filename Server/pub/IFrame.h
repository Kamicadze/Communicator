#include "SFrame.h"

class IFrame
{
	public:
		virtual ~IFrame(){}
		virtual void parseFrame(Frame*)=0;
		virtual void createFrame()=0;
};
