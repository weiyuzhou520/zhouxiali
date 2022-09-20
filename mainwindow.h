#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    int on_windowsExit();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon * m_systemTrayIcon;
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
