#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::printRect(int i, int j, QPainter painter, QColor color)
{
    QPoint pos(20, 80);
    QRect rect(pos, QSize(W, W));
    const QPoint dir[] = {QPoint(0,W), QPoint(W,0)};
    pos = pos + i*dir[0] + j*dir[1];
    painter.setBrush(color);
    rect.moveTo(pos);
    painter.drawRect(rect);
}

void MainWindow::paintEvent(QPaintEvent *) {
    if(p.empty()) {return;}
    // init
    QPainter painter(this);
    QPen pen(QColor(128,128,128));
    painter.setPen(pen);
    int length = 0;
    int steps = 0;
    // print
    QRect rect(pos, QSize(W, W));
    for(int i = 0; i < p.size(); i++) {
        for(int j = 0; j < p[i].size(); j++) {
            if (p[i][j].g > printStep && p[i][j].val != "S" && p[i][j].val != "E" && p[i][j].val != "X") {
                printRect(i, j, painter, QColor(255,255,255));
                continue;
            }

            if(p[i][j].val == "O") {
                printRect(i, j, painter, QColor(0,0,0));
            } else if (p[i][j].val == "X"){
                printRect(i, j, painter, QColor(255,255,255));
            } else if (p[i][j].val == "S"){
                printRect(i, j, painter, QColor(0,0,255));
            } else if (p[i][j].val == "E") {
                printRect(i, j, painter, QColor(255,0,0));
            } else if (p[i][j].val == "A"){
                printRect(i, j, painter, QColor(0,255,0));
                length++;
            } else {
                printRect(i, j, painter, QColor(255,255,0));
                steps++;
            }
        }
    }
    std::string str = length == 0 ? "unreachable" : std::to_string(length);
    ui->distance->setText(QString::fromStdString(str));
    ui->steps->setText(QString::fromStdString(std::to_string(steps + length)));
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && event->pos().x() >= 20 && event->pos().x() < 20 + W * N
            && event->pos().y() >= 80 && event->pos().y() < 80 + W * M) {
        QPoint point = event->pos();
        int x = (point.x()-20)/W;
        int y = (point.y()-80)/W;
        if(select_start) {
            std::cout << (point.x()-20)/W << "," << (point.y()-80)/W << std::endl;
            static int xt,yt;
            static std::string tt;
            if (x1!=-1 && p[xt][yt].val == "S") {
                p[xt][yt].val = tt;
            }
            y1 = x;
            x1 = y;
            xt = x1;
            yt = y1;
            tt = p[x1][y1].val;
            p[x1][y1].val = "S";
            select_start = false;
            QString s = QString::fromStdString("(" + std::to_string(x) + "," + std::to_string(y) + ")");
            ui->start_point->setText(s);
        } else if(select_end) {
            std::cout << (point.x()-20)/W << "," << (point.y()-80)/W << std::endl;
            static int xt,yt;
            static std::string tt;
            if (x2!=-1 && p[xt][yt].val == "E") {
                p[xt][yt].val = tt;
            }
            x2 = y;
            y2 = x;
            xt = x2;
            yt = y2;
            tt = p[x2][y2].val;
            p[x2][y2].val = "E";
            select_end = false;
            QString s = QString::fromStdString("(" + std::to_string(x) + "," + std::to_string(y) + ")");
            ui->end_point->setText(s);
        }

        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::RightButton && event->pos().x() >= 20 && event->pos().x() < 20 + W * N
            && event->pos().y() >= 80 && event->pos().y() < 80 + W * M) {
        QPoint point = event->pos();
        int x = (point.x()-20)/W;
        int y = (point.y()-80)/W;
        std::cout << "p[y][x]:" << y << "," << x << std::endl;
        p[y][x].val = "X";
        update();

    }
    if(event->buttons() & Qt::LeftButton && event->pos().x() >= 20 && event->pos().x() < 20 + W * N
            && event->pos().y() >= 80 && event->pos().y() < 80 + W * M) {
        QPoint point = event->pos();
        int x = (point.x()-20)/W;
        int y = (point.y()-80)/W;
        std::cout << "p[y][x]:" << y << "," << x << std::endl;
        p[y][x].val = "O";
        update();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeWindow() {
    W = ui->size_edit->text().toInt();
    M = ui->row_edit->text().toInt();
    N = ui->col_edit->text().toInt();

    int width = this->size().width();
    int height = this->size().height();
    if(height < W*M + 100) {
        height = W*M + 100;
    } else if(W*M + 100 < 600) {
        height = 600;
    }
    if (width < W*N + 40) {
        width = W*N + 40;
    } else if(W*N + 40 < 800) {
        width = 800;
    }
    this->resize(width, height);

    // blank map
    p.clear();
    for (int i = 0; i < M; i++) {
        std::vector<algorithm::preNode> pt;
        for (int j = 0; j < N; j++) {
            algorithm::preNode node = {"X", -1};
            pt.push_back(node);
        }
        p.push_back(pt);
    }

    update();
}

void MainWindow::createRandomMap() {
    if (isPaint == false) {
        return;
    }
    int row = ui->row_edit->text().toInt();
    int col = ui->col_edit->text().toInt();
    double sq = ui->density_edit->text().toDouble();
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0, sq);
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = myclock::now() - beginning;
    unsigned seed = d.count();
    generator.seed(seed);
    p.clear();
    for (int i = 0; i < row; i++) {
        std::vector<algorithm::preNode> pt;
        for (int j = 0; j < col; j++) {
            int k = distribution(generator) < 1;
            algorithm::preNode node;
            if(k) {
                node = {"O", -1};
            } else {
                node = {"X", -1};
            }
            pt.push_back(node);
        }
        p.push_back(pt);
    }
    update();
}

void MainWindow::on_paint_btn_clicked()
{
    isPaint = true;
    resizeWindow();
}

void MainWindow::on_random_btn_clicked()
{
    createRandomMap();
}

void MainWindow::on_set_start_btn_clicked()
{
    select_start = true;
}

void MainWindow::on_set_end_btn_clicked()
{
    select_end = true;
}

void MainWindow::on_start_btn_clicked()
{
    if(p.empty() || (x1==x2&&y1==y2) || x1<0 || x2<0 || y1<0 ||y2<0 ||x1>=M||x2>=M||y1>=N||y2>=N) {
        return;
    }
    QString str = ui->algo_cbx->currentText();
    int (algorithm::distance::*algo)(int, int, int, int) = NULL;
    if(str == "BFS") {
        algo = &algorithm::distance::bfs;
    } else if (str == "Manhattan") {
        algo = &algorithm::distance::manhattan;
    } else if (str == "Euclidean") {
        algo = &algorithm::distance::euclidean;
    } else if (str == "Chebyshev") {
        algo = &algorithm::distance::chebyshev;
    }
    algorithm a(p);
    a.setAlgo(algo);
    timer = new QTimer();
    reset = false;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->setInterval(ui->interval_edit->text().toInt());
    timer->start();
}

void MainWindow::updateMap() {
    static int i = 0;
    if (reset) {
        i = 0;
        timer->stop();
        reset = false;
    }
    if (i < step) {
        printStep = i;
        update();
        i++;
    } else {
        timer->stop();
        i = 0;
        reset = false;
    }
}

void MainWindow::on_clear_btn_clicked()
{
    reset = true;
    if (p.empty()) return;
    for (int i = 0; i < p.size(); i++) {
        for (int j = 0; j < p[0].size(); j++) {
            if (p[i][j] != "E" && p[i][j] != "S" && p[i][j] != "O" && p[i][j] != "X") {
                p[i][j] = "X";
            }
        }
    }
    update();
}
