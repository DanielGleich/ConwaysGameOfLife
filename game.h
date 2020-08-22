#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QDebug>
#include <cstdlib>

#include "config.h"

class Game : public QObject
{
  Q_OBJECT

private:
  CELLSTATE                 *currentField = nullptr;
  CELLSTATE                 *nextField = nullptr;
  int                       currentWidth, currentHeight;
  bool                      isTorus;

public:
  Game(QObject *parent = nullptr);
  int                       getFieldWidth() const;
  int                       getFieldHeight() const;
  int                       getFieldSize() const;
  int                       getIndex(int x, int y) const;
  CELLSTATE*                getCurrentField() const;
  CELLSTATE                 getField(int x, int y) const;
  bool                      isTorusMode() const;
  bool timerActive;

  int                       countNeighbors(int x, int y) const;
  bool                      isValidField(int x, int y) const;

  CELLSTATE                 cellstateForNextFrame(int x, int y) const;
  void                      printCurrentField() const;
  void                      printNextField() const;
  void                      clearCurrentField();
  void                      clearNextField();
  void                      isFieldEmpty();
public slots:
  void                      reset();
  void                      setField(int x, int y, CELLSTATE status);
  void                      setMapSize(int width, int height);
  void                      setTorus(int state = Qt::PartiallyChecked);
  void                      nextFrame();
  void                      randomField();
signals:
  void fieldChanged();
};

#endif // GAME_H
