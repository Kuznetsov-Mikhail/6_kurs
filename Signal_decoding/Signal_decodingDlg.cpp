
// Signal_decodingDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Signal_decoding.h"
#include "Signal_decodingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CSignaldecodingDlg



CSignaldecodingDlg::CSignaldecodingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNAL_DECODING_DIALOG, pParent)
	, bits_count(10)
	, fd(35e4)
	, bitrate(5e3)
	, snr(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignaldecodingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, bits_count);
	DDX_Text(pDX, IDC_EDIT2, fd);
	DDX_Text(pDX, IDC_EDIT3, bitrate);
	DDX_Text(pDX, IDC_EDIT4, snr);
}

BEGIN_MESSAGE_MAP(CSignaldecodingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CSignaldecodingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSignaldecodingDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений CSignaldecodingDlg

BOOL CSignaldecodingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSignaldecodingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSignaldecodingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSignaldecodingDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}


void CSignaldecodingDlg::OnBnClickedButton1()
{
	UpdateData(1);
	decoder.Init(bits_count);
	UpdateData(0);
}
