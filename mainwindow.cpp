#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "struct.h"
#include "dtnode.h"
#include "dtree.h"

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

    // 初始化配置标志
    toConfigure = false;
    ui->analysis->setEnabled(toConfigure);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newFile_triggered()
{
    chart.clear();
    label.clear();
    dataSum = 0;
    attributionSum = 0;
    toConfigure = false;
    ui->analysis->setEnabled(toConfigure);
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
    dataSum = 0;        // 保存数据数
    attributionSum = 0; // 保存属性数
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
        comBox->addItem(QString::fromStdString(columnAttribute[1]));
        comBox->addItem(QString::fromStdString(columnAttribute[2]));
        comBox->addItem(QString::fromStdString(columnAttribute[3]));
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

void MainWindow::on_ready_triggered()
{
    label.clear();
    chart.clear();
    int labelSC = -1;
    int labelSum = 0;

    // 获取类别标签下标和数量
    for(int i = 0; i < attributionSum && labelSum < 2; ++i)
    {
        QWidget* widget = ui->tableWidget->cellWidget(0, i);
        QComboBox* combox = (QComboBox*)widget;
        string str = combox->currentText().toStdString();
        if(str == columnAttribute[3])
        {
            labelSC = i;
            ++labelSum;
        }
    }

    // 将类别标签拥有的值提取出来
    if(labelSum == 1)
    {// 类别标签设置正确
        for(int i = 1; i < dataSum; ++i)
        {
            vector<string>::iterator iter;
            string str = ui->tableWidget->item(i, labelSC)->text().toStdString();

            iter = std::find(label.begin(), label.end(), str);
            if(iter == label.end())
                label.push_back(str);
        }
        vector<string>::iterator iter = label.begin();
        while(iter < label.end())
            std::cout << *iter++ << endl;
    }

    else if(labelSum > 1)
    {// 多个类别标签
        qDebug() << tr("设置多个类别标签，类别标签只能有一个!");
        QMessageBox::information(this, "类别标签过多", "设置多个类别标签，类别标签只能有一个!");
        return ;
    }

    else
    {// 没有设置类别标签
        qDebug() << tr("没有设置类别标签，必须设置一个类别标签!");
        QMessageBox::information(this, "类别标签过多", "没有设置类别标签，必须设置一个类别标签!");
        return ;
    }

    // 将数据表格存入vector<vector<cell> >
    vector<cell> addData;
    cell c;

    // 将表头存入vector<vector<cell> >
    for(int i = 0; i < attributionSum; ++i)
    {
        if(labelSC == i)
            // 跳过类别标签列
            continue;
        string str;
        QWidget * widget = ui->tableWidget->cellWidget(0, i);
        QComboBox *combox = (QComboBox*)widget;
        int cASC = 0;

        // 获取该列数据类型
        str = combox->currentText().toStdString();
        while(str != columnAttribute[cASC])
            ++cASC;
        c.flag = 0 - cASC;

        // 获取表头文本
        str = ui->tableWidget->horizontalHeaderItem(i)->text().toStdString();
        c.value = str;

        // 将当前获取的表头信息放入
        addData.push_back(c);
    }
    // 将表头放入<vector<vetor<cell> >中
    chart.push_back(addData);

    // 将表格中的数据存入<vector<vector<cell> >
    for(int i = 1; i < dataSum; ++i)
    {
        // 初始化
        addData.clear();
        c.value = "";
        c.flag = -1;

        // 设置该行的类别标签值
        vector<string>::iterator iter;
        iter = std::find(label.begin(), label.end(), ui->tableWidget->item(i, labelSC)->text().toStdString());
        c.flag = iter - label.begin();

        // 提取当前单元格的值
        for(int j = 0; j < attributionSum; ++j)
        {
            if(labelSC == j)
                continue;

            c.value = ui->tableWidget->item(i, j)->text().toStdString();
            addData.push_back(c);
        }

        // 将该行数据放入<vector<vector<cell> >
        chart.push_back(addData);
    }

    /*
    for(int i = 0; i < chart.size(); ++i)
    {
        for(int j = 0; j < chart[0].size(); ++j)
            cout << chart[i][j].value << "\t";
        cout << endl;
        for(int j = 0; j < chart[0].size(); ++j)
            cout << chart[i][j].flag << "\t";
        cout << endl;
    }
    */
    toConfigure = true;
    ui->analysis->setEnabled(toConfigure);
    QMessageBox::information(this, "配置完毕", "列表属性配置完毕，F5开始进行分析！");
}

void MainWindow::on_analysis_triggered()
{
    if(!toConfigure)
    {
        QMessageBox::information(this, "分析失败", "没有进行类别标签配置, 请先配置类别标签");
        return ;
    }
    DTNode* p = new DTNode;
    DTree tree(p);

    tree.createDTree(p, chart, label, dataSum);
    tree.traverDTree(tree.returnRoot(), 0);
}

void MainWindow::on_closeFile_triggered()
{
    chart.clear();
    label.clear();
    dataSum = 0;
    attributionSum = 0;
    toConfigure = false;
    ui->analysis->setEnabled(toConfigure);
    ui->importButton->show();
    ui->tableWidget->hide();
}

void MainWindow::on_exit_triggered()
{
    qApp->quit();
}


