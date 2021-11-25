#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <iostream>

class Worker : public QObject {
    Q_OBJECT    //this is a macro
signals:
    void rpmEmitter(float rpm);

public:
    Q_SLOT void foo() {
        std::cout << "foo";
//        qDebug() << "Foo called\n" << QThread::currentThread();
    }


    Worker(QObject * parent = nullptr) : QObject{parent} {}   //constructor I belive

};

#endif // WORKER_H
