#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_systemTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/new/prefix1/350.jpeg");
    m_systemTrayIcon->setIcon(icon);
    m_systemTrayIcon->setToolTip("系统托盘演示程序");

    QAction *action = new QAction(this);
    action->setText("退出程序");

    connect(action, &QAction::triggered, this, &MainWindow::on_windowsExit);
    QMenu * menu = new QMenu(this);
    menu->addAction(action);
    m_systemTrayIcon->setContextMenu(menu);
    connect(m_systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    //在系统托盘显示此对象
    m_systemTrayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

int MainWindow::on_windowsExit()
{
    QApplication::exit(0);
    return 0;
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {

    case QSystemTrayIcon::Trigger:
        this->show();
        break;

    case QSystemTrayIcon::DoubleClick:

        this->show();

        break;

    default:

        break;

    }
}
