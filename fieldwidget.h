#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "game.h"

class FieldWidget : public QWidget
{
  protected:
   Game *game;
   bool drawMode = false;
   CELLSTATE drawModeState = ALIVE;

   void setDrawMode(bool state);
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void setField(QMouseEvent *event);
   void setDrawModeState(QMouseEvent *event);
  public:
    FieldWidget(Game *game, QWidget *parent = 0);
    void paintEvent(QPaintEvent* e);
};

#endif // FIELDWIDGET_H
