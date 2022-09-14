#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE
struct Msgpkg
{
    QString name;
    QString msg;
};
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void updateState();
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *m_clientSocket;
};
#endif // WIDGET_H
