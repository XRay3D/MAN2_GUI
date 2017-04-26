#include "automaticmeasurements.h"
#include "inputparameters.h"
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QSettings>
#include <QDate>
#include <QDir>
#include <measuringinterface/measuringinterface.h>

const ScanSettings AutomaticMeasurements::m_scanSettings;

AutomaticMeasurements::AutomaticMeasurements(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    for (int i = 0; i < listWidget->count(); ++i) {
        m_doNotSkip[i] = true;
        listWidget->item(i)->setText("");
        listWidget->item(i)->setHidden(true);
    }

    QFont font;
    font.setPixelSize(20);
    listWidget->setFont(font);
    font.setPixelSize(14);
    widget->setFont(font);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &AutomaticMeasurements::ItemDoubleClicked);
    m_paths << ""
            << ""
            << ""
            << ""
            << ""
            << ""
            << ""
            << "";
    m_serNum << ""
             << ""
             << ""
             << ""
             << ""
             << ""
             << ""
             << "";
    m_listCheckBox << checkBox_1 << checkBox_2 << checkBox_3 << checkBox_4 << checkBox_5 << checkBox_6 << checkBox_7;
    m_listDsbVoltage << dsbVoltage_1 << dsbVoltage_2 << dsbVoltage_3 << dsbVoltage_4 << dsbVoltage_5 << dsbVoltage_6 << dsbVoltage_7 << dsbVoltage_8;
    m_listDsbCurrent << dsbCurrent_1 << dsbCurrent_2 << dsbCurrent_3 << dsbCurrent_4 << dsbCurrent_5 << dsbCurrent_6 << dsbCurrent_7 << dsbCurrent_8;
}

AutomaticMeasurements::~AutomaticMeasurements()
{
    qDebug() << "~AutomaticMeasurements()";
}

void AutomaticMeasurements::ScanSettingsSlot(const ScanSettings* scanSettings)
{
    *const_cast<ScanSettings*>(&m_scanSettings) = *scanSettings;
}

void AutomaticMeasurements::SerialNumberChanged(const QString& serialNumber, int index)
{
    qDebug() << "SerialNumberChanged" << serialNumber << index;
    listWidget->item(index)->setText(serialNumber);

    if (serialNumber.isEmpty()) {
        listWidget->item(index)->setHidden(true);
    }
    else {
        m_serNum[index] = serialNumber;
        listWidget->item(index)->setHidden(false);
    }

    for (int i = 0; i < 8; ++i) {
        m_doNotSkip[i] = true;
    }
    for (int i = 0; i < listWidget->count(); ++i) {
        if (listWidget->item(i)->isHidden()) {
            for (int j = 0; j < m_scanSettings.NumberOfChannels; ++j) {
                m_doNotSkip[i * int(m_scanSettings.NumberOfChannels) + j] = false;
            }
        }
    }
}

