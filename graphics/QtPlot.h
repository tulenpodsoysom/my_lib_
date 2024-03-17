#ifndef MYQTPLOT_H
#define MYQTPLOT_H

//#pragma comment(lib,"gdiplus.lib")

#include <QWidget>
#include <QPainter>
#include <graphics/PlotImage.hpp>

// poorly made gdiplus plot port

class MyQtPlot : public my_graphics::PlotImage, public QWidget
{
public:
    MyQtPlot(QWidget* d = nullptr) : my_graphics::PlotImage(),QWidget((QWidget*)d){
        //QWidget::paintEngine();
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_NativeWindow, true);
    }
    bool do_animate = false;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event)
    {

        using namespace Gdiplus;

        auto a = this->geometry();//this->geometry();
        a.translate(-a.left(),-a.top());
        Rect outerrect{ a.left(), a.top(), a.right() - a.left(), a.bottom() - a.top() };
        Rect rect = outerrect; my_graphics::DeflateRect(rect, offsetX, offsetY);
        if (!do_animate) accommodate(outerrect);

        HWND hwnd = (HWND)this->winId();
        HDC hdc = GetDC(hwnd);

        Graphics graphicsDC(hdc);
        Bitmap bitmap(outerrect.Width, outerrect.Height);
        Graphics graphics(&bitmap);

        my_graphics::PlotImage::render(graphics, rect);

        graphicsDC.DrawImage(&bitmap, outerrect);
        ReleaseDC(hwnd, hdc);
    };

    // QPaintDevice interface
public:
    QPaintEngine *paintEngine() const
    {
        return nullptr;
    }

    // QObject interface
public:
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::Paint) { paintEvent((QPaintEvent*)event);return true;}
        if (event->type() == QEvent::UpdateRequest) { paintEvent((QPaintEvent*)event); return true;}
        return QObject::event(event);
    }
};



#endif // MYQTPLOT_H
