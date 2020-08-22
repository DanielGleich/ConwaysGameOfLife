#include <QString>
#include <QtTest>

#include "game.h"
#include "config.h"

class GameOfLifeTest : public QObject
{
    Q_OBJECT

  public:
    GameOfLifeTest();
    Game *game;

  private Q_SLOTS:
    void init();
    void cleanup();
    void checkFieldSize();
    void checkEmptyField();
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
  QCOMPARE(game->getHeight(), MAPHEIGHT);
  QCOMPARE(game->getLenght(), MAPLENGHT);
}

void GameOfLifeTest::checkEmptyField()
{
  game->getField(x,y);
}

QTEST_APPLESS_MAIN(GameOfLifeTest)

#include "tst_gameoflife.moc"
