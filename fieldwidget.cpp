#include "fieldwidget.h"

void FieldWidget::setDrawMode(bool state)
{
  drawMode = state;
}

void FieldWidget::mousePressEvent(QMouseEvent* event)
{
  setDrawModeState(event);
  setDrawMode(true);
  setField(event);
}

void FieldWidget::mouseMoveEvent(QMouseEvent* event)
{
  setField(event);
}

void FieldWidget::mouseReleaseEvent(QMouseEvent* event)
{
  setDrawMode(false);
  setField(event);
}

void FieldWidget::setField(QMouseEvent *event)
{
  if (drawMode)
    {
      int x = event->x()/FIELDSIZE;
      int y = event->y()/FIELDSIZE;
      if (x < game->getFieldWidth() && y < game->getFieldHeight())
        game->setField(x,y,drawModeState);
    }
  update();
}

void FieldWidget::setDrawModeState(QMouseEvent *event)
{
  int x = event->x()/FIELDSIZE;
  int y = event->y()/FIELDSIZE;
  drawModeState = game->getField(x,y) == ALIVE ? DEAD : ALIVE;
}

FieldWidget::FieldWidget(Game *game,QWidget* parent)
  :QWidget(parent)
{
  this->game = game;
  setFixedSize(game->getFieldWidth()*FIELDSIZE+1,game->getFieldHeight()*FIELDSIZE+1);
  setStyleSheet("background: white");
  update();
}

void FieldWidget::paintEvent(QPaintEvent*)//e)
{
  QPainter paint(this);
  QPen pen(GRIDCOLOR);
  QBrush brush(DEADCELL);

  paint.setPen(pen);
  paint.setBrush(Qt::SolidPattern);
  for(int y = game->getFieldHeight()-1; y >= 0; y--)
      for(int x = 0; x < game->getFieldWidth(); x++)
        {
          if ( game->getField(x,y) == ALIVE ) brush.setColor(ALIVECELL);
          else brush.setColor(DEADCELL);
          paint.setBrush(brush);
          paint.drawRect(x*FIELDSIZE,y*FIELDSIZE,FIELDSIZE,FIELDSIZE);
        }
  setFixedSize(game->getFieldWidth()*FIELDSIZE+1,game->getFieldHeight()*FIELDSIZE+1);
}
