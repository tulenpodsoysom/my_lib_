#include <graphics/PlotImage.hpp>

class MFCPlot : public my_graphics::PlotImage, public CStatic
{
public:
	MFCPlot() : my_graphics::PlotImage(), CStatic() {}

	// Does some sort of optimization
	bool do_animate = false;

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{	
		using namespace Gdiplus;

		auto& a = lpDrawItemStruct->rcItem;
		Rect outerrect{ a.left, a.top, a.right - a.left, a.bottom - a.top };
		Rect rect = outerrect; my_graphics::DeflateRect(rect, offsetX, offsetY);
		if (!do_animate) accommodate(outerrect);	

		Graphics graphicsDC(lpDrawItemStruct->hDC);
		Bitmap bitmap(outerrect.Width, outerrect.Height);
		Graphics graphics(&bitmap);

		render(graphics, rect);
		

		graphicsDC.DrawImage(&bitmap, outerrect);
	}
};