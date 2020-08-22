#include "game.h"

Game::Game(QObject* parent)
  : QObject(parent)
{
  currentHeight = FIELDHEIGHT;
  currentWidth = FIELDWIDTH;
  min_age = MIN_AGE;
  max_age = MAX_AGE;
  isTorus = TORUS;
  isAging = AGING;
  isEndless = ENDLESS_MODE;
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

int Game::getAgeMapField(int x, int y) const
{ return ageMap[getIndex(x,y)]; }

int Game::getMinAge()
{ return min_age; }

int Game::getMaxAge()
{ return max_age; }

bool Game::isTorusMode() const
{ return isTorus; }

void Game::setField(int x, int y, CELLSTATE status)
{
  currentField[getIndex(x,y)] = status;
  if(isAging) ageMap[getIndex(x,y)] = status == ALIVE ? 1 : 0;
}

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

void Game::setMinAge(int age)
{
  if (age >= max_age)
    min_age = max_age;
  else min_age = age;
}

void Game::setMaxAge(int age)
{
  if (age <= min_age)
    max_age = min_age;
  else max_age = age;
}

void Game::setAging(bool state)
{ isAging = state; }

void Game::setEndlessMode(bool state)
{ isEndless = state; }

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

void Game::printAgeMap() const
{
  qDebug() << "Agemap";
  for (int y = currentHeight-1; y >= 0; y--)
    {
      QString row = "";
      for (int x = 0; x < currentWidth; x++)
        {
          QString cellstate = QString::number(getAgeMapField(x,y));
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

void Game::clearAgeMap()
{
  for(int x = 0; x < currentWidth; x++)
    for(int y = 0; y < currentHeight; y++)
      ageMap[getIndex(x,y)] = 0;
}

void Game::isFieldEmpty()
{
  for(int i=0; i<getFieldSize();i++)
    if(currentField[i] != DEAD) return;
  randomField();
}

CELLSTATE Game::aliveAfterAging(int x, int y)
{
  int *ageCell = &ageMap[getIndex(x,y)];
  srand(time(NULL));
  int random = rand()%3;
  bool state = random == 0? false : true;
  if (state) return ALIVE;
  else
    {
      *ageCell = 0;
      return DEAD;
    }
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

            if(currentField[getIndex(torusX,torusY)] != DEAD) i++;
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
            else if(isValidField(actualX,actualY) && currentField[getIndex(actualX,actualY)] != DEAD) i++;
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

CELLSTATE Game::cellstateForNextFrame(int x, int y)
{
  CELLSTATE cell = getField(x,y);
  int neighbors = countNeighbors(x,y);

  if (cell == DEAD && neighbors == 3 || cell != DEAD && neighbors > 1 && neighbors < 4)
    {
      ageMap[getIndex(x,y)]++;
      return ALIVE;
    }
  else
    {
      ageMap[getIndex(x,y)] = 0;
      return DEAD;
    }
}

void Game::nextFrame()
{

  for (int y = 0; y < currentHeight; y++)
    for(int x = 0 ; x < currentWidth; x++)
      nextField[getIndex(x,y)] = cellstateForNextFrame(x,y);
  if(isAging)
    {
      for (int y = 0; y < currentHeight; y++)
        for(int x = 0 ; x < currentWidth; x++)
          {
            if (ageMap[getIndex(x,y)] > max_age)
              {
                ageMap[getIndex(x,y)] = 0;
                nextField[getIndex(x,y)] = DEAD;
              }
            else if (ageMap[getIndex(x,y)] > min_age) nextField[getIndex(x,y)] = aliveAfterAging(x,y);
          }
    }
  delete currentField;
  currentField = nextField;
  nextField = nullptr;
  nextField = new CELLSTATE[currentHeight*currentWidth];
  clearNextField();
  if (isEndless) isFieldEmpty();
  emit fieldChanged();
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
  delete ageMap;
  currentField = nullptr;
  nextField = nullptr;
  ageMap = nullptr;

  currentField = new CELLSTATE[currentHeight*currentWidth];
  nextField = new CELLSTATE[currentHeight*currentWidth];
  ageMap = new int[currentHeight*currentWidth];

  clearCurrentField();
  clearNextField();
  clearAgeMap();
}
