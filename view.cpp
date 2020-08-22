#include "view.h"

View::View(QWidget *parent)
  : QWidget(parent)
{
  setWindowTitle("Conway's Game of Life");
  QVBoxLayout *configBox = new QVBoxLayout();
  QVBoxLayout *layout = new QVBoxLayout();
  this->move(0,0);
  sbFieldVertical = new QSpinBox(this);
  sbFieldHorizontal = new QSpinBox(this);
  cbTorus = new QCheckBox(this);
  cbAging = new QCheckBox(this);
  cbEndlessMode = new QCheckBox(this);
  pbNextFrame = new QPushButton("Nächster Schritt",this);
  pbPlayTimer = new QPushButton("Start",this);
  sbTimerSpeed = new QSpinBox(this);
  pbPauseTimer = new QPushButton("Pause",this);
  pbReset = new QPushButton("Feld zurücksetzen",this);
  pbExit = new QPushButton("Beenden",this);
  timer = new QTimer(this);
  game = new Game();
  field = new FieldWidget(game);
  timerActive = false;
  game->setMapSize(95,50);
  game->randomField();

  QLabel *lbFieldHorizontal = new QLabel("Feldbreite",this);
  QLabel *lbFieldVertical = new QLabel("Feldhöhe",this);
  QLabel *lbVelocity = new QLabel("Schnelligkeit:",this);

  lbFieldHorizontal->setMaximumSize(100,20);
  lbFieldVertical->setMaximumSize(100,20);
  lbVelocity->setFixedSize(100,20);

  cbTorus->setText("Torus-Modus?");
  cbEndlessMode->setText("Endlos-Modus?");
  cbAging->setText("Altern?");


  pbPauseTimer->setMinimumWidth(100);
  pbPlayTimer->setMinimumWidth(100);
  pbNextFrame->setMinimumWidth(150);
  pbReset->setMinimumWidth(150);


  sbFieldVertical->setRange(5,98);
  sbFieldVertical->setMaximumWidth(50);
  sbFieldHorizontal->setRange(5,190);
  sbFieldHorizontal->setMaximumWidth(50);
  sbTimerSpeed->setMaximumWidth(50);
  sbTimerSpeed->setRange(1,10);

  pbPauseTimer->setDisabled(true);

  cbTorus->setChecked(true);
  cbEndlessMode->setChecked(true);
  cbAging->setChecked(true);
  sbFieldHorizontal->setValue(game->getFieldWidth());
  sbFieldVertical->setValue(game->getFieldHeight());

  connect(pbPlayTimer, SIGNAL(clicked()),this,SLOT(toggleStartPauseMode()));
  connect(pbNextFrame, SIGNAL(clicked()),game,SLOT(nextFrame()));
  connect(game, SIGNAL(fieldChanged()),field,SLOT(update()));
  connect(pbReset, SIGNAL(clicked()),game,SLOT(reset()));
  connect(pbReset, SIGNAL(clicked()),field,SLOT(update()));
  connect(pbPauseTimer, SIGNAL(clicked()),this,SLOT(toggleStartPauseMode()));
  connect(pbExit, SIGNAL(clicked()),this,SLOT(close()));
  connect(pbExit, SIGNAL(clicked()),field,SLOT(close()));
  connect(sbFieldHorizontal,SIGNAL(valueChanged(int)),this,SLOT(setViewMapSize()));
  connect(sbFieldVertical,SIGNAL(valueChanged(int)),this,SLOT(setViewMapSize()));
  connect(sbTimerSpeed,SIGNAL(valueChanged(int)),this,SLOT(setPlayerSpeed()));
  connect(timer,SIGNAL(timeout()),game,SLOT(nextFrame()));
  connect(cbTorus, SIGNAL(stateChanged(int)),game, SLOT(setTorus(int)));
  connect(cbEndlessMode, SIGNAL(stateChanged(int)), this,SLOT(setConfigModes()));
  connect(cbAging, SIGNAL(stateChanged(int)), this,SLOT(setConfigModes()));
  //connect(game, SIGNAL(keyPressEventSignal(e)),this,SLOT(keyPressEvent(e)));

  configBox->addWidget(lbFieldHorizontal);
  configBox->addWidget(sbFieldHorizontal);
  configBox->addWidget(lbFieldVertical);
  configBox->addWidget(sbFieldVertical);
  configBox->addWidget(cbTorus);
  configBox->addWidget(cbAging);
  configBox->addWidget(cbEndlessMode);
  configBox->addWidget(pbNextFrame);
  configBox->addWidget(pbPlayTimer);
  configBox->addWidget(lbVelocity);
  configBox->addWidget(sbTimerSpeed);
  configBox->addWidget(pbPauseTimer);
  configBox->addWidget(pbReset);
  layout->addLayout(configBox);
  layout->addWidget(pbExit);
  layout->setAlignment(Qt::AlignTop);
  setLayout(layout);
  setPlayerSpeed();

  setFocusPolicy(Qt::NoFocus);
  setFocus(Qt::NoFocusReason);
  field->show();
}

