#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
//#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>


// ЭТО КОНСТУРУКТОР
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

// ЭТО ДЕСТРУКТОР
MainWindow::~MainWindow()
{
    delete ui;
}

// Объявляем глобальные переменные
QSerialPort* serial;  // Указатель на объект QSerialPort
QTimer* timer = nullptr;  // Указатель на объект QTimer, инициализируем его значением nullptr
int blinkCount = 0;  // Счетчик миганий светодиода

void MainWindow::on_pushButton_clicked()
{
    // Если таймер уже работает и не все мигания выполнены, ничего не делаем
    if (timer && timer->isActive() && blinkCount < 10) {
        return;
    }

    // Если таймер не был запущен или все мигания выполнены, создаем новый
    // Если таймер работал, но все мигания выполнены, сбрасываем счетчик blinkCount
    if (blinkCount >= 4) {
        blinkCount = 0;
    }

    // Создаем объект QSerialPort и настраиваем его параметры
    serial = new QSerialPort(this);
    serial->setPortName("COM8");  // Укажите правильный порт COM для вашей Arduino
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Открываем порт
    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Ошибка при открытии порта: " << serial->errorString();
        delete serial;  // Освобождаем ресурсы
        return;
    }

    // Создаем таймер и настраиваем его параметры
    timer = new QTimer(this);
    timer->setInterval(1000);  // Интервал между миганиями в миллисекундах

    // Подключаем сигнал таймера к слоту для мигания светодиода
    connect(timer, &QTimer::timeout, this, [this]()
            {
                // Отправляем команду мигания светодиода на Arduino
                QByteArray data;
                data.append("B");  // Команда для мигания светодиода
                serial->write(data);

                // Увеличиваем счетчик миганий
                blinkCount++;

                // Если достигнуто нужное количество миганий
                if (blinkCount >= 4) {
                    // Останавливаем таймер
                    timer->stop();
                    // Закрываем порт
                    serial->close();
                    // Освобождаем ресурсы
                    delete serial;
                    delete timer;
                    timer = nullptr;  // Устанавливаем указатель на таймер в значение nullptr
                }
            });

    // Запускаем таймер
    timer->start();
}