void AutomaticMeasurements::ShowMessage(int num)
{
    switch (num) {
    case 0:
        if (QMessageBox::information(this, "", QString("Установите входное напряжение %1±%2В")
                                                   .arg(m_scanSettings.Voltageerrortest5U1)
                                                   .arg(m_scanSettings.Voltageerrortest5U2)
                                                   .replace('.', ','),
                "Ок", "Остановить измерения")) {
            m_worker->FinishMeasurements();
        }
        else {
            m_worker->Continue();
        }
        break;
    case 1:
        if (QMessageBox::information(this, "", QString("Установите входное напряжение %1±%2В")
                                                   .arg(m_scanSettings.Voltageerrortest3_4U1)
                                                   .arg(m_scanSettings.Voltageerrortest3_4U2)
                                                   .replace('.', ','),
                "Ок", "Остановить измерения")) {
            m_worker->FinishMeasurements();
        }
        else {
            m_worker->Continue();
        }
        break;
    case 2:
        if (QMessageBox::information(this, "", "Установите входное напряжение 220±4,4В", "Ок", "Остановить измерения")) {
            m_worker->FinishMeasurements();
        }
        else {
            m_worker->Continue();
        }
        break;
    case 3:
        QMessageBox::critical(this, "", "Нет связи с МАНом!");
        on_pbStartStop_clicked(false);
        break;
    case 4:
        if (QMessageBox::information(this, "", "Восстановите работу каналов блока питания", "Ок", "Остановить измерения")) {
            m_worker->FinishMeasurements();
        }
        else {
            m_worker->Continue();
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        switch (QMessageBox::question(this, "", QString("Пульсации в канале %2 соответствуют норме <%1мВ ?")
                                                    .arg(m_scanSettings.VisualControl)
                                                    .arg(num - 10)
                                                    .replace('.', ','),
            "Да", "Нет", "Остановить измерения")) {
        case 0:
            m_worker->Continue();
            m_result[num - 11].test2 = 1.0;
            break;
        case 1:
            m_result[num - 11].test2 = -1.0;
            m_worker->Continue();
            break;
        case 2:
            m_worker->FinishMeasurements();
            break;
        default:
            break;
        }
        break;
    case 20:
        QMessageBox::information(this, "", "Проверка закончена.");
        //        for (int i = 0; i < 8; ++i) {
        //            if (m_doNotSkip[i]) {
        //                qDebug() << i << "test1" << m_result[i].test1;
        //                qDebug() << i << "test2" << m_result[i].test2;
        //                qDebug() << i << "test3" << m_result[i].test3;
        //                qDebug() << i << "test4" << m_result[i].test4;
        //                qDebug() << i << "test5" << m_result[i].test5;
        //                qDebug() << i << "test6" << m_result[i].test6;
        //                qDebug() << i << "test7" << m_result[i].test7;
        //            }
        //        }
        for (int i = 0; i < 8; ++i) {
            if (!listWidget->item(i)->isHidden()) {
                SaveProtokol(listWidget->item(i)->text(), i);
            }
            if (!m_paths[i].isEmpty()) {
                ShowProtocol(i);
            }
        }
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        m_listCheckBox[num - 21]->setChecked(true);
        break;
    default:
        break;
    }
}

void AutomaticMeasurements::ShowProtocol(int num)
{
    MyDialog* Dialog = new MyDialog(this, m_serNum[num]);
    Dialog->LoadFile(m_paths[num]);
    //    Dialog->exec();
    //    Dialog->deleteLater();
}

void AutomaticMeasurements::ItemDoubleClicked(QListWidgetItem* item)
{
    ShowProtocol(listWidget->row(item));
}

void AutomaticMeasurements::UpdateProgresBar()
{
    if (progressBar->value() == progressBar->maximum()) {
        progressBar->setValue(0);
    }
    progressBar->setValue(progressBar->value() + 1);
}

void AutomaticMeasurements::SaveProtokol(const QString& serialNumber, int number)
{
    /* //    qDebug() << "PdfWriter";
    //    qDebug() << "Cipher" << m_scanSettings.Cipher;
    //    qDebug() << "Type" << m_scanSettings.Type;
    //    qDebug() << "RatedVoltage" << m_scanSettings.RatedVoltage;
    //    qDebug() << "NumberOfChannels" << m_scanSettings.NumberOfChannels;
    //    qDebug() << "RatedCurrent" << m_scanSettings.RatedCurrent;
    //    qDebug() << "RestrictionTest2" << m_scanSettings.RestrictionTest2;
    //    qDebug() << "VisualControl" << m_scanSettings.VisualControl;
    //    qDebug() << "LimitationsTest4_5" << m_scanSettings.LimitationsTest4_5;
    //    qDebug() << "LimitTest6" << m_scanSettings.LimitTest6;
    //    qDebug() << "RestrictionsTest7Min" << m_scanSettings.RestrictionsTest7Min;
    //    qDebug() << "RestrictionsTest7Max" << m_scanSettings.RestrictionsTest7Max;
    //    qDebug() << "Voltageerrortest3_4U1" << m_scanSettings.Voltageerrortest3_4U1;
    //    qDebug() << "Voltageerrortest3_4U2" << m_scanSettings.Voltageerrortest3_4U2;
    //    qDebug() << "Voltageerrortest5U1" << m_scanSettings.Voltageerrortest5U1;
    //    qDebug() << "Voltageerrortest5U2" << m_scanSettings.Voltageerrortest5U2;
    //    qDebug() << "VoltageErrorTest7" << m_scanSettings.VoltageErrorTest7;
    //    qDebug() << "ParameterDLTest7" << m_scanSettings.ParameterDLTest7;

    const int rowCount = (int)m_scanSettings.NumberOfChannels;

    for (int sn = 0; sn < serialNumber.size(); ++sn) {
    }
    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(m_scanSettings.Type)
                       .append(QDate::currentDate().toString("/yyyy/"))
                       .append(QDate::currentDate().toString("MM"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(this, "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path
               .append("/")
               .append(serialNumber)
               .append(QDate::currentDate().toString("_dd.MM.yyyy"))
               .append("г.pdf");

    m_paths[number] = path;
    QPdfWriter PdfWriter(path);
    PdfWriter.setResolution(100);
    PdfWriter.setPageLayout(QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(0, 0, 0, 0)));

    QPainter painter(&PdfWriter);
    painter.translate(QPoint(80, 40));
    painter.setPen(QPen(QBrush(), 1.25));

    QList<QString> headerNameList;
    headerNameList
        << "№\n"
           "канала"
        << QString("Выходное\n"
                   "напряжени\n"
                   "от %1\n"
                   "до %2\n"
                   "В")
               .arg(m_scanSettings.RatedVoltage - m_scanSettings.RestrictionTest2)
               .arg(m_scanSettings.RatedVoltage + m_scanSettings.RestrictionTest2)
               .replace('.', ',')
        << QString("Пульсация\n"
                   "выходного\n"
                   "напряжения\n"
                   "не более\n"
                   "%1 мВ")
               .arg(m_scanSettings.VisualControl)
               .replace('.', ',')
        << QString("Нестабильность\n"
                   "выходного\n"
                   "напряжения при\n"
                   "изменении напряжения\n"
                   "сети,не более выходного\n"
                   "напряжения\n"
                   "± %1 В")
               .arg(m_scanSettings.LimitationsTest4_5)
               .replace('.', ',')
        << QString("Нестабильность выходного\n"
                   "напряжения при изменении\n"
                   "тока нагрузки от нуля\n"
                   "до номинального не более\n"
                   "вых. напряжения\n"
                   "± %1 В")
               .arg(m_scanSettings.LimitationsTest4_5)
               .replace('.', ',')
        << QString("Ток\n"
                   "срабатывания\n"
                   "электронной\n"
                   "защиты\n"
                   "от %1\n"
                   "до %2 мА")
               .arg(m_scanSettings.RestrictionsTest7Min)
               .arg(m_scanSettings.RestrictionsTest7Max)
               .replace('.', ',')
        << "Срабатывание\n"
           "электронной\n"
           "защиты\n"
           "при коротком\n"
           "замыкании";

    QString capSrting = QString("Протокол приемо-сдаточных испытаний источника питания постоянного тока\n"
                                "%1\n"
                                "Заводской номер № %2\n")
                            .arg(m_scanSettings.Cipher)
                            .arg(serialNumber);

    QVector<int> columnWidth;
    QVector<int> rowHeight;
    QVector<int> columnOffset(1, 0);
    QVector<int> rowOffset(1, 0);
    QRect rect;

    // заполняем ширину столбцов
    painter.setFont(QFont("Times New Roman", 14));
    for (int i = 0, width = 0; i < headerNameList.size(); ++i) {
        width = painter.boundingRect(0, 0, 10, 10, Qt::AlignCenter, headerNameList[i]).width() + 20;
        if (i == 3) {
            columnWidth.append(width / 2);
            columnWidth.append(width / 2);
        }
        else {
            columnWidth.append(width);
        }
    }
    // заполняем высоту строк
    painter.setFont(QFont("Times New Roman", 16));
    rowHeight << painter.boundingRect(QRect(), Qt::AlignCenter, capSrting).height() + 10;
    painter.setFont(QFont("Times New Roman", 14));
    rowHeight << painter.boundingRect(QRect(), Qt::AlignCenter, headerNameList[3]).height() + 10;
    for (int row = 0; row <= rowCount; ++row) {
        rowHeight << painter.boundingRect(QRect(), Qt::AlignCenter, "0,000").height() + 10;
    }
    rowHeight << rowHeight.last() * 2;

    // заполняем смещения от 0 до N
    for (int i = 0, c = 0; i < columnWidth.size(); ++i) {
        c += columnWidth[i];
        columnOffset.append(c);
    }
    for (int i = 0, c = 0; i < rowHeight.size(); ++i) {
        c += rowHeight[i];
        rowOffset.append(c);
    }

    // пишем шапку
    painter.setFont(QFont("Times New Roman", 16));
    painter.drawText(columnOffset[0], rowOffset[0], columnOffset.last(), rowHeight[0], Qt::AlignTop | Qt::AlignHCenter, capSrting);

    // заполняем заголовок таблицы
    painter.setFont(QFont("Times New Roman", 14));
    for (int i = 0, col = 0; i < headerNameList.size(); ++i, ++col) {
        if (i == 3) {
            rect = QRect(columnOffset[col], rowOffset[1], columnWidth[col] + columnWidth[col + 1], rowHeight[1]);
            ++col;
        }
        else
            rect = QRect(columnOffset[col], rowOffset[1], columnWidth[col], rowHeight[1] + rowHeight[2]);
        painter.drawText(rect, Qt::AlignCenter, headerNameList[i]);
    }
    // заполняем данные
    double val;
    QString str;
    char num[100];
    rect = QRect(columnOffset[3], rowOffset[2], columnWidth[3], rowHeight[2]);
    val = m_scanSettings.Voltageerrortest3_4U1;
    sprintf(num, "%.2f", val);
    str = QString(num).replace('.', ',');
    painter.drawText(rect, Qt::AlignCenter, str);
    rect = QRect(columnOffset[4], rowOffset[2], columnWidth[4], rowHeight[2]);
    val = m_scanSettings.Voltageerrortest5U1;
    sprintf(num, "%.2f", val);
    str = QString(num);
    painter.drawText(rect, Qt::AlignCenter, str);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnWidth.size(); ++col) {
            rect = QRect(columnOffset[col], rowOffset[row + 3], columnWidth[col], rowHeight[row + 3]);
            switch (col) {
            case 0:
                str = QString().setNum(row + 1);
                break;
            case 1:
                val = m_result[row + number * rowCount].test1;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(m_scanSettings.RatedVoltage - val) > m_scanSettings.RestrictionTest2) {
                    painter.setBrush(QBrush(Qt::red));
                }
                break;
            case 2:
                val = m_result[row + number * rowCount].test2;
                if (val > 0) {
                    str = "в норме";
                }
                else {
                    painter.setBrush(QBrush(Qt::red));
                    str = "отказ";
                }
                break;
            case 3:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test3;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    painter.setBrush(QBrush(Qt::red));
                }
                break;
            case 4:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test4;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    painter.setBrush(QBrush(Qt::red));
                }
                break;
            case 5:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test5;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    painter.setBrush(QBrush(Qt::red));
                }
                break;
            case 6:
                val = m_result[row + number * rowCount].test6;
                sprintf(num, "%.1f", val);
                str = QString(num).replace('.', ',');
                if ((m_scanSettings.RestrictionsTest7Min) > val || val > (m_scanSettings.RestrictionsTest7Max)) {
                    painter.setBrush(QBrush(Qt::red));
                }
                if (0.0 > val) {
                    painter.setBrush(QBrush(Qt::red));
                    //str = "отказ";
                }
                break;
            case 7:
                val = m_result[row + number * rowCount].test7;
                sprintf(num, "%.3f", val);
                str = QString(num).replace('.', ',');
                if (val > m_scanSettings.VoltageErrorTest7) {
                    painter.setBrush(QBrush(Qt::red));
                    //str = "отказ";
                }
                break;
            default:
                break;
            }
            painter.drawText(rect, Qt::AlignCenter, str);
            painter.setBrush(QBrush(Qt::black));
        }
    }
    // заполняем "корень"
    rect = QRect(columnOffset[0], rowOffset[rowOffset.size() - 2], columnOffset[5], rowHeight.last());
    painter.drawText(rect, Qt::AlignBottom | Qt::AlignRight, QDate::currentDate().toString("dd.MM.yyyy г.") + " Представитель ОТК");
    rect = QRect(columnOffset[6], rowOffset[rowOffset.size() - 2], columnOffset.last() - columnOffset[6], rowHeight.last());
    painter.drawText(rect, Qt::AlignBottom | Qt::AlignLeft, m_scanSettings.Fio);

    // рисуем границы строк
    for (int row = 1; row < rowOffset.size() - 1; ++row) {
        if (row == 2)
            painter.drawLine(columnOffset[3], rowOffset[row], columnOffset[5], rowOffset[row]);
        else
            painter.drawLine(columnOffset.first(), rowOffset[row], columnOffset.last(), rowOffset[row]);
    }
    painter.drawLine(columnOffset[5], rowOffset.last(), columnOffset[6], rowOffset.last());

    // рисуем границы солбцов
    for (int col = 0; col < columnOffset.size(); ++col) {
        if (col == 4)
            painter.drawLine(columnOffset[col], rowOffset[2], columnOffset[col], rowOffset[rowOffset.size() - 2]);
        else
            painter.drawLine(columnOffset[col], rowOffset[1], columnOffset[col], rowOffset[rowOffset.size() - 2]);
    }

    painter.end();*/

    //    QFile file1(":/protocol/Шапка.htm");
    //    QFile file2(":/protocol/Строка.htm");
    //    QFile file3(":/protocol/Подвал.htm");
    QFile file1(qApp->applicationDirPath().append("/").append("protocol/Шапка.htm"));
    QFile file2(qApp->applicationDirPath().append("/").append("protocol/Строка.htm"));
    QFile file3(qApp->applicationDirPath().append("/").append("protocol/Подвал.htm"));

    qDebug() << file1.open(QFile::ReadOnly);
    qDebug() << file2.open(QFile::ReadOnly);
    qDebug() << file3.open(QFile::ReadOnly);

    QString protocol;
    QString cap = QString().fromLocal8Bit(file1.readAll());
    QString rowStr = QString().fromLocal8Bit(file2.readAll());
    QString basement = QString().fromLocal8Bit(file3.readAll());

    protocol.append(cap
                        .arg(m_scanSettings.Cipher)
                        .arg(serialNumber)
                        .arg(QString().setNum(m_scanSettings.RatedVoltage - m_scanSettings.RestrictionTest2).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.RatedVoltage + m_scanSettings.RestrictionTest2).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.VisualControl).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.LimitationsTest4_5).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.LimitationsTest4_5).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.RestrictionsTest7Min).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.RestrictionsTest7Max).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.Voltageerrortest3_4U1).replace('.', ','))
                        .arg(QString().setNum(m_scanSettings.Voltageerrortest5U1).replace('.', ',')));

    const int rowCount = m_scanSettings.NumberOfChannels;

    for (int row = 0; row < rowCount; ++row) {
        bool flags[7];
        flags[0] = false;
        flags[1] = false;
        flags[2] = false;
        flags[3] = false;
        flags[4] = false;
        flags[5] = false;
        flags[6] = false;
        QString str;
        QStringList list;
        list.clear();
        double val;
        char num[100];
        for (int col = 0; col < 8; ++col) {
            switch (col) {
            case 0:
                str = QString().setNum(row + 1);
                break;
            case 1:
                val = m_result[row + number * rowCount].test1;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(m_scanSettings.RatedVoltage - val) > m_scanSettings.RestrictionTest2) {
                    flags[col - 1] = true;
                }
                break;
            case 2:
                val = m_result[row + number * rowCount].test2;
                if (val > 0) {
                    str = "в норме";
                }
                else {
                    flags[col - 1] = true;
                    str = "отказ";
                }
                break;
            case 3:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test3;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 4:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test4;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 5:
                val = m_result[row + number * rowCount].test1 - m_result[row + number * rowCount].test5;
                sprintf(num, "%.4f", val);
                str = QString(num).replace('.', ',');
                if (qAbs(val) > m_scanSettings.LimitationsTest4_5) {
                    flags[col - 1] = true;
                }
                break;
            case 6:
                val = m_result[row + number * rowCount].test6;
                sprintf(num, "%.1f", val);
                str = QString(num).replace('.', ',');
                if ((m_scanSettings.RestrictionsTest7Min) > val || val > (m_scanSettings.RestrictionsTest7Max)) {
                    flags[col - 1] = true;
                }
                if (0.0 > val) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            case 7:
                val = m_result[row + number * rowCount].test7;
                sprintf(num, "%.3f", val);
                str = QString(num).replace('.', ',');
                if (val > m_scanSettings.VoltageErrorTest7) {
                    flags[col - 1] = true;
                    //str = "отказ";
                }
                break;
            default:
                break;
            }
            list.append(str);
        }
        protocol.append(rowStr
                            .arg(list[0])
                            .arg(list[1])
                            .arg(list[2])
                            .arg(list[3])
                            .arg(list[4])
                            .arg(list[5])
                            .arg(list[6])
                            .arg(list[7])
                            .arg(flags[0] ? "; color:red" : "")
                            .arg(flags[1] ? "; color:red" : "")
                            .arg(flags[2] ? "; color:red" : "")
                            .arg(flags[3] ? "; color:red" : "")
                            .arg(flags[4] ? "; color:red" : "")
                            .arg(flags[5] ? "; color:red" : "")
                            .arg(flags[6] ? "; color:red" : ""));
    }
    protocol.append(basement
                        .arg(QDate::currentDate().toString("dd.MM.yyyy"))
                        .arg(m_scanSettings.Fio));

    QString path = qApp->applicationDirPath()
                       .append("/")
                       .append(m_scanSettings.Type)
                       .append(QDate::currentDate().toString("/yyyy"));

    if (!QDir().mkpath(path)) {
        QMessageBox::critical(this, "", "Не удaётся записать на диск файл протокола!");
        return;
    }
    path = path
               .append("/")
               .append(serialNumber)
               .append(QDate::currentDate().toString("_dd.MM.yyyy"))
               .append("г.htm");

    m_paths[number] = path;

    QFile file4(path);
    qDebug() << file4.open(QFile::WriteOnly);
    qDebug() << file4.write(protocol.toLocal8Bit());
    file4.close();
    //    MyDialog* Dialog = new MyDialog(this, "123");
    //    Dialog->LoadFile(qApp->applicationDirPath().append("/").append("123.htm"));
}

