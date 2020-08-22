#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "game.h"
#include "fieldwidget.h"


class View : public QWidget
{
    Q_OBJECT
  private:
    QTimer *timer;
    QSpinBox *sbFieldHorizontal;
    QSpinBox *sbFieldVertical;
    QCheckBox *cbTorus;
    QCheckBox *cbEndlessMode;
    QCheckBox *cbAging;
    QPushButton *pbNextFrame;
    QPushButton *pbPlayTimer;
    QPushButton *pbPauseTimer;
    QPushButton *pbExit;
    QSpinBox *sbTimerSpeed;
    QPushButton *pbReset;
    FieldWidget *field;
    Game *game;

  public:
    View(QWidget *parent = 0);
    ~View();
    bool timerActive;
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);
  public slots:
    void toggleStartPauseMode();
    void setViewMapSize();
    void setPlayerSpeed();
    void startDrawWhileTimer();
    void endDrawWhileTimer();
    void setConfigModes();
};

#endif // VIEW_H
