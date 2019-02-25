#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "QDebug"
#include "QFile"
#include "QTextStream"
#include "qmessagebox.h"
#include "windows.h"
#include "qprocess.h"
#include "QCoreApplication"
#include "QDir"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->setStyleSheet("QLabel{border:1px solid black;}");
    ui->label_3->setStyleSheet("QLabel{border:1px solid black;}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this,"Select folder...","./");
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {
        ui->label_2->setText(file_path);
    }
}

void MainWindow::on_toolButton_2_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this,"Select folder...","./");
    if(file_path.isEmpty())
    {
        return;
    }
    else
    {
        ui->label_3->setText(file_path);
    }
}

void MainWindow::on_pushButton_clicked()
{
    char a[20];
    QString str = ui->textEdit->toPlainText();
    int len = str.length();
    for(int i = 0; i < len; ++i)
    {
        QChar t = str.at(i);
        a[i] = t.toLatin1();
    }
    QString convert = QString(a);
    if(len>3)
    {
        QMessageBox::about(NULL, "错误", "error:00(任务名高于三位!)");
        Sleep(500);
        ui->progressBar->setValue(0);

    }
    else
    {
        if(len<3)
        {
            QMessageBox::about(NULL, "错误", "error:01(任务名低于三位!)");
            Sleep(500);
            ui->progressBar->setValue(0);
        }
        else
        {
            convert.replace(QString("("), QString(""));
            convert.replace(QString("?"), QString(""));
            convert.replace(QString("�"), QString(""));
            convert.replace(QString("#"), QString(""));
            QFile file(QString("%1.vbs").arg(convert));//处理vbs
            if(file.exists())
            {
                QMessageBox::about(NULL, "错误", "error:02(该任务名已存在!)");
                Sleep(500);
                ui->progressBar->setValue(0);
            }
            else
            {
                QString temp = "temp.bat";

                file.open( QIODevice::ReadWrite | QIODevice::Text );
                Sleep(500);
                ui->progressBar->setValue(10);
                QTextStream invbs(&file);
                QString tempdir = QDir::currentPath();
                invbs<<"Set ws = CreateObject(\"Wscript.Shell\")\n"<<"ws.run \""<<tempdir<<"/"<<convert<<".bat\",vbhide";
                file.close();//到此处 vbs成功建立,收回内存;
                Sleep(500);
                ui->progressBar->setValue(30);

                //获取间隔时间,单位
                char b[20];
                QString time1 = ui->textEdit_2->toPlainText();
                int len1 = time1.length();
                for(int i = 0; i < len1; ++i)
                {
                    QChar t = time1.at(i);
                    b[i] = t.toLatin1();
                }
                QString time = QString(b);
                Sleep(500);
                ui->progressBar->setValue(40);

                QFile tempf(temp);//临时bat
                tempf.open( QIODevice::ReadWrite | QIODevice::Text );
                QTextStream tempbat(&tempf);
                QString str = time;
                QString tmp,text;
                    for(int j = 0; j < str.length(); j++)
                    {
                    if(str[j] >= '0' && str[j] <= '9')
                    tmp.append(str[j]);
                    }
                    text=tmp.mid(tmp.size()-2,2);//截取后两位
                    qDebug()<<tmp<<"---->"<<text;
                Sleep(500);
                ui->progressBar->setValue(45);

                //单位
                QString timefar;
                int index = ui->comboBox->currentIndex();
                if(index == 0){timefar = "minute";}
                if(index == 1){timefar = "hourly";}
                if(index == 2){timefar = "daily";}

                tempbat<<"schtasks /create /tn "<<convert<<" /tr "<<tempdir<<"/"<<convert<<".vbs"<<" /sc "<<timefar<<" /mo "<<text;
                tempf.close();

                Sleep(500);
                ui->progressBar->setValue(50);

                //获取两个文件夹地址
                QString send = ui->label_2->text();
                QString recieved = ui->label_3->text();
                Sleep(500);
                ui->progressBar->setValue(70);

                QFile file2(QString("%1.bat").arg(convert));//主要处理bat
                file2.open( QIODevice::ReadWrite | QIODevice::Text );
                //QDir::currentPath()
                Sleep(500);
                ui->progressBar->setValue(80);

                QTextStream inbat(&file2);
                inbat<<"xcopy /e /h /y \""<<send<<"\" \""<<recieved<<"\"";
                file2.close();//回收内存

                Sleep(500);
                ui->progressBar->setValue(95);

                system("temp.bat");
                Sleep(500);
                ui->progressBar->setValue(100);
                system("del /q temp.bat");
                QMessageBox::about(NULL, "成功", "计划任务开启成功!如果无效,请检查是否是杀软将系统的计划任务服务开机启动项关闭!\n您可以关闭此软件,不会影响任务正常执行,但请不要删除本软件目录下的任何文件!\n自动备份非即时生效,假设您设置每1小时自动同步一次,那么第一次同步将会在一个小时后进行,现在不会开始同步!\n如有问题请咨询微信:1101635162");
                Sleep(500);
                ui->progressBar->setValue(0);
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::about(NULL, "如何删除计划任务?", "请右键计算机-管理-任务计划程序-任务计划程序库中,找到您设置的名称,然后右键删除即可!");
}
