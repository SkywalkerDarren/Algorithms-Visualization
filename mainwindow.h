#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <vector>
#include <QPainter>
#include <random>
#include <iostream>
#include <QTimer>
#include <QThread>
#include "algorithm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_paint_btn_clicked();

    void on_random_btn_clicked();

    void on_set_start_btn_clicked();

    void on_set_end_btn_clicked();

    void on_start_btn_clicked();

    void on_clear_btn_clicked();

    void updateMap();

private:
    bool reset = false;
    QTimer *timer;
    Ui::MainWindow *ui;
    int W, M, N;
    int x1, x2, y1, y2;
    int degree;
    bool select_start = false, select_end = false, isPaint = false;
    algorithm::nodeMap p;
    void resizeWindow();
    void createRandomMap();

};

#endif // MAINWINDOW_H
