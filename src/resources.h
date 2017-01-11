#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <qpixmap.h>

class Resources {

public:
	Resources();
	~Resources();
	QPixmap openIcon, saveIcon, printIcon, flipIcon, actualsizeIcon,
		topologyIcon, vertfitIcon, horifitIcon, logwinIcon, tipIcon;  
};

#endif
