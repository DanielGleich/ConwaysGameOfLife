#include "game.h"

Game::Game(QObject* parent)
  : QObject(parent)
{
  currentHeight = FIELDHEIGHT;
  currentWidth = FIELDWIDTH;
  isTorus = TORUS;
  reset();

}

int Game::getFieldWidth() const
{ return currentWidth; }

int Game::getFieldHeight() const
{ return currentHeight; }

int Game::getFieldSize() const
{ return currentWidth*currentHeight; }

int Game::getIndex(int x, int y) const
{
  if ( x < 0 || y < 0 || x >= currentWidth || y >= currentHeight)
    qDebug() << "FEHLER: Index mit x=" << x << "und y= " << y << "wurde aufgerufen";
  return (currentWidth*y+x);
}

CELLSTATE*Game::getCurrentField() const
{ return currentField; }

CELLSTATE Game::getField(int x, int y) const
{ return currentField[getIndex(x,y)]; }

bool Game::isTorusMode() const
{ return isTorus; }

void Game::setField(int x, int y, CELLSTATE status)
{ currentField[getIndex(x,y)] = status; }

void Game::setMapSize(int width, int height)
{
  if ( width < 5 || height < 5)
    {
      currentHeight = 5;
      currentWidth = 5;
    }
  currentHeight = height;
  currentWidth = width;
  reset();
}

void Game::setTorus(int state)
{
  if (state == Qt::Unchecked)
    isTorus = false;
  else isTorus = true;
}


void Game::printCurrentField() const
{
  qDebug() << "Current-Field";
  for (int y = currentHeight-1; y >= 0; y--)
    {
      QString row = "";
      for (int x = 0; x < currentWidth; x++)
        {
          QString cellstate = getField(x,y) == DEAD ? "0" : "1";
          row.append(cellstate);
        }
      qDebug() << row;
    }
  qDebug() << "";
}

void Game::printNextField() const
{
  qDebug() << "Next-Field";
  for (int y = currentHeight-1; y >= 0; y--)
    {
      QString row = "";
      for (int x = 0; x < currentWidth; x++)
        {
          QString cellstate = nextField[getIndex(x,y)] == DEAD ? "0" : "1";
          row.append(cellstate);
        }
      qDebug() << row;
    }
  qDebug() << "";
}

void Game::clearCurrentField()
{
  for (int x = 0; x < currentWidth; x++)
    for (int y = 0; y < currentHeight; y++)
      currentField[getIndex(x,y)] = DEAD;
}

void Game::clearNextField()
{

  for (int x = 0; x < currentWidth; x++)
    for (int y = 0; y < currentHeight; y++)
      nextField[getIndex(x,y)] = DEAD;
}

void Game::isFieldEmpty()
{
  for(int i=0; i<getFieldSize();i++)
    if(currentField[i] == ALIVE) return;
  randomField();
}

int Game::countNeighbors(int x, int y) const
{
  int i = 0;
  if ( isTorus )
    {
      for (int xOffset = -1; xOffset < 2;xOffset++)
        for (int yOffset = -1; yOffset < 2; yOffset++)
          {
            if (xOffset == 0 && yOffset == 0) continue;
            int torusX = x+xOffset;
            int torusY = y+yOffset;

            if(torusX >= currentWidth)
              torusX -= currentWidth;
            if(torusX < 0)
              torusX += currentWidth;

            if(torusY >= currentHeight)
              torusY -= currentHeight;
            if(torusY < 0)
              torusY += currentHeight;

            if(currentField[getIndex(torusX,torusY)] == ALIVE) i++;
          }
    }
  else
    {
      for (int xOffset = -1; xOffset < 2;xOffset++)
        for (int yOffset = -1; yOffset < 2; yOffset++)
          {
            int actualX = x + xOffset;
            int actualY = y + yOffset;
            if(xOffset == 0 && yOffset == 0) continue;
            else if(isValidField(actualX,actualY) && currentField[getIndex(actualX,actualY)] == ALIVE) i++;
          }
    }
  return i;
}

bool Game::isValidField(int x, int y) const
{
  if (!isTorus)
    if ( x < 0 || x >= currentWidth || y < 0 || y >= currentHeight)
      return false;
  return true;
}

CELLSTATE Game::cellstateForNextFrame(int x, int y) const
{
  CELLSTATE cell = getField(x,y);
  int neighbors = countNeighbors(x,y);
  if (cell == DEAD && neighbors == 3) return ALIVE;                            // REGEL 1
  else if (cell == ALIVE && neighbors < 2) return DEAD;                        // REGEL 2
  else if (cell == ALIVE && neighbors > 1 && neighbors < 4) return ALIVE;      // REGEL 3
  else if (cell == ALIVE && neighbors > 3) return DEAD;                        // REGEL 4
  else return DEAD;
}

void Game::nextFrame()
{
  for (int y = 0; y < currentHeight; y++)
    for(int x = 0 ; x < currentWidth; x++)
        nextField[getIndex(x,y)] = cellstateForNextFrame(x,y);
  currentField = nextField;
  nextField = new CELLSTATE[currentHeight*currentWidth];
  clearNextField();
  emit fieldChanged();
  if(ENDLESSMODE) isFieldEmpty();
}

void Game::randomField()
{
  for(int i = 0 ; i < getFieldSize(); i++)
    currentField[i] = std::rand()%2 == 1 ? ALIVE : DEAD;
  emit fieldChanged();
}

void Game::reset()
{
  delete currentField;
  delete nextField;
  currentField = nullptr;
  nextField = nullptr;

  currentField = new CELLSTATE[currentHeight*currentWidth];
  nextField = new CELLSTATE[currentHeight*currentWidth];

  clearCurrentField();
  clearNextField();
}
