#include "server.h"
#include "digitalosc.h"
#include <QtNetwork>
#include <QtWidgets>
#include <qlocalserver.h>
#include <qlocalsocket.h>
#include <stdlib.h>

Server::Server(QWidget* parent)
    : QDialog(parent)
    , osc(new DigitalOsc(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Создаём и запускаем сервер командой listen.
    // Если сервер не может быть запущен, выдать сообщение об ошибке и завершить работу программы
    server = new QLocalServer(this);
    if (!server->listen("oscadsh")) {
        QMessageBox::critical(0, "Server error",
            "Unable to start server:" + server->errorString());
        server->close();
        return;
    }

    // Соединяем сигнал сервера о наличии нового подключения с обработчиком нового клиентского подключения
    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    // Формируем окно для просмотра текстовых сообщений от клиента
    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel(serverName));
    layout->addWidget(textEdit);
    setLayout(layout);

    //    server = new QLocalServer(this);
    //    if (!server->listen("oscadsh")) {
    //        QMessageBox::critical(this, tr("Local Fortune Server"), tr("Unable to start the server: %1.").arg(server->errorString()));
    //        close();
    //        return;
    //    }

    //    osc->ping();

    //    QLabel* statusLabel = new QLabel;
    //    statusLabel->setWordWrap(true);
    //    statusLabel->setText(osc->idn());

    //    QPushButton* quitButton = new QPushButton(tr("Quit"));
    //    quitButton->setAutoDefault(false);
    //    connect(quitButton, &QPushButton::clicked, this, &Server::close);
    //    connect(server, &QLocalServer::newConnection, this, &Server::sendFortune);

    //    QHBoxLayout* buttonLayout = new QHBoxLayout;
    //    buttonLayout->addStretch(1);
    //    buttonLayout->addWidget(quitButton);
    //    buttonLayout->addStretch(1);

    //    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    //    mainLayout->addWidget(statusLabel);
    //    mainLayout->addLayout(buttonLayout);

    //    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sendFortune()
{
    qDebug(Q_FUNC_INFO);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //    out.setVersion(QDataStream::Qt_5_10);

    QString str = QString::number(osc->AVERage(2)) + QString::number(qrand());
    out << quint32(str.size());
    out << str;

    QLocalSocket* clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QLocalSocket::disconnected, clientConnection, &QLocalSocket::deleteLater);
    qDebug() << clientConnection->readAll();
    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
}

void Server::sendToClient(QLocalSocket* localSocket, const QString& string)
{
    // Поскольку заранее размер блока неизвестен (параметр string может быть любой длины),
    // вначале создаём объект array класса QByteArray
    QByteArray array;
    // На его основе создаём выходной поток
    QDataStream out(&array, QIODevice::WriteOnly);
    // Устанавливаем версию сериализации данных потока
    out.setVersion(QDataStream::Qt_5_3);
    // Записываем в поток данные для отправки. На первом месте идёт нулевой размер блока
    out << quint16(0) << QTime::currentTime() << string;

    // Перемещаем указатель на начало блока
    out.device()->seek(0);
    // Записываем двухбайтное значение действительного размера блока без учёта пересылаемого размера блока
    out << quint16(array.size() - sizeof(quint16));

    // Отправляем получившийся блок клиенту
    localSocket->write(array);
}

void Server::slotNewConnection()
{
    // Получаем сокет, подключённый к серверу
    QLocalSocket* localSocket = server->nextPendingConnection();
    // Соединяем сигнал отключения сокета с обработчиком удаления сокета
    connect(localSocket, SIGNAL(disconnected()), localSocket, SLOT(deleteLater()));
    // Соединяем сигнал сокета о готовности передачи данных с обработчиком данных
    connect(localSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    // Отправляем информацию клиенту о соединении с сервером
    sendToClient(localSocket, "Server response: Connected!");
}

void Server::slotReadClient()
{
    // Получаем QLocalSocket после срабатывания сигнала о готовности передачи данных
    QLocalSocket* localSocket = (QLocalSocket*)sender();
    // Создаём входной поток получения данных на основе сокета
    QDataStream in(localSocket);
    // Устанавливаем версию сериализации данных потока. У клиента и сервера они должны быть одинаковыми
    in.setVersion(QDataStream::Qt_5_3);
    // Бесконечный цикл нужен для приёма блоков данных разных размеров, от двух байт и выше
    for (;;) {
        // Если размер блока равен нулю
        if (!nextBlockSize) {
            // Если размер передаваемого блока меньше двух байт, выйти из цикла
            if (localSocket->bytesAvailable() < (int)sizeof(quint16))
                break;
            // Извлекаем из потока размер блока данных
            in >> nextBlockSize;
        }

        // Извлекаем из потока время и строку
        QTime time;
        QString string;
        in >> time >> string;

        // Преобразуем полученные данные и показываем их в виджете
        QString message = time.toString() + " " + "Client has sent - " + string;
        textEdit->append(message);

        nextBlockSize = 0;

        // Отправляем ответ клиенту
        sendToClient(localSocket, "Server response: received \"" + string + "\"");
    }
}