void AutomaticMeasurements::on_pbStartStop_clicked(bool checked)
{
    pbStartStop->setChecked(checked);
    if (checked) {
        for (int i = 0, flag = 0; i < listWidget->count(); ++i) {
            if (listWidget->item(i)->isHidden()) {
                ++flag;
            }
            if (flag == 8) {
                QMessageBox::critical(this, "", "Не введен ни один серийный номер!");
                pbStartStop->setChecked(false);
                return;
            }
        }
        foreach (QCheckBox* cb, m_listCheckBox) {
            cb->setChecked(false);
        }
        pbStartStop->setText("Остановить измерения");
        m_worker = new Worker(m_doNotSkip, m_result, &m_scanSettings);
        connect(m_worker, &QThread::finished, [this]() { qDebug() << "QThread::finished"; on_pbStartStop_clicked(false); });
        connect(m_worker, &QThread::finished, m_worker, &QObject::deleteLater);
        connect(m_worker, &Worker::ShowMessage, this, &AutomaticMeasurements::ShowMessage);
        connect(m_worker, &Worker::UpdateProgresBar, this, &AutomaticMeasurements::UpdateProgresBar);
        progressBar->setValue(0);
        m_worker->start();
    }
    else {
        pbStartStop->setText("Начать измерения");
        m_worker->FinishMeasurements();
    }
}

