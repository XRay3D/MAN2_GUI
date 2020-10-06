#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLocalServer;
class QTextEdit;
class QLocalSocket;
QT_END_NAMESPACE
class DigitalOsc;

class Server : public QDialog {
    Q_OBJECT

public:
    explicit Server(QWidget* parent = nullptr);

private slots:
    void sendFortune();

private:
    DigitalOsc* osc;

    // Указатель на QLocalServer
    QLocalServer* server;

    // Указатель на QTextEdit, в котором будет показываться поступающая от клиента информация
    QTextEdit* textEdit;

    // Переменная для хранения размера получаемого от клиента блока
    quint16 nextBlockSize;

    // Метод для отправки клиенту подтверждения о приёме информации
    void sendToClient(QLocalSocket* localSocket, const QString& string);

public slots:
    // Слот обработки нового клиентского подключения
    void slotNewConnection();

    // Слот чтения информации от клиента
    void slotReadClient();
};
