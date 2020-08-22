#ifndef CONFIG_H
#define CONFIG_H
#include <QColor>

enum CELLSTATE{DEAD, ALIVE};

const int   FIELDHEIGHT   = 40;
const int   FIELDWIDTH    = 40;
const int   FIELDSIZE     = 20;
const bool  TORUS         = true;
const bool  DEFAULT_RULES = true;
const bool  ENDLESSMODE = true;
const QColor ALIVECELL(Qt::black);
const QColor DEADCELL(Qt::white);
const QColor GRIDCOLOR(Qt::black);

#endif // CONFIG_H
