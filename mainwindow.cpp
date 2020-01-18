#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTextStream>
#include <QComboBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 导入表格按钮添加图标
    QIcon importIcon;
    importIcon.addFile(tr(":/home/images/导入.png"));
    ui->importButton->setIcon(importIcon);
    ui->tableWidget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_3_triggered()
{
    qApp->quit();
}

void MainWindow::on_action_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->hide();
    ui->importButton->show();
}

void MainWindow::on_importButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择txt文件"), ".", tr("txt文件(*.txt)"));
    if(fileName != "")
    {
        ui->importButton->hide();
        ui->tableWidget->show();
    }

    QFile* file = new QFile(fileName);
    int dataSum = 0;        // 保存数据数
    int attributionSum = 0; // 保存属性数
    QString head;
    QStringList hHeadList;
    QStringList vHeadList;
    if(!file->open(QIODevice::ReadOnly))
        qDebug() << tr("打开失败！") << endl;

    // 获取数据行数
    QTextStream txtInput(file);
    while(!txtInput.atEnd())
    {
        txtInput.readLine();
        ++dataSum;
    }
    txtInput.seek(0);

    // 获取表头和属性数
    head = txtInput.readLine();
    hHeadList = head.split(" ");
    attributionSum = hHeadList.size();

    // 设置表格
    ui->tableWidget->setRowCount(dataSum);          // 设置行数
    ui->tableWidget->setColumnCount(attributionSum);// 设置列数
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);            // 禁止修改
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);// 自动适应宽度

    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(hHeadList);
    /*
    vHeadList << "数据属性";
    for(int i = 1; i < dataSum; ++i)
        vHeadList << QString::number(i);
    ui->tableWidget->setVerticalHeaderLabels(vHeadList);
    */

    // 设置列属性comboBox
    for(int i = 0; i < attributionSum; ++i)
    {
        QComboBox* comBox = new QComboBox();
        comBox->addItem("离散型数据");
        comBox->addItem("连续型属性");
        comBox->addItem("类别标签");
        ui->tableWidget->setCellWidget(0, i, comBox);
    }

    // 读取表格数据
    for(int i = 1; !txtInput.atEnd(); ++i)
    {
        QString line = txtInput.readLine();
        QStringList qsl = line.split(" ");
        for(int j = 0; j < attributionSum; ++j)
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(qsl[j]));
    }

    file->close();
}

void MainWindow::on_action_10_triggered()
{
    ui->importButton->show();
    ui->tableWidget->hide();
}
