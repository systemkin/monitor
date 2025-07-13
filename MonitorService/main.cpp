#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <server.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server server;
    server.listen(QHostAddress::Any, 12345);
    return a.exec();
}

