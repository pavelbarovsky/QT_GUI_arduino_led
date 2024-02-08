#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(QObject *parent = nullptr);

protected:
    void run() override;

signals:
    void mySignal(); // Пример сигнала, который вы можете использовать для взаимодействия с главным потоком

public slots:
    void mySlot(); // Пример слота, который вы можете использовать для взаимодействия с главным потоком
};

#endif // MYTHREAD_H
