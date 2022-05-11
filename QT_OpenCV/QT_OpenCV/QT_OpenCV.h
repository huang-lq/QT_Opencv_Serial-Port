#pragma once
//#pragma comment(lib, "libName")

#include <QtWidgets/QMainWindow>
#include "ui_QT_OpenCV.h"
#include<QtSerialPort/qserialport.h>
#include<QtSerialPort/qserialportinfo.h>

/*
*   If you have any questions, please contact me
*   Github: https://github.com/huang-lq
*   CSDN:   https://blog.csdn.net/weixin_44346103?type=blog
*/

using namespace std;

class QT_OpenCV : public QMainWindow
{
    Q_OBJECT

public:
    QT_OpenCV(QWidget *parent = Q_NULLPTR);
	int a;
	bool send_enter_flag = false;
private:
    Ui::QT_OpenCVClass ui;
	QSerialPort serial;

private slots:
	void on_OpenCameraBtn_clicked();	//障碍物识别
	void on_pushButton_clicked();       //人脸识别
	void on_CloseCameraBtn_clicked();   //关闭摄像头
	void on_HSV_clicked();              //HSV选取
	void on_actionRedo_triggered();
	void on_actionUndo_triggered();
	void on_btn_serch_clicked();
	void serialPort_readyRead();        //connect连接方式，串口接收的数据进行处理
	void on_cbb_baud_currentIndexChanged(const QString &arg1);
	void on_btn_clear_clicked();
	void on_btn_send_clicked();
	void on_cb_send_enter_clicked();
	void on_btn_uart_Ctrl_clicked();
	void on_pushButton_5_clicked();     //启动、停止
	void on_pushButton_7_clicked();     //步进电机正转
	void on_pushButton_8_clicked();     //步进电机反转
};


