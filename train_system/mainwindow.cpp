#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "unistd.h"
#include "semaphore.h"
#include "pthread.h"

int train_yellow_x = 90;
int train_yellow_y = 90;
int train_blue_x = 410;
int train_blue_y = 90;
int train_green_x = 410;
int train_green_y = 410;
int train_red_x = 90;
int train_red_y = 410;
int speed_yellow = 1;
int speed_blue = 1;
int speed_red = 1;
int speed_green = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startButton, SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked(bool)),this,SLOT(stop()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(speed()));
    timer->setInterval(1);
    timer->start();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    /*trilho 1: amarelho*/
    QPen pen1(Qt::yellow);
    pen1.setWidth(5);
    painter.setPen(pen1);
    painter.drawRect(QRect(100,100,150,150));

    /*trilho 2: azul*/
    QPen pen2(Qt::blue);
    pen2.setWidth(5);
    painter.setPen(pen2);
    painter.drawRect(QRect(270,100,150,150));

    /*trilho 3: verde*/
    QPen pen3(Qt::green);
    pen3.setWidth(5);
    painter.setPen(pen3);
    painter.drawRect(QRect(270,270,150,150));

    /*trilho 4: vermelho*/
    QPen pen4(Qt::red);
    pen4.setWidth(5);
    painter.setPen(pen4);
    painter.drawRect(QRect(100,270,150,150));


    /*trem 1: amarelo*/
    QBrush brush1(Qt::yellow);
    brush1.setStyle(Qt::SolidPattern);
    painter.setBrush(brush1);
    painter.setPen(pen1);
    painter.drawRect(QRect(train_yellow_x,train_yellow_y,20,20));

    /*trem 2: azul*/
    QBrush brush2(Qt::blue);
    brush2.setStyle(Qt::SolidPattern);
    painter.setBrush(brush2);
    painter.setPen(pen2);
    painter.drawRect(QRect(train_blue_x,train_blue_y,20,20));

    /*trem 3: verde*/
    QBrush brush3(Qt::green);
    brush3.setStyle(Qt::SolidPattern);
    painter.setBrush(brush3);
    painter.setPen(pen3);
    painter.drawRect(QRect(train_green_x,train_green_y,20,20));

    /*trem 4: vermelho*/
    QBrush brush4(Qt::red);
    brush4.setStyle(Qt::SolidPattern);
    painter.setBrush(brush4);
    painter.setPen(pen4);
    painter.drawRect(QRect(train_red_x,train_red_y,20,20));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void *train_yellow(void *arg);
void *train_blue(void *arg);
void *train_green(void *arg);
void *train_red(void *arg);
void *speed(void *arg);

sem_t sem_L3;
sem_t sem_L4;
sem_t sem_L7;
sem_t sem_L9;

void MainWindow::start()
{
    sem_init(&sem_L3, 0, 1);
    sem_init(&sem_L4, 0, 1);
    sem_init(&sem_L7, 0, 1);
    sem_init(&sem_L9, 0, 1);

    pthread_t train1, train2, train3, train4, control_speed;
    pthread_create(&train1, NULL, train_yellow, (void*)this);
    pthread_create(&train2, NULL, train_blue, (void*)this);
    pthread_create(&train3, NULL, train_green, (void*)this);
    pthread_create(&train4, NULL, train_red, (void*)this);    

    sem_destroy(&sem_L3);
    sem_destroy(&sem_L4);
    sem_destroy(&sem_L7);
    sem_destroy(&sem_L9);
}

void MainWindow::speed()
{
    speed_yellow = ui -> sliderYellow -> value();

    speed_blue = ui -> sliderBlue -> value();

    speed_green = ui -> sliderGreen -> value();

    speed_red = ui -> sliderRed -> value();

    update();
}


void MainWindow::stop()
{
    exit(0);
}


void *train_yellow(void *args){
    while(1){
        /* L2 */
        if(train_yellow_x < 240 && train_yellow_y == 90){
            train_yellow_x += 1;
            if(train_yellow_x == 240){
                sem_wait(&sem_L4);
                sem_wait(&sem_L3);
            }
        }
        /* L3 */
        if(train_yellow_x == 240 && train_yellow_y < 240){
            train_yellow_y += 1;
            if(train_yellow_y == 240){
                sem_post(&sem_L3);
            }
        }
        /* L4 */
        if(train_yellow_x > 90 && train_yellow_x <= 240 && train_yellow_y == 240){
            train_yellow_x -= 1;
            if(train_yellow_x == 90){
                sem_post(&sem_L4);
            }
        }
        /* L1 */
        if(train_yellow_x == 90 && train_yellow_y <= 240 && train_yellow_y > 90){
            train_yellow_y -=1;
        }

      usleep(80000/(speed_yellow));
    }
}

void *train_blue(void *args){
    while(1){
        /* L6 */
        if(train_blue_x == 410 && train_blue_y < 240){
            train_blue_y += 1;
            if(train_blue_y == 240){
                sem_wait(&sem_L3);
                sem_wait(&sem_L7);
            }
        }
        /* L7 */
        if(train_blue_x <= 410 && train_blue_x > 260 && train_blue_y == 240){
            train_blue_x -= 1;
            if(train_blue_x == 260){
                sem_post(&sem_L7);
            }
        }
        /* L3 */
        if(train_blue_x == 260 && train_blue_y > 90 && train_blue_y <= 240){
            train_blue_y -= 1;
            if(train_blue_y == 90){
                sem_post(&sem_L3);
            }
        }
        /* L5 */
        if(train_blue_x < 410  && train_blue_y == 90){
            train_blue_x +=1;
        }
      usleep(80000/(speed_blue));
    }

}

void *train_green(void *args){
    while(1){
        /* L11 */
        if(train_green_x == 410 && train_green_y < 410){
            train_green_y += 1;
        }
        /* L12 */
        if(train_green_x <= 410 && train_green_x > 260 && train_green_y == 410){
            train_green_x -= 1;
            if(train_green_x == 260){
                sem_wait(&sem_L7);
                sem_wait(&sem_L9);
            }
        }
        /* L9 */
        if(train_green_x == 260 && train_green_y > 260 && train_green_y <= 410){
            train_green_y -= 1;
            if(train_green_y == 260){
                sem_post(&sem_L9);
            }
        }
        /* L7 */
        if(train_green_x < 410  && train_green_y == 260){
            train_green_x +=1;
            if(train_green_x == 410){
                sem_post(&sem_L7);
            }
        }
       usleep(80000/(speed_green));
    }
}

void *train_red(void *args){
    while(1){
        /* L8 */
        if(train_red_x == 90 && train_red_y <= 410 && train_red_y > 260){
            train_red_y -= 1;
            if(train_red_y == 260 ){
                sem_wait(&sem_L9);
                sem_wait(&sem_L4);
            }
        }
        /* L4 */
        if(train_red_x < 240 && train_red_y == 260){
            train_red_x += 1;
            if(train_red_x == 240){
                sem_post(&sem_L4);
            }
        }
        /* L9 */
        if(train_red_x == 240 && train_red_y < 410){
            train_red_y += 1;
            if(train_red_y == 410){
                sem_post(&sem_L9);
            }
        }
        /* L10 */
        if(train_red_x > 90 && train_red_x <= 240 && train_red_y == 410){
            train_red_x -=1;
        }
        usleep(80000/(speed_red));
    }
}
