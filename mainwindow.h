#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class analysisTree;
class DTree;

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

    void on_analysis_triggered();

    void on_analysisTreeShow_triggered();

private:
    Ui::MainWindow *ui;
    vector<vector<cell> >chart;
    vector<string> label;
    const string columnAttribute[4] = {"", "离散型属性", "连续型属性", "类别标签"};
    int dataSum;        // 保存数据数
    int attributionSum; // 保存属性数

    bool toConfigure;   // 配置列属性标志
    bool aTCreate;      // 分析树建立标志

    analysisTree* new_Tree;
    DTree* tree;
};
#endif // MAINWINDOW_H
