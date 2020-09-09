
// SignalsPsDlg.h: файл заголовка
//

#pragma once
#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "ChartViewer.h"
#include "Signals_helper.h"
using namespace std;

// Диалоговое окно CSignalsPsDlg
class CSignalsPsDlg : public CDialogEx
{
	// Создание
public:
	CSignalsPsDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNALSPS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	// Частота дискретизации
	int sampling;
	// Несущая
	int f_0;
	// Частота модуляции
	int bitrate;
	// Число бит данных
	int bits_size;
	// ОСШ для несущего сигнала
	double SNR_1;
	// ОСШ для полученного сигнала
	double SNR_2;
	// Тип модуляции (1-АМ, 2-ФМ2, 3-MSK)
	int mod_type;
	// Задержка (0-1 * N1)
	double delay;
	//Объект класса Signal
	Signal MySignals;
	//вектор с данными для рисовалки
	vector<vector<double>> draw_vector;

	/*Объект отрисовки*/
	CChartViewer viewer1;
	/*Объект отрисовки*/
	CChartViewer viewer2;
	/*Объект отрисовки*/
	CChartViewer viewer3;

	//////////////////////////////////
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	//////////////////////////////////
	/*Преобразование вектора double к вектору DoubleArray*/
	DoubleArray vectorToArray(vector<double>& v)
	{
		return (v.size() == 0) ? DoubleArray() : DoubleArray(v.data(), (int)v.size());
	}
	void ViewerDraw(vector<vector<double>>& data, double Xmin, double Xmax, CChartViewer& viewer_num, string PathPic, bool podpisi);
};
