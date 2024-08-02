#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CApple2eHardware;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startPc();
    void stopPc();
    void resetPc();

    void saveImage();

protected:
    void timerEvent(QTimerEvent *event);
    bool event(QEvent *ev);
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    void setOAActive(int active);
    void setSAActive(int active);
    void buildComputer();
private:
    CApple2eHardware *m_system;
    Ui::MainWindow *ui;

    bool oaDown;
    bool saDown;
};
#endif // MAINWINDOW_H
