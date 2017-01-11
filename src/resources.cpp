#include "resources.h"

#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"
#include "topology.xpm"
#include "vertfit.xpm"
#include "horifit.xpm"
#include "logwin.xpm"
#include "flip.xpm"
#include "actualsize.xpm"
#include "tip.xpm"

Resources::Resources() {
	saveIcon = QPixmap(filesave);
	openIcon = QPixmap(fileopen);
	printIcon = QPixmap(fileprint);
	topologyIcon = QPixmap(topology);
	vertfitIcon = QPixmap(vertfit);
	actualsizeIcon = QPixmap(actualsize);
	horifitIcon = QPixmap(horifit);
	logwinIcon = QPixmap(logwin);
	flipIcon = QPixmap(flip);
	tipIcon = QPixmap(tip);
}
