#include <QtTest>
#include <QDebug>
#include <QObject>
#include "game.h"

class GameOfLifeTest : public QObject
{
  Q_OBJECT

public:
  GameOfLifeTest();
  Game *game;

private slots:
  void init();
  void cleanup();
  void checkFieldSize();
  void checkDeadField();
  void checkAliveField();
  void checkMapSizeChange();
  void checkSingleAlive();
  void checkGameReset();
  void checkNeighborCount();
  void checkTorus();
  void checkTorus2();
  void checkFirstRule();
  void checkSecondRule();
  void checkThirdRule();
  void checkFourthRule();
  void checkThreeSteps();
  void checkThreeStepsTorus();
  void checkAgeMap();

private:
  void compareFields(CELLSTATE *field)
  {
    for(int x = 0; x < game->getFieldWidth(); x++)
      for(int y = 0; y < game->getFieldHeight(); y++)
        QCOMPARE(game->getField(x,y),field[y*game->getFieldWidth()+x]);
  }

  void compareAgeMap(int *field)
  {
    for(int x = 0; x < game->getFieldWidth(); x++)
      for(int y = 0; y < game->getFieldHeight(); y++)
        QCOMPARE(game->getAgeMapField(x,y),field[y*game->getFieldWidth()+x]);
  }
};

GameOfLifeTest::GameOfLifeTest()
{

}

void GameOfLifeTest::init()
{
  game = new Game();
}

void GameOfLifeTest::cleanup()
{
  delete game;
  game = nullptr;
}

void GameOfLifeTest::checkFieldSize()
{
  QCOMPARE(game->getFieldWidth(),FIELDWIDTH);
  QCOMPARE(game->getFieldHeight(),FIELDHEIGHT);
  QCOMPARE(game->getFieldSize(),FIELDWIDTH*FIELDHEIGHT);
}

void GameOfLifeTest::checkDeadField()
{
  CELLSTATE field[game->getFieldSize()] = { DEAD };
  compareFields(field);
  for ( int x = 0 ; x < game->getFieldWidth(); x++)
    for (int y = 0; y < game->getFieldHeight(); y++)
      QCOMPARE(game->getField(x,y),DEAD);
}

void GameOfLifeTest::checkAliveField()
{
  for ( int x = 0 ; x < game->getFieldWidth(); x++)
    for (int y = 0; y < game->getFieldHeight(); y++)
      game->setField(x,y,ALIVE);

  for ( int x = 0 ; x < game->getFieldWidth(); x++)
    for (int y = 0; y < game->getFieldHeight(); y++)
      QCOMPARE(game->getField(x,y),ALIVE);
}

void GameOfLifeTest::checkMapSizeChange()
{
  game->setField(0,0,ALIVE);
  QCOMPARE(game->getField(0,0),ALIVE);
  game->setMapSize(4,3);
  QCOMPARE(game->getFieldWidth(),4);
  QCOMPARE(game->getFieldHeight(),3);

  CELLSTATE field[] = {DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD};
  compareFields(field);
}

void GameOfLifeTest::checkSingleAlive()
{
  game->setMapSize(3,3);
  game->setField(2,2,ALIVE);

  CELLSTATE field[] = {DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,
                       DEAD,DEAD,ALIVE};
  compareFields(field);
}

void GameOfLifeTest::checkGameReset()
{
  game->setMapSize(5,5);
  game->setField(0,0,ALIVE);
  game->setField(2,3,ALIVE);
  game->reset();

  CELLSTATE field[] = {DEAD,DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,DEAD};

  compareFields(field);
}

void GameOfLifeTest::checkNeighborCount()
{
  game->setMapSize(3,3);
  game->setField(1,1,ALIVE);
  game->setField(0,0,ALIVE);

  game->setTorus(true);
  QCOMPARE(game->countNeighbors(1,1),1);
  QCOMPARE(game->countNeighbors(0,2),2);
  QCOMPARE(game->countNeighbors(0,1),2);
  QCOMPARE(game->countNeighbors(2,2),2);

  game->setTorus(false);
  QCOMPARE(game->countNeighbors(1,1),1);
  QCOMPARE(game->countNeighbors(0,2),1);
  QCOMPARE(game->countNeighbors(0,1),2);
  QCOMPARE(game->countNeighbors(2,2),1);

}

void GameOfLifeTest::checkTorus()
{
  game->setMapSize(5,5);
  game->setField(0,0,ALIVE);
  game->setField(0,1,ALIVE);
  game->setField(1,0,ALIVE);

  game->setTorus(true);
  QCOMPARE(game->countNeighbors(0,4),2);
  QCOMPARE(game->countNeighbors(3,4),0);

  game->setTorus(false);
  QCOMPARE(game->countNeighbors(0,4),0);
  QCOMPARE(game->countNeighbors(3,4),0);
}

void GameOfLifeTest::checkTorus2()
{
  game->setMapSize(3,3);
  for(int x = 0; x < game->getFieldWidth(); x++)
    for (int y = 0; y < game->getFieldHeight(); y++)
      game->setField(x,y,ALIVE);

  game->setTorus(true);
  QCOMPARE(game->countNeighbors(2,1),8);

  game->setTorus(false);
  QCOMPARE(game->countNeighbors(2,1),5);
}

