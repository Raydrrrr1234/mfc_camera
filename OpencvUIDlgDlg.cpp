// OpencvUIDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpencvUIDlg.h"
#include "OpencvUIDlgDlg.h"
#include "cv.h"
#include "highgui.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CvvImage m_CvvImage;
CvCapture * m_Video;
IplImage* m_Frame;//用于存储一帧
IplImage* m_GrabFrame;//用于存储一帧
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
bool b_flagProcess;
bool b_flagSaveMovie=0;
CvVideoWriter* m_Movie;
char chEdit[10];
double m_vieoProtery;
int m_fameCount=0;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpencvUIDlgDlg dialog

COpencvUIDlgDlg::COpencvUIDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpencvUIDlgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpencvUIDlgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpencvUIDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpencvUIDlgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpencvUIDlgDlg, CDialog)
	//{{AFX_MSG_MAP(COpencvUIDlgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPENCAMERA, OnOpencamera)
	ON_BN_CLICKED(IDC_CLOSECAMERA, OnClosecamera)
	ON_BN_CLICKED(IDC_GRABFRAME, OnGrabframe)
	ON_BN_CLICKED(IDC_SAVEMOVIE, OnSavemovie)
	ON_BN_CLICKED(IDC_STOPSAVEMOVIE, OnStopsavemovie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpencvUIDlgDlg message handlers

BOOL COpencvUIDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	pwnd = GetDlgItem(IDC_SHOWPIC);
	//pwnd->MoveWindow(35,30,387,318);
    pDC =pwnd->GetDC();
	//pDC =GetDC();
    hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);


	CDC MemDC; 
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BLANK); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	
	//CClientDC dc(this); 
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
	
	m_Bitmap1.DeleteObject();
	MemDC.DeleteDC();

	
	b_flagProcess=0;
	Invalidate();



	SetDlgItemText(IDC_EDIT_TIME,"无");
	SetDlgItemText(IDC_EDIT_FRAME_WIDTH,"无");
	SetDlgItemText(IDC_EDIT_FRAME_HEIGHT,"无");
	SetDlgItemText(IDC_EDIT_FPS,"无");
	SetDlgItemText(IDC_EDIT_FOURCC,"无");
	SetDlgItemText(IDC_EDIT_FRAME_COUNT,"无");



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpencvUIDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpencvUIDlgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	if (!b_flagProcess)
	{
		CDC MemDC; 
		CBitmap m_Bitmap1;
		m_Bitmap1.LoadBitmap(IDB_BLANK); 
		MemDC.CreateCompatibleDC(NULL);
		MemDC.SelectObject(&m_Bitmap1);
		
		//CClientDC dc(this); 
		pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
		
		m_Bitmap1.DeleteObject();
		MemDC.DeleteDC();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpencvUIDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COpencvUIDlgDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//	KillTimer(nIDEvent);
	m_Frame=cvQueryFrame(m_Video);
//	cvNamedWindow("new",1);
//	cvShowImage("new",m_Frame);
	m_CvvImage.CopyOf(m_Frame,1);
    m_CvvImage.DrawToHDC(hDC, &rect);
    //cvWaitKey(10);
	b_flagProcess=1;
 


	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_POS_MSEC);
	itoa(m_vieoProtery,chEdit,10);
	SetDlgItemText(IDC_EDIT_TIME,chEdit);
	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_FRAME_WIDTH);
	itoa(m_vieoProtery,chEdit,10);
	SetDlgItemText(IDC_EDIT_FRAME_WIDTH,chEdit);
	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_FRAME_HEIGHT);
	itoa(m_vieoProtery,chEdit,10);
	SetDlgItemText(IDC_EDIT_FRAME_HEIGHT,chEdit);
	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_FPS);
	itoa(m_vieoProtery,chEdit,10);
	SetDlgItemText(IDC_EDIT_FPS,chEdit);
	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_FOURCC);
	itoa(m_vieoProtery,chEdit,10);
	SetDlgItemText(IDC_EDIT_FOURCC,chEdit);
	m_fameCount++;
