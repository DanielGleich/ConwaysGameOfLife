#ifndef CONFIG_H
#define CONFIG_H
#include <QColor>

enum CELLSTATE{DEAD, ALIVE};

const int       FIELDHEIGHT   = 40;
const int       FIELDWIDTH    = 40;
const int       FIELDSIZE     = 20;
const int       MIN_AGE       = 20;
const int       MAX_AGE       = 50;
const bool      AGING         = true;
const bool      ENDLESS_MODE  = true;
const bool      TORUS         = true;
const bool      DEFAULT_RULES = true;
const QColor    BORNCELL(0,0,0);
const QColor    DYINGCELL(249,2,31);
const QColor    DEADCELL(Qt::white);
const QColor    GRIDCOLOR(Qt::black);

#endif // CONFIG_H