View::~View()
{

}

void View::keyPressEvent(QKeyEvent *e)
{
  setFocus();
  if(e->key() == Qt::Key_Space)                                   toggleStartPauseMode();
  if(e->key() == Qt::Key_Plus)                                    sbTimerSpeed->setValue(sbTimerSpeed->value()+1);
  if(e->key() == Qt::Key_Minus)                                   sbTimerSpeed->setValue(sbTimerSpeed->value()-1);
  if(e->key() == Qt::Key_T && cbTorus->isEnabled())               cbTorus->click();
  if(e->key() == Qt::Key_Right && sbFieldHorizontal->isEnabled()) sbFieldHorizontal->setValue(sbFieldHorizontal->value()+1);
  if(e->key() == Qt::Key_Left && sbFieldHorizontal->isEnabled())  sbFieldHorizontal->setValue(sbFieldHorizontal->value()-1);
  if(e->key() == Qt::Key_Up && sbFieldVertical->isEnabled())      sbFieldVertical->setValue(sbFieldVertical->value()+1);
  if(e->key() == Qt::Key_Down && sbFieldVertical->isEnabled())    sbFieldVertical->setValue(sbFieldVertical->value()-1);
  if(e->key() == Qt::Key_R && pbReset->isEnabled())               pbReset->click();
  if(e->key() == Qt::Key_F)                                       game->randomField();

  if(e->key() == Qt::Key_Escape)                                  pbExit->click();
  setFocusPolicy(Qt::NoFocus);
}

void View::closeEvent(QCloseEvent* e)
{
  field->close();
}

void View::toggleStartPauseMode()
{
  if(pbPlayTimer->isEnabled())
  {
    pbPlayTimer->setDisabled(true);
    pbPauseTimer->setDisabled(false);
    sbFieldVertical->setDisabled(true);
    sbFieldHorizontal->setDisabled(true);
    cbTorus->setDisabled(true);
    pbNextFrame->setDisabled(true);
    pbReset->setDisabled(true);
    timer->start();
    timerActive = true;
  }
  else
  {
    pbPlayTimer->setDisabled(false);
    pbPauseTimer->setDisabled(true);
    sbFieldVertical->setDisabled(false);
    sbFieldHorizontal->setDisabled(false);
    cbTorus->setDisabled(false);
    pbNextFrame->setDisabled(false);
    pbReset->setDisabled(false);
    timer->stop();
    timerActive = false;
  }
}

void View::setViewMapSize()
{
  game->setMapSize(sbFieldHorizontal->value(),sbFieldVertical->value());
  field->update();
}

void View::setPlayerSpeed()
{
  timer->setInterval(150/sbTimerSpeed->value());
}

void View::startDrawWhileTimer()
{
  if (!timerActive)
    timer->stop();
}

void View::endDrawWhileTimer()
{
  if (!timerActive)
    timer->start();
}

void View::setConfigModes()
{
  game->setAging(cbAging->isChecked());
  game->setEndlessMode(cbEndlessMode->isChecked());

}
