#include <iostream>
#include <algorithm>
#include <QPainter>
#include <QDebug>
#include <QColor>
#include <QMouseEvent>
#include <QPoint>
#include <QImage>
#include <QRect>

#include "mainwindow.h"
#include "complex_num.h"

const int ITR = 200; // number of iterations
int SZ = 400; // Size of window
const double C = 10.0;
const double EPS = 1e-7;

double BGN_X = -2.0;
double BGN_Y = -2.0;
double STEP = 2.0 / double(SZ);
bool PRESSED_BTN = false;

int copy[1002][1002];

QImage *image;
QPoint beginpoint;
QPoint endpoint;
QPoint ptmp;

int set_point(complex &point, const int ITR) {
    complex z = complex(0.0, 0.0);
    int i = 0;
    for (i = 0; i < ITR; ++i) {
        z = z * z + point;
        if (z.radius() > 2.0 + EPS) {
            break;
        }
    }
    return i;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *main_win = new QWidget(this);
    this->setCentralWidget(main_win);

    this->setMinimumSize(2 * SZ + 1, 2 * SZ + 1);
    this->setMaximumSize(2 * SZ + 1, 2 * SZ + 1);

    image = new QImage(2 * SZ + 1, 2 * SZ + 1, QImage::Format_RGB32);
}

MainWindow::~MainWindow() {

}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    PRESSED_BTN = true;
    endpoint = beginpoint = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!PRESSED_BTN) {
        return;
    }

    ptmp = event->pos();
    if (ptmp.x() < 0 || ptmp.y() < 0 || ptmp.x() > 2 * SZ || ptmp.y() > 2 * SZ) {
        return;
    }
    ptmp = endpoint;
    endpoint = event->pos();
    repaint();

    return;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    PRESSED_BTN = false;

    if (!(ptmp.x() < 0 || ptmp.y() < 0 || ptmp.x() > 2 * SZ || ptmp.y() > 2 * SZ)) {
        endpoint = event->pos();
    }

    int delx = endpoint.x() - beginpoint.x(), dely;
    if (delx > 0) {
        dely = abs(endpoint.y() - beginpoint.y());
        if (dely != 0) {
            BGN_X = double(std::min(endpoint.x(), beginpoint.x())) * STEP + BGN_X;
            BGN_Y = double(2.0 * SZ - std::max(endpoint.y(), beginpoint.y()))*STEP + BGN_Y;
            STEP = double(std::max(delx, dely)) / (2.0 * double(SZ)) * STEP;
            repaint();
        }
    } else if (delx < 0) {
        dely = abs(endpoint.y() - beginpoint.y());
        if (dely != 0) {
            STEP = (2.0 * double(SZ)) / double(std::max(delx, dely)) * STEP;
            BGN_X -= double(std::min(endpoint.x(), beginpoint.x())) * STEP;
            BGN_Y -= double(2.0 * SZ - std::max(endpoint.y(), beginpoint.y())) * STEP;
            repaint();
        }
    }
}


void MainWindow::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.translate(SZ, SZ);
    painter.scale(1, -1);
    if (PRESSED_BTN) {
        int px1 = std::min(beginpoint.x(),endpoint.x()) - SZ,
            px2 = std::max(beginpoint.x(),endpoint.x()) - SZ,
            py1 = SZ - std::max(beginpoint.y(),endpoint.y()),
            py2 = SZ - std::min(beginpoint.y(), endpoint.y());

        auto tmp = *image;
        for (int i = px1; i <= px2; ++i) {
            tmp.setPixelColor(i + SZ, SZ + py1, Qt::red);
            tmp.setPixelColor(i + SZ, SZ + py2, Qt::red);
        }
        for (int i = py1; i <= py2; ++i) {
            tmp.setPixelColor(px1+SZ, SZ + i, Qt::red);
            tmp.setPixelColor(px2+SZ, SZ + i, Qt::red);
        }
        painter.drawImage(QPoint(-SZ, -SZ), tmp);
        return;
    }

    for (int i = -SZ; i <= SZ; ++i) {
        for (int j = -SZ; j <= SZ; ++j) {
            complex mypoint = complex((i + SZ)*STEP + BGN_X, (SZ - j) * STEP + BGN_Y);
            if (set_point(mypoint, ITR) == ITR) {
                image->setPixelColor(i + SZ, SZ - j, Qt::black);
                copy[i + SZ][j + SZ] = 1;
            } else {
                image->setPixelColor(i + SZ, SZ - j, Qt::white);
                copy[i + SZ][j + SZ] = 0;
            }

        }
    }
    painter.drawImage(QPoint(-SZ, -SZ), *image);
    return;
}
