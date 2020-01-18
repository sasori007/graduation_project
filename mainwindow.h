#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "struct.h"

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
    void on_newFile_triggered();

    void on_importButton_clicked();

    void on_closeFile_triggered();

    void on_exit_triggered();

    void on_ready_triggered();

private:
    Ui::MainWindow *ui;
    vector<vector<cell> >chart;
    vector<string> label;
    const string columnAttribute[4] = {"", "离散型属性", "连续型属性", "类别标签"};
    int dataSum;        // 保存数据数
    int attributionSum; // 保存属性数
};
#endif // MAINWINDOW_H
