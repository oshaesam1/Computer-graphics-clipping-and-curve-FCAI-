#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
int x1,x2;
int myRadius,x_c,y_c,x_2,y_2;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void draw8points(HDC hdc,int x,int y,int xc,int yc){
SetPixel(hdc,xc+x,yc+y,RGB(0,0,100));
SetPixel(hdc,xc-x,yc+y,RGB(0,0,100));
SetPixel(hdc,xc+x,yc-y,RGB(0,0,100));
SetPixel(hdc,xc-x,yc-y,RGB(0,0,100));

SetPixel(hdc,xc-y,yc+x,RGB(0,0,100));
SetPixel(hdc,xc+y,yc-x,RGB(0,0,100));
SetPixel(hdc,xc+y,yc+x,RGB(0,0,100));
SetPixel(hdc,xc-y,yc-x,RGB(0,0,100));
}

void DrawCircleMidpoint(HDC hdc,int xc,int yc,int r){
int x=0;
int y=r;
double d=1-r;
draw8points(hdc,x,y,xc,yc);
while(x<y){
    if(d<=0){
        d=d+2*x+3;
    }
    else{
        d=d+2*(x-y)+5;
        y--;
    }
    x++;
    draw8points(hdc,x,y,xc,yc);
}
}
int Round(double x)
{
	return (int)(x + 0.5);
}
struct Vector{
	double v[2];
	Vector(double x = 0, double y = 0)
	 { v[0] = x; v[1] = y; }
	double& operator[](int i){ return v[i];
	}
};

int getRadius(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

COLORREF getColour(int x1, int y1, int x2, int y2, int radius)
{
    int R = getRadius(x1, y1, x2, y2);
    if (R > radius)
        return RGB(0, 0, 100);
    else
        return RGB(255, 0, 0);
}

void DrawHermiteCurve(HDC hdc,Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF color)
{
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t*t, t3 = t2*t;
		double x = a0 + a1*t + a2*t2 + a3*t3;
		double y = b0 + b1*t + b2*t2 + b3*t3;
		color = getColour(x, y, x_c, y_c, myRadius);
        SetPixel(hdc, Round(x), Round(y), color);
}
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    static Vector v[4];
	static int n = 0;
static int i=0;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONDOWN:
            if (i==0){
            x_c=LOWORD(lParam);
            y_c=HIWORD(lParam);
            i++;}
            else if(i==1)
            {
            x_2=LOWORD(lParam);
            y_2=HIWORD(lParam);
            myRadius=sqrt(pow((x_2-x_c),2)+pow((y_2-y_c),2));
            DrawCircleMidpoint(hdc,x_c,y_c,myRadius);
            i++;
            }
           else if(i>1)
            {
                       v[n] = Vector(LOWORD(lParam), HIWORD(lParam));
                  if (n == 3){
                   Vector T1(3 * (v[1][0] - v[0][0]), 3 * (v[1][1] - v[0][1]));
                   Vector T2(3 * (v[3][0] - v[2][0]), 3 * (v[3][1] - v[2][1]));
                   hdc = GetDC(hwnd);
                   DrawHermiteCurve(hdc, v[0], T1, v[3], T2, RGB(255, 0, 0));
                   ReleaseDC(hwnd, hdc);
                   n=0;
                  }
                  else n++;
            }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
