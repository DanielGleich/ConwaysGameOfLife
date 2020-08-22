#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QDebug>
#include <ctime>
#include <cstdlib>
#include <QtWidgets>

#include "config.h"

class Game : public QObject
{
  Q_OBJECT

private:
  CELLSTATE                 *currentField = nullptr;
  CELLSTATE                 *nextField = nullptr;
  int                       *ageMap = nullptr;
  int                       currentWidth, currentHeight;
  int                       min_age,max_age;
  bool                      isTorus;
  bool                      isAging;
  bool                      isEndless;

public:
  Game(QObject *parent = nullptr);
  int                       getFieldWidth() const;
  int                       getFieldHeight() const;
  int                       getFieldSize() const;
  int                       getIndex(int x, int y) const;
  CELLSTATE*                getCurrentField() const;
  CELLSTATE                 getField(int x, int y) const;
  int                       getAgeMapField(int x, int y) const;
  int                       getMinAge();
  int                       getMaxAge();
  bool                      isTorusMode() const;
  int                       countNeighbors(int x, int y) const;
  bool                      isValidField(int x, int y) const;

  CELLSTATE                 cellstateForNextFrame(int x, int y);
  void                      printCurrentField() const;
  void                      printNextField() const;
  void                      printAgeMap() const;
  void                      clearCurrentField();
  void                      clearNextField();
  void                      clearAgeMap();
  void                      isFieldEmpty();
  CELLSTATE                 aliveAfterAging(int x, int y);

public slots:
  void                      reset();
  void                      setField(int x, int y, CELLSTATE status);
  void                      setMapSize(int width, int height);
  void                      setTorus(int state = Qt::PartiallyChecked);
  void                      setMinAge(int age);
  void                      setMaxAge(int age);
  void                      setAging(bool state);
  void                      setEndlessMode(bool state);
  void                      nextFrame();
  void                      randomField();
signals:
  void fieldChanged();
};

#endif // GAME_H
