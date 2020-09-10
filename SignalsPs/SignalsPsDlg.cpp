
// SignalsPsDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "SignalsPs.h"
#include "SignalsPsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CSignalsPsDlg



CSignalsPsDlg::CSignalsPsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNALSPS_DIALOG, pParent)
	, sampling(350000)
	, f_0(20000)
	, bitrate(5000)
	, bits_size(20)
	, SNR_1(10)
	, SNR_2(1)
	, mod_type(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignalsPsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC1, viewer1);
	DDX_Control(pDX, IDC_PIC2, viewer2);
	DDX_Control(pDX, IDC_PIC3, viewer3);
	DDX_Text(pDX, IDC_EDIT1, sampling);
	DDX_Text(pDX, IDC_EDIT2, f_0);
	DDX_Text(pDX, IDC_EDIT3, bitrate);
	DDX_Text(pDX, IDC_EDIT4, bits_size);
	DDX_Text(pDX, IDC_EDIT5, SNR_1);
	DDX_Text(pDX, IDC_EDIT6, SNR_2);
	DDX_Text(pDX, IDC_EDIT7, mod_type);
}

BEGIN_MESSAGE_MAP(CSignalsPsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CSignalsPsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSignalsPsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSignalsPsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSignalsPsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Обработчики сообщений CSignalsPsDlg

BOOL CSignalsPsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CSignalsPsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSignalsPsDlg::OnPaint()
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
HCURSOR CSignalsPsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSignalsPsDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}

//Signals Generation
void CSignalsPsDlg::OnBnClickedButton1()
{
	UpdateData(1);
	MySignals.Init(sampling, f_0, bitrate, bits_size, SNR_1, SNR_2, mod_type, 0.3);
	MySignals.GetSignals();
	MySignals.addNoize(MySignals.Signal1, SNR_1);
	MySignals.addNoize(MySignals.Signal2, SNR_2);

	/// /////////
	draw_vector.resize(1);
	draw_vector[0] = MySignals.Signal1;
	ViewerDraw(draw_vector, 0, MySignals.Signal1.size(), viewer1, "Signal1.png", false);
	draw_vector[0] = MySignals.Signal2;
	ViewerDraw(draw_vector, 0, MySignals.Signal2.size(), viewer2, "Signal2.png", false);

	UpdateData(0);
}

//ММП
void CSignalsPsDlg::OnBnClickedButton2()
{
	MySignals.Get_MMP(MMP);
	draw_vector.resize(1);
	draw_vector[0] = MMP;
	ViewerDraw(draw_vector, 0, MMP.size(), viewer3, "MMP.png", false);
}

//Исследование
void CSignalsPsDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
}

void CSignalsPsDlg::ViewerDraw(vector<vector<double>>& data, double Xmin, double Xmax, CChartViewer& viewer_num, string PathPic, bool podpisi)
{
	if (data.empty())return;
	// In this example, we simply use random data for the 3 data series.
	vector<DoubleArray> Arr_dataReal; Arr_dataReal.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		Arr_dataReal[i] = vectorToArray(data[i]);
	}

	vector<double>Datatime;

	double OXstep = (Xmax - Xmin) / (data[0].size() - 1);
	for (double i = Xmin; i <= Xmax; i += OXstep)Datatime.push_back(i);
	DoubleArray timeStamps = vectorToArray(Datatime);

	// Create a XYChart object of size 600 x 400 pixels
	XYChart* c = new XYChart(850, 240);

	// Add a title box using grey (0x555555) 20pt Arial font
	//c->addTitle("", "arial.ttf", 20, 0x555555);

	// Set the plotarea at (70, 70) and of size 500 x 300 pixels, with transparent background and
	// border and light grey (0xcccccc) horizontal grid lines
	c->setPlotArea(70, 50, 700, 120, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

	// Add a legend box with horizontal layout above the plot area at (70, 35). Use 12pt Arial font,
	// transparent background and border, and line style legend icon.
	LegendBox* b = c->addLegend(20, 5, false, "arial.ttf", 12);
	b->setBackground(Chart::Transparent, Chart::Transparent);
	b->setLineStyleKey();

	// Set axis label font to 12pt Arial
	c->xAxis()->setLabelStyle("arial.ttf", 12);
	c->yAxis()->setLabelStyle("arial.ttf", 12);

	// Set the x and y axis stems to transparent, and the x-axis tick color to grey (0xaaaaaa)
	c->xAxis()->setColors(Chart::TextColor, Chart::TextColor, Chart::TextColor, 0xaaaaaa);
	c->yAxis()->setColors(Chart::TextColor);

	// Set the major/minor tick lengths for the x-axis to 10 and 0.
	c->xAxis()->setTickLength(10, 0);

	// For the automatic axis labels, set the minimum spacing to 80/40 pixels for the x/y axis.
	c->xAxis()->setTickDensity(80);
	c->yAxis()->setTickDensity(40);

	// Add a title to the y axis using dark grey (0x555555) 14pt Arial font
	c->yAxis()->setTitle("", "arial.ttf", 14, 0x555555);

	// Add a line layer to the chart with 3-pixel line width
	LineLayer* layer = c->addLineLayer();
	layer->setLineWidth(3);
	//
	if (podpisi) layer->setDataLabelFormat("{value|1} ");
	// Add 3 data series to the line layer
	for (int i = 0; i < Arr_dataReal.size(); i++)
	{
		stringstream ss;
		ss << "Data " << i;
		string name = ss.str();
		int color = 0. + 16777216 * rand() / RAND_MAX;
		layer->addDataSet(Arr_dataReal[i], color, name.c_str());
	}
	// The x-coordinates for the line layer
	layer->setXData(timeStamps);
	viewer_num.setChart(c);
	const char* chPathPic = PathPic.c_str();
	c->makeChart(chPathPic);
	delete c;
}
