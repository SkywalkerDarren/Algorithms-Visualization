#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPen pen(QColor(0,0,0));
    QBrush brush(QColor(255,255,255));
    painter.setBrush(brush);
    painter.setPen(pen);
    const QPoint start(20, 80);
    const QPoint dir[] = {QPoint(0,W), QPoint(W,0)};

    QPoint pos = start;
    QRect rect(pos, QSize(W, W));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            rect.moveTo(pos);
            painter.drawRect(rect);
            pos += dir[1];
        }
        pos -= N * dir[1];
        pos += dir[0];
    }
    if(!p.empty()) {
        for(int i = 0; i < p.size(); i++) {
            for(int j = 0; j < p[i].size(); j++) {
                if(p[i][j] == "O") {
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(0,0,0));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                } else if (p[i][j] == "X"){
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(255,255,255));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                } else if (p[i][j] == "S"){
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(0,0,255));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                } else if (p[i][j] == "E") {
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(255,0,0));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                } else if (p[i][j] == "A"){
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(0,255,0));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                } else {
                    pos = start + i*dir[0] + j*dir[1];
                    painter.setBrush(QColor(255,255,0));
                    rect.moveTo(pos);
                    painter.drawRect(rect);
                }
            }
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton && event->pos().x() >= 20 && event->pos().x() <= 20 + W * N
            && event->pos().y() >= 80 && event->pos().y() <= 80 + W * M) {
        QPoint point = event->pos();
        int x = (point.x()-20)/W;
        int y = (point.y()-80)/W;
        if(select_start) {
            std::cout << (point.x()-20)/W << "," << (point.y()-80)/W << std::endl;
            static int xt,yt;
            static std::string tt;
            if (x1!=-1 && p[xt][yt] == "S") {
                p[xt][yt] = tt;
            }
            y1 = x;
            x1 = y;
            xt = x1;
            yt = y1;
            tt = p[x1][y1];
            p[x1][y1] = "S";
            select_start = false;
        }
        if(select_end) {
            std::cout << (point.x()-20)/W << "," << (point.y()-80)/W << std::endl;
            static int xt,yt;
            static std::string tt;
            if (x2!=-1 && p[xt][yt] == "E") {
                p[xt][yt] = tt;
            }
            x2 = y;
            y2 = x;
            xt = x2;
            yt = y2;
            tt = p[x2][y2];
            p[x2][y2] = "E";
            select_end = false;
        }
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
    } else if(W*M + 100 < 300) {
        height = 300;
    }
    if (width < W*N + 40) {
        width = W*N + 40;
    } else if(W*N + 40 < 400) {
        width = 400;
    }
    this->resize(width, height);

    // blank map
    p.clear();
    for (int i = 0; i < M; i++) {
        std::vector<std::string> pt;
        for (int j = 0; j < N; j++) {
            pt.push_back("X");
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
    double sq = std::sqrt(row*col);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0, 3);
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = myclock::now() - beginning;
    unsigned seed = d.count();
    generator.seed(seed);
    p.clear();
    for (int i = 0; i < row; i++) {
        std::vector<std::string> pt;
        for (int j = 0; j < col; j++) {
            int k = distribution(generator) < 1;
            if(k) {
                pt.push_back("O");
            } else {
                pt.push_back("X");
            }
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
    if(p.empty()) {
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
    p = a.start();
    update();
}

void MainWindow::on_clear_btn_clicked()
{
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