#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent *) {
    if(p.empty()) {return;}
    QPainter painter(this);
    QPen pen(QColor(128,128,128));
    painter.setPen(pen);
    const QPoint start(20, 80);
    const QPoint dir[] = {QPoint(0,W), QPoint(W,0)};
    int length = 0;
    int steps = 0;
    QPoint pos = start;
    QRect rect(pos, QSize(W, W));
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
                length++;
            } else {
                pos = start + i*dir[0] + j*dir[1];
                painter.setBrush(QColor(255,255,0));
                rect.moveTo(pos);
                painter.drawRect(rect);
                steps++;
            }
        }
    }
    std::string str = length == 0 ? "unreachable" : std::to_string(length);
    ui->distance->setText(QString::fromStdString(str));
    ui->steps->setText(QString::fromStdString(std::to_string(steps + length)));
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
            QString s = QString::fromStdString("(" + std::to_string(x) + "," + std::to_string(y) + ")");
            ui->start_point->setText(s);
        } else if(select_end) {
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
            QString s = QString::fromStdString("(" + std::to_string(x) + "," + std::to_string(y) + ")");
            ui->end_point->setText(s);
        } else if (p[y][x] == "O") {
            p[y][x] = "X";
        } else if (p[y][x] == "X") {
            p[y][x] = "O";
        }

        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton && event->pos().x() >= 20 && event->pos().x() <= 20 + W * N
            && event->pos().y() >= 80 && event->pos().y() <= 80 + W * M) {
        QPoint point = event->pos();
        int x = (point.x()-20)/W;
        int y = (point.y()-80)/W;
        if (p[y][x] == "O") {
            p[y][x] = "X";
        } else if (p[y][x] == "X") {
            p[y][x] = "O";
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
    } else if(W*M + 100 < 320) {
        height = 600;
    }
    if (width < W*N + 40) {
        width = W*N + 40;
    } else if(W*N + 40 < 480) {
        width = 800;
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
    pList = a.start();
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
    if (i < pList.size()) {
        p = pList[i];
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
