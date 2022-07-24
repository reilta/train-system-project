#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <semaphore.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    ~MainWindow();


public slots:
    void start();
    void stop();
    void speed();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