//	m_vieoProtery=cvGetCaptureProperty(m_Video,CV_CAP_PROP_FRAME_COUNT);
	itoa(m_fameCount,chEdit,10);
	SetDlgItemText(IDC_EDIT_FRAME_COUNT,chEdit);

	if (b_flagSaveMovie)
	{
		if (!m_Movie)
			return;   
		cvWriteFrame(m_Movie,m_Frame);
	}
	

	CDialog::OnTimer(nIDEvent);
}

void COpencvUIDlgDlg::OnOpencamera() 
{
	// TODO: Add your control notification handler code here


	                      
	//m_Video=cvCaptureFromCAM(-1);
	m_Video=cvCreateCameraCapture(-1);


	if (!m_Video)
	{
		AfxMessageBox("无法打开摄像头");
		return;
	}
	
	SetTimer(1,100,NULL);


}

void COpencvUIDlgDlg::OnClosecamera() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	cvReleaseCapture(&m_Video);
	cvReleaseVideoWriter(&m_Movie);
	b_flagProcess=0;
	m_fameCount=0;
	b_flagSaveMovie=0;

	CDC MemDC; 
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BLANK); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
	m_Bitmap1.DeleteObject();
	MemDC.DeleteDC();
	
	SetDlgItemText(IDC_EDIT_TIME,"无");
	SetDlgItemText(IDC_EDIT_FRAME_WIDTH,"无");
	SetDlgItemText(IDC_EDIT_FRAME_HEIGHT,"无");
	SetDlgItemText(IDC_EDIT_FPS,"无");
	SetDlgItemText(IDC_EDIT_FOURCC,"无");
	SetDlgItemText(IDC_EDIT_FRAME_COUNT,"无");
}

void COpencvUIDlgDlg::OnGrabframe() 
{
	// TODO: Add your control notification handler code here
	if (!m_Video)
		return;

	m_GrabFrame=cvQueryFrame(m_Video);
	if (!m_GrabFrame)
		return;

	static	char countsnap='1';
	CString   m_name="";
	m_name=+countsnap;
	m_name+=".bmp";	
	
	IplImage * m_snap=cvCreateImage(cvGetSize(m_GrabFrame),m_GrabFrame->depth,m_GrabFrame->nChannels);
	cvCopy(m_GrabFrame,m_snap,NULL);

	const char* pszStr = m_name.GetBuffer(m_name.GetLength());
	cvSaveImage(pszStr,m_snap);	//把图像写入文件

	countsnap++;

}

//DEL BOOL COpencvUIDlgDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	CDC MemDC; 
//DEL 	CBitmap m_Bitmap1;
//DEL 	m_Bitmap1.LoadBitmap(IDB_BLANK); 
//DEL 	MemDC.CreateCompatibleDC(NULL);
//DEL 	MemDC.SelectObject(&m_Bitmap1);
//DEL 	
//DEL 	//CClientDC dc(this); 
//DEL 	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
//DEL 	
//DEL 	m_Bitmap1.DeleteObject();
//DEL 	MemDC.DeleteDC();	
//DEL 	return CDialog::Create(IDD, pParentWnd);
//DEL }

void COpencvUIDlgDlg::OnSavemovie() 
{
	// TODO: Add your control notification handler code here
	//m_Movie=cvCreateVideoWriter("video.avi",-1,10,cvSize(352,288),1);
	if (b_flagProcess)
	{
		//m_Movie=cvCreateVideoWriter("video.avi",CV_FOURCC('P','I','M','1'),10,cvSize(352,288),1);
		m_Movie=cvCreateVideoWriter("video.avi",-1,10,cvSize(352,288),1);
		b_flagSaveMovie=1;
	}

}

void COpencvUIDlgDlg::OnStopsavemovie() 
{
	// TODO: Add your control notification handler code here
	cvReleaseVideoWriter(&m_Movie);
	b_flagSaveMovie=0;	
}