//void AutomaticMeasurements::paintEvent(QPaintEvent* event)
//{
//    Q_UNUSED(event)
//    static QMutex mutex;
//    QMutexLocker locker(&mutex);
//    if (!m_paintState) {
//        return;
//    }
//    m_paintState = false;
//    for (int i = 0; i < 8; ++i) {
//        if (!listWidget->item(i)->isHidden()) {
//            SaveProtokol(listWidget->item(i)->text(), i);
//        }
//    }
//    for (int i = 0; i < 8; ++i) {
//        if (!m_paths[i].isEmpty()) {
//            ShowProtocol(i);
//        }
//    }
//}

void AutomaticMeasurements::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    if (MI::man()->IsConnected()) {
        return;
    }
    setEnabled(false);
}
///////////////////////////////////////
/// \brief Worker::Worker
/// \param doNotSkip
/// \param result
/// \param scanSettings
/// \param man
/// \param parent
///
Worker::Worker(bool* doNotSkip, Result_t* result, const ScanSettings* scanSettings, QObject* parent)
    : QThread(parent)
    , m_doNotSkip(doNotSkip)
    , m_result(result)
    , m_pScanSettings(scanSettings)
{
}

Worker::~Worker()
{
    m_semaphore.acquire(m_semaphore.available());
    //    quit();
    //    wait();
}

