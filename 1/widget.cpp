#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    ui->addressEdit->setText("127.0.0.1");
    ui->portEdit->setText("8080");
    m_clientSocket = new QTcpSocket(this);
    connect(ui->connectBtn,&QPushButton::clicked,[this]{
            //判断当前是否已连接，连接了就断开
            if(m_clientSocket->state()==QAbstractSocket::ConnectedState){
                //如果使用disconnectFromHost()不会重置套接字，isValid还是会为true
                m_clientSocket->abort();
            }else if(m_clientSocket->state()==QAbstractSocket::UnconnectedState){
                //从界面上读取ip和端口
                const QHostAddress address=QHostAddress(ui->addressEdit->text());
                const unsigned short port=ui->portEdit->text().toUShort();
                //连接服务器
                m_clientSocket->connectToHost(address,port);
            }else{
                ui->textBrowser->append("It is not ConnectedState or UnconnectedState");
            }
        });

        //连接状态
        connect(m_clientSocket,&QTcpSocket::connected,[this]{
            //已连接就设置为不可编辑
            ui->connectBtn->setText("Disconnect");
            ui->addressEdit->setEnabled(false);
            ui->portEdit->setEnabled(false);
            updateState();
        });
        connect(m_clientSocket,&QTcpSocket::disconnected,[this]{
            //断开连接还原状态
            ui->connectBtn->setText("Connect");
            ui->addressEdit->setEnabled(true);
            ui->portEdit->setEnabled(true);
            updateState();
        });

        //发送数据
        connect(ui->sendBtn,&QPushButton::clicked,[this]{
            Msgpkg pkg;
            if (ui->mesgEdit->toPlainText().toLatin1().isEmpty() && ui->nameEdit->text().isEmpty())
            {
                return;
            }
            pkg.msg = ui->mesgEdit->toPlainText().toLatin1().data();
            pkg.name = ui->nameEdit->text().toLatin1().data();
            qDebug()<<pkg.msg <<pkg.name;
            //判断是可操作，isValid表示准备好读写
            if(!m_clientSocket->isValid())
                return;

            m_clientSocket->write();
            ui->mesgEdit->clear();
            //client->waitForBytesWritten();
        });

        //收到数据，触发readyRead
        connect(m_clientSocket,&QTcpSocket::readyRead,[this]{
            //没有可读的数据就返回
            if(m_clientSocket->bytesAvailable()<=0)
                return;
            //注意收发两端文本要使用对应的编解码
            const QString recv_text=QString::fromUtf8(m_clientSocket->readAll());
            ui->textBrowser->append(QString("[%1:%2]")
                                 .arg(m_clientSocket->peerAddress().toString())
                                 .arg(m_clientSocket->peerPort()));
            ui->textBrowser->append(recv_text);
        });

}

Widget::~Widget()
{
    m_clientSocket->abort();
    delete ui;
}

void Widget::updateState()
{
    if(m_clientSocket->state()==QAbstractSocket::ConnectedState)
    {
            setWindowTitle(QString("Client[%1:%2]")
                           .arg(m_clientSocket->localAddress().toString())
                           .arg(m_clientSocket->localPort()));
    } else {
        setWindowTitle("Client");
    }
}