void GameOfLifeTest::checkFirstRule()
{
  game->setMapSize(3,3);
  game->setTorus(false);
  game->setField(0,0,ALIVE);
  game->setField(0,1,ALIVE);
  game->setField(1,0,ALIVE);

  game->nextFrame();

  CELLSTATE field[] = {ALIVE,ALIVE,DEAD,
                       ALIVE,ALIVE,DEAD,
                       DEAD,DEAD,DEAD};

  compareFields(field);
}

void GameOfLifeTest::checkSecondRule()
{
  game->setMapSize(3,3);
  game->setField(0,0,ALIVE);
  game->setField(1,1,ALIVE);
  game->nextFrame();

  CELLSTATE field[] = {DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD};
  compareFields(field);
}

void GameOfLifeTest::checkThirdRule()
{
  game->setMapSize(3,3);
  game->setField(0,0,ALIVE);
  game->setField(1,0,ALIVE);
  game->setField(2,0,ALIVE);
  game->setField(1,1,ALIVE);

  /*
      0 0 0
      0 1 0
      1 1 1

      0 0 0
      1 1 1
      1 1 1
  */
  game->nextFrame();
  CELLSTATE field[] = {ALIVE,ALIVE, ALIVE,
                       DEAD,ALIVE,DEAD,
                       DEAD,DEAD,DEAD};
  compareFields(field);
}

void GameOfLifeTest::checkFourthRule()
{
  game->setMapSize(3,3);
  for(int x = 0 ; x < 3; x++)
    for(int y = 0; y < 3; y++)
      game->setField(x,y,ALIVE);
  game->nextFrame();

  CELLSTATE field[] = {DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD,
                       DEAD,DEAD,DEAD};
  compareFields(field);
}

void GameOfLifeTest::checkThreeSteps()
{
  game->setMapSize(5,5);
  game->setTorus(false);
  game->setField(3,0,ALIVE);
  game->setField(4,0,ALIVE);
  game->setField(3,1,ALIVE);
  game->setField(1,2,ALIVE);
  game->setField(1,3,ALIVE);
  game->setField(2,3,ALIVE);
  game->setField(3,3,ALIVE);
  game->nextFrame();

  CELLSTATE field[] = {DEAD,DEAD,DEAD,ALIVE,ALIVE,
                       DEAD,DEAD,ALIVE,ALIVE,ALIVE,
                       DEAD,ALIVE,DEAD,ALIVE,DEAD,
                       DEAD,ALIVE,ALIVE,DEAD,DEAD,
                       DEAD,DEAD,ALIVE,DEAD,DEAD};
  compareFields(field);

  CELLSTATE field2[] = {DEAD,DEAD,ALIVE,DEAD,ALIVE,
                        DEAD,DEAD,DEAD,DEAD,DEAD,
                        DEAD,ALIVE,DEAD,DEAD,ALIVE,
                        DEAD,ALIVE,DEAD,ALIVE,DEAD,
                        DEAD,ALIVE,ALIVE,DEAD,DEAD};

  game->nextFrame();
  compareFields(field2);
}

void GameOfLifeTest::checkThreeStepsTorus()
{
  game->setMapSize(5,5);
  game->setField(3,0,ALIVE);
  game->setField(4,0,ALIVE);
  game->setField(3,1,ALIVE);
  game->setField(1,2,ALIVE);
  game->setField(1,3,ALIVE);
  game->setField(2,3,ALIVE);
  game->setField(3,3,ALIVE);

  game->nextFrame();
  CELLSTATE field[] = {DEAD,DEAD,DEAD,ALIVE,ALIVE,
                       DEAD,DEAD,ALIVE,ALIVE,ALIVE,
                       DEAD,ALIVE,DEAD,ALIVE,DEAD,
                       DEAD,ALIVE,ALIVE,DEAD,DEAD,
                       DEAD,DEAD,DEAD,DEAD,ALIVE};
  compareFields(field);
  game->nextFrame();
  CELLSTATE field2[] = {ALIVE,DEAD,ALIVE,DEAD,DEAD,
                        ALIVE,DEAD,DEAD,DEAD,DEAD,
                        ALIVE,ALIVE,DEAD,DEAD,ALIVE,
                        ALIVE,ALIVE,ALIVE,ALIVE,DEAD,
                        ALIVE,DEAD,ALIVE,DEAD,ALIVE};
  compareFields(field2);
}

void GameOfLifeTest::checkAgeMap()
{
  int field[] = {0,0,0,0,0,
                 0,0,0,0,0,
                 0,1,1,1,0,
                 0,0,0,0,0,
                 0,0,0,0,0};

  game->setTorus(false);
  game->setAging(true);
  game->setMapSize(5,5);
  game->setMinAge(3);
  game->setMaxAge(4);


  game->setField(1,2,ALIVE);
  game->setField(2,2,ALIVE);
  game->setField(3,2,ALIVE);

  game->printAgeMap();
  compareAgeMap(field);

  int field2[] = {0,0,0,0,0,
                  0,0,1,0,0,
                  0,0,2,0,0,
                  0,0,1,0,0,
                  0,0,0,0,0};

  game->nextFrame();
  game->printAgeMap();
  compareAgeMap(field2);

  int field3[] = {0,0,0,0,0,
                  0,0,0,0,0,
                  0,1,3,1,0,
                  0,0,0,0,0,
                  0,0,0,0,0};

  game->nextFrame();
  game->printAgeMap();
  compareAgeMap(field3);
}

QTEST_APPLESS_MAIN(GameOfLifeTest)

#include "tst_gameoflifetest.moc"