void Worker::Continue()
{
    m_semaphore.release(1);
}

void Worker::FinishMeasurements()
{
    if (MI::man()->SwitchCurrent(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
    if (!MI::man()->SetCurrent(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
    if (!MI::man()->Oscilloscope(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
    //quit();
    m_semaphore.release(2);
}

void Worker::Quit()
{
    //quit();
    if (MI::man()->SwitchCurrent(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
    if (!MI::man()->SetCurrent(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
    if (!MI::man()->Oscilloscope(0)) {
        //emit ShowMessage(3); //Нет связи с МАНом
    }
}

void Worker::run()
{
    m_counter = 0;
    setTerminationEnabled();
    m_semaphore.acquire(m_semaphore.available());
    if (!MI::man()->SetCurrent(0)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        //quit();
        return;
    }
    if (!MI::man()->SwitchCurrent(1)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        //quit();
        return;
    }
    // 2 Измерение выходного напряжения под номинальной нагрузкой Максимальное (249) Да
    emit ShowMessage(0);
    if (Finished()) {
        return;
    }
    if (!Test4()) {
        return;
    }
    // 8 Измерение выходного напряжения под номинальной нагрузкой Минимальное (130) Да
    emit ShowMessage(1);
    if (Finished()) {
        return;
    }
    if (!Test3()) {
        return;
    }
    // 7 Измерение Пульсации выходного напряжения под номинальной нагрузкой Минимальное (130) Да
    if (!Test2()) {
        return;
    }
    // 3 Измерение выходного напряжения под номинальной нагрузкой Номинальное (220) Да
    emit ShowMessage(2);
    if (Finished()) {
        return;
    }
    if (!Test1()) {
        return;
    }
    // 4 Измерение выходного напряжения без нагрузки Номинальное (220) Нет
    if (!Test5()) {
        return;
    }
    // 5 Поиск точки срабатывания защиты  Номинальное (220) Плавное нарастание
    if (!Test6()) {
        return;
    }
    // 6 Тест К.З. - восстановление напряжения при подключенной нагрузке  Номинальное (220) Да
    emit ShowMessage(4);
    if (Finished()) {
        return;
    }
    if (!Test7()) {
        return;
    }
    // end
    emit ShowMessage(20);
    m_semaphore.acquire(m_semaphore.available());
    //quit();
}

bool Worker::Finished()
{
    m_semaphore.acquire(1);
    if (m_semaphore.available()) {
        qDebug() << "Finished";
        m_semaphore.acquire(m_semaphore.available());
        //quit();
        if (!MI::man()->SwitchCurrent(0)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
        }
        if (!MI::man()->SetCurrent(0)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
        }
        if (!MI::man()->Oscilloscope(0)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
        }
        return true;
    }
    return false;
}

bool Worker::CheckFinished()
{
    if (m_semaphore.available()) {
        m_semaphore.acquire(m_semaphore.available());
        return true;
    }
    return false;
}

bool Worker::Test1()
{
    qDebug() << "Test1";
    Msleep(5000);
    QList<MeasuredValue_t> list;
    do {
        Msleep(500);

        if (!MI::man()->GetMeasuredValue(list)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            //quit();
            return false;
        }
        for (int i = 0; i < list.size(); ++i) {
            m_result[i].test1 = list[i].Value1;
            qDebug() << i << list[i].Value1;
        }
        if (CheckFinished()) {
            return false;
        }
    } while (list.size() != 8);
    if (CheckFinished()) {
        return false;
    }
    emit ShowMessage(21);
    return true;
}

bool Worker::Test2()
{
    qDebug() << "Test2";
    for (int i = 0; i < 8; ++i) {
        if (m_doNotSkip[i]) {
            if (!MI::man()->Oscilloscope(0)) {
                emit ShowMessage(3);
                terminate();
                qDebug() << "terminate()"; //Нет связи с МАНом
                return false;
            }
            if (!MI::man()->Oscilloscope(i + 1)) {
                emit ShowMessage(3);
                terminate();
                qDebug() << "terminate()"; //Нет связи с МАНом
                return false;
            }
            emit ShowMessage(i + 11);
            if (Finished()) {
                return false;
            }
        }
    }
    if (!MI::man()->Oscilloscope(0)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        return false;
    }
    emit ShowMessage(22);
    return true;
}

bool Worker::Test3()
{
    qDebug() << "Test3";
    Msleep(5000);
    QList<MeasuredValue_t> list;
    do {
        Msleep(500);

        if (!MI::man()->GetMeasuredValue(list)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        for (int i = 0; i < list.size(); ++i) {
            m_result[i].test3 = list[i].Value1;
            qDebug() << i << list[i].Value1;
        }
        if (CheckFinished()) {
            return false;
        }
    } while (list.size() != 8);
    if (CheckFinished()) {
        return false;
    }
    emit ShowMessage(23);
    return true;
}

bool Worker::Test4()
{
    qDebug() << "Test4";
    if (!MI::man()->SetCurrent(m_pScanSettings->RatedCurrent)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        return false;
    }
    Msleep(5000);
    QList<MeasuredValue_t> list;
    do {
        Msleep(500);
        if (!MI::man()->GetMeasuredValue(list)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        for (int i = 0; i < list.size(); ++i) {
            m_result[i].test4 = list[i].Value1;
            qDebug() << i << list[i].Value1;
        }
        if (CheckFinished()) {
            return false;
        }
    } while (list.size() != 8);
    if (CheckFinished()) {
        return false;
    }
    emit ShowMessage(24);
    return true;
}

bool Worker::Test5()
{
    qDebug() << "Test5";
    if (!MI::man()->SetCurrent(0)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        return false;
    }
    Msleep(5000);
    QList<MeasuredValue_t> list;
    do {
        Msleep(500);
        if (!MI::man()->GetMeasuredValue(list)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        for (int i = 0; i < list.size(); ++i) {
            m_result[i].test5 = list[i].Value1;
            qDebug() << i << list[i].Value1;
        }
        if (CheckFinished()) {
            return false;
        }
    } while (list.size() != 8);
    if (CheckFinished()) {
        return false;
    }
    emit ShowMessage(25);
    return true;
}

bool Worker::Test6()
{
    qDebug() << "Test6";
    MeasuredValue_t value;
    if (!MI::man()->SetCurrent(0)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        return false;
    };
    QList<double> peak;
    for (uint8_t i = 1; i <= 8; ++i) {
        if (m_doNotSkip[i - 1]) {
            peak.append(-1.0);
            if (!MI::man()->SetCurrent(m_pScanSettings->RestrictionsTest7Min, i)) {
                Msleep(50);
                if (!MI::man()->SetCurrent(m_pScanSettings->RestrictionsTest7Min, i)) {
                    emit ShowMessage(3);
                    terminate();
                    qDebug() << "terminate()"; //Нет связи с МАНом
                    return false;
                };
            };
            Msleep(5000);
            for (double current = m_pScanSettings->RestrictionsTest7Min;
                 current <= m_pScanSettings->RestrictionsTest7Max * 1.1;
                 current += (m_pScanSettings->RestrictionsTest7Max - m_pScanSettings->RestrictionsTest7Min) / 20.0) {
                Msleep(50);
                if (!MI::man()->SetCurrent(current, i)) {
                    Msleep(50);
                    if (!MI::man()->SetCurrent(current, i)) {
                        emit ShowMessage(3);
                        terminate();
                        qDebug() << "terminate()"; //Нет связи с МАНом
                        return false;
                    };
                };
                Msleep(50);
                if (!MI::man()->GetMeasuredValue(value, i)) {
                    Msleep(50);
                    if (!MI::man()->GetMeasuredValue(value, i)) {
                        emit ShowMessage(3);
                        terminate();
                        qDebug() << "terminate()"; //Нет связи с МАНом
                        return false;
                    }
                }
                qDebug() << i << value.Value2 << value.Value1;
                if (value.Value1 < 1.0) {
                    break;
                }
                peak.append(value.Value2);
                if (CheckFinished()) {
                    return false;
                }
            }
            if (!MI::man()->SetCurrent(0, i)) {
                emit ShowMessage(3);
                terminate();
                qDebug() << "terminate()"; //Нет связи с МАНом
                return false;
            };
            qSort(peak);
            m_result[i - 1].test6 = peak.last();
        }
    }
    emit ShowMessage(26);
    return true;
}

bool Worker::Test7()
{
    qDebug() << "Test7";
    if (!MI::man()->SetCurrent(m_pScanSettings->RatedCurrent)) {
        emit ShowMessage(3);
        terminate();
        qDebug() << "terminate()"; //Нет связи с МАНом
        return false;
    }
    Msleep(5000);
    QList<MeasuredValue_t> list;
    do {
        Msleep(500);
        if (!MI::man()->ShortCircuitTest(1)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        Msleep(2000);
        if (!MI::man()->ShortCircuitTest(0)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        Msleep(100);
        if (!MI::man()->GetMeasuredValue(list)) {
            emit ShowMessage(3);
            terminate();
            qDebug() << "terminate()"; //Нет связи с МАНом
            return false;
        }
        for (int i = 0; i < list.size(); ++i) {
            m_result[i].test7 = list[i].Value1;
            qDebug() << i << list[i].Value1;
        }
        if (CheckFinished()) {
            return false;
        }
    } while (list.size() != 8);
    emit ShowMessage(27);
    return true;
}

void Worker::Msleep(unsigned long time)
{
    time /= 10;
    do {
        emit UpdateProgresBar();
        msleep(10);
        if (CheckFinished()) {
            terminate();
            return;
        }
    } while (--time);
}
