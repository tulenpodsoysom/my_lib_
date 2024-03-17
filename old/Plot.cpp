#include "pch.h"
#include "Plot.h"

#include <algorithm>
#undef max

using namespace Gdiplus;


void my_graphics::Plot::render(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.Clear(Color(255, 255, 255));



	render_grid(graphics);

	graphics.SetTransform(transform_matrix);
	render_main(graphics);
	graphics.ResetTransform();

	render_main_grid(graphics);
	render_title(graphics, rect); 
	render_captions(graphics, rect);
	render_after_main(graphics, rect);
}

void my_graphics::Plot::render_grid(Gdiplus::Graphics& graphics)
{
	SolidBrush brush(Color(0, 0, 0));

	for (size_t i = 0; i < worldRect.Width / grid_step_x; i++)
	{
		PointF p1 = { worldRect.X + (REAL)i * grid_step_x,worldRect.Y };
		PointF p2 = { worldRect.X + (REAL)i * grid_step_x,worldRect.Y + worldRect.Height };
		CString caption; caption.Format(L"%g", p1.X);

		transform_matrix->TransformPoints(&p1);
		transform_matrix->TransformPoints(&p2);

		graphics.DrawLine(gridPen, p1, p2);

		RectF r = { PointF{ p1.X - offsetX, p1.Y }, SizeF{ 2 * offsetX, offsetY } };
		graphics.DrawString(caption.GetString(), caption.GetLength(), font1, r, stringformat, &brush);
	}

	for (size_t i = 0; i < worldRect.Height / grid_step_y; i++)
	{
		PointF p1 = { worldRect.X,                    worldRect.Y + (REAL)i * grid_step_y };
		PointF p2 = { worldRect.X + worldRect.Width , worldRect.Y + (REAL)i * grid_step_y };
		CString caption; caption.Format(L"%g", p1.Y);

		transform_matrix->TransformPoints(&p1);
		transform_matrix->TransformPoints(&p2);

		graphics.DrawLine(gridPen, p1, p2);

		RectF r = { PointF{ 0, p1.Y - 30 }, SizeF{ offsetX, 30 * 2 } };
		graphics.DrawString(caption.GetString(), caption.GetLength(), font1, r, stringformat, &brush);
	}
}

void my_graphics::Plot::render_main(Gdiplus::Graphics& graphics,void* program)
{
	if (!points.empty())
		for (size_t var = 0; var < points.size(); ++var)
		{
			if (points[var].size() == 0) continue;
			Pen* graphPen;
			switch (var) {
			case 0: {graphPen = graphPen1; break; }
			case 1: {graphPen = graphPen2; break; }
			case 2: {graphPen = graphPen3; break; }
			case 3: {graphPen = graphPen4; break; }
			case 4: {graphPen = graphPen5; break; }
			default: {continue; break; }
			}

			graphics.DrawCurve(graphPen, points[var].data(), points[var].size(), 0.00f);
		}
}

void my_graphics::Plot::render_main_grid(Gdiplus::Graphics& graphics)
{
	PointF xl{ worldRect.X, 0 }, xr{ worldRect.X + worldRect.Width, 0 };
	PointF yb{ 0, worldRect.Y }, yt{ 0, worldRect.Y + worldRect.Height };

	transform_matrix->TransformPoints(&xl); transform_matrix->TransformPoints(&xr);
	transform_matrix->TransformPoints(&yb); transform_matrix->TransformPoints(&yt);

	graphics.DrawLine(mainGridPen, xl, xr);
	graphics.DrawLine(mainGridPen, yb, yt);
}

void my_graphics::Plot::render_after_main(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
}

void my_graphics::Plot::render_captions(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
	SolidBrush brush(Color(0, 0, 0));
	graphics.TranslateTransform(offsetX, offsetY);
	graphics.DrawString(captionY.GetString(), captionY.GetLength(), font2, PointF{ 0.0f, 0.0f }, &brush);
	graphics.DrawString(captionX.GetString(), captionX.GetLength(), font2, PointF{ (REAL)rect.Width, (REAL)rect.Height / 2.0f }, &brush);
	graphics.ResetTransform();
}

void my_graphics::Plot::render_title(Gdiplus::Graphics& graphics, Gdiplus::Rect rect)
{
	SolidBrush brush(Color(0, 0, 0));
	graphics.TranslateTransform(offsetX, offsetY);
	graphics.DrawString(title.GetString(), title.GetLength(), font3, RectF{ PointF{0.0f,-offsetY },SizeF{(REAL)rect.Width,offsetY} }, stringformat, &brush);
	graphics.ResetTransform();
}

void my_graphics::Plot::accommodate()
{
	accommodate(acc_settings);
}

void my_graphics::Plot::accommodate(accommodate_settings settings)
{
	CRect rect_; 
	this->GetClientRect(&rect_);

	Rect outerrect = { rect_.left,rect_.top,rect_.Width(),rect_.Height() };
	Rect rect = outerrect;
	DeflateRect(rect, offsetX, offsetY);


	if (settings.accommodate_plot_bounds)
	{



		std::vector<float> max_top(points.size());
		std::vector<float> min_bottom(points.size());
		std::vector<float> min_lefts(points.size());
		std::vector<float> max_rights(points.size());

		for (size_t i = 0; i < points.size(); i++)
		{
			auto a = std::minmax_element(points[i].begin(), points[i].end(), [](auto& a, auto& b) {return a.Y < b.Y; });
			min_bottom[i] = a.first->Y;
			max_top[i] = a.second->Y;

			min_lefts[i] = points[i].begin()->X;
			max_rights[i] = points[i].rbegin()->X;
		}
		float min_y = *std::max_element(min_bottom.begin(), min_bottom.end(), [](auto& a, auto& b) {return abs(a) < abs(b); });
		float max_y = *std::max_element(max_top.begin(), max_top.end(), [](auto& a, auto& b) {return abs(a) < abs(b); });

		float min_x = *std::max_element(min_lefts.begin(), min_lefts.end(), [](auto& a, auto& b) {return abs(a) < abs(b); });
		float max_x = *std::max_element(max_rights.begin(), max_rights.end(), [](auto& a, auto& b) {return abs(a) < abs(b); });

		switch (settings.bounds_detection)
		{
		case accommodate_settings::linear:
			min_y = floor(min_y);
			max_y = ceil(max_y);

			min_x = floor(min_x);
			max_x = ceil(max_x);
			break;
		case accommodate_settings::log:
			min_y = (min_y > 0) ? FloorMantissa(min_y) : CeilMantissa(min_y);
			max_y = (max_y > 0) ? CeilMantissa(max_y) : FloorMantissa(max_y);

			min_x = (min_x > 0) ? FloorMantissa(min_x) : CeilMantissa(min_x);
			max_x = (max_x > 0) ? CeilMantissa(max_x) : FloorMantissa(max_x);
			break;
		case accommodate_settings::max_min:
			//min_y = (min_y > 0) ? FloorMantissa(min_y) : CeilMantissa(min_y);
			//max_y = (max_y > 0) ? CeilMantissa(max_y) : FloorMantissa(max_y);
			//
			//min_x = (min_x > 0) ? FloorMantissa(min_x) : CeilMantissa(min_x);
			//max_x = (max_x > 0) ? CeilMantissa(max_x) : FloorMantissa(max_x);
			break;
		}






		if (settings.x_axis_symmetry) { max_x = std::max(max_x, min_x); min_x = -max_x; }
		if (settings.y_axis_symmetry) { max_y = std::max(max_y, min_y); min_y = -max_y; }
		
		worldRect = {
			min_x,
			min_y,
			max_x - min_x,
			max_y - min_y
		};
	}

	if (settings.accommodate_grid_scale)
	{
		grid_step_x = Get10thPower(std::max(worldRect.GetLeft(), worldRect.GetRight()));
		grid_step_y = Get10thPower(std::max(worldRect.GetTop(), worldRect.GetBottom()));
	}
	else
	{
		grid_step_x = worldRect.Width / grid_count_x;
		grid_step_y = worldRect.Height / grid_count_y;
	}

	if (settings.create_matrix)
	{
		transform_matrix->Reset();
		transform_matrix->Translate(0.5 * outerrect.Width, 0.5 * outerrect.Height);
		transform_matrix->Scale((REAL)rect.Width / worldRect.Width, -(REAL)rect.Height / worldRect.Height);
		transform_matrix->Translate(-(worldRect.X + worldRect.Width / 2.0), -(worldRect.Y + worldRect.Height / 2.0));
	}
}

my_graphics::Plot::Plot()
{
	Status s = GdiplusStartup(&token, &input, NULL);
	if (s != Status::Ok) { MessageBox(L"Failed to initialize GDI+", L"Error"); throw; }

	font1 = new Gdiplus::Font{ L"Arial", 9, Gdiplus::FontStyleRegular };
	font2 = new Gdiplus::Font{ L"Arial", 11, Gdiplus::FontStyleRegular };
	font3 = new Gdiplus::Font{ L"Arial", 11, Gdiplus::FontStyleItalic };


	gridPen = new Gdiplus::Pen{ Gdiplus::Color(128, 128, 128), 0.005 };
	mainGridPen = new Gdiplus::Pen{ Gdiplus::Color(0, 0, 0), 0.01 };


	graphPen1 = new Gdiplus::Pen{ Gdiplus::Color(255, 0, 0), 0.001 };
	graphPen2 = new Gdiplus::Pen{ Gdiplus::Color(0, 0, 255), 0.001 };
	graphPen3 = new Gdiplus::Pen{ Gdiplus::Color(0, 255, 0), 0.001 };
	graphPen4 = new Gdiplus::Pen{ Gdiplus::Color(255, 255, 0), 0.001 };
	graphPen5 = new Gdiplus::Pen{ Gdiplus::Color(0x1f, 0x3a, 0x3d), 0.001 };

	stringformat = new StringFormat();
	stringformat->SetAlignment(StringAlignmentCenter);
	stringformat->SetLineAlignment(StringAlignmentCenter);

	acc_settings = {};
	acc_settings.create_matrix				= true;
	acc_settings.accommodate_grid_scale		= true;
	acc_settings.accommodate_plot_bounds	= true;
	acc_settings.x_axis_symmetry			= false;
	acc_settings.y_axis_symmetry			= false;
	acc_settings.bounds_detection = accommodate_settings::plot_bounds_detect::log;

	transform_matrix = new Matrix();
}

my_graphics::Plot::~Plot()
{
	delete transform_matrix;
	GdiplusShutdown(token);
}


void my_graphics::Plot::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	auto& a = lpDrawItemStruct->rcItem;
	Rect outerrect{ a.left, a.top, a.right - a.left, a.bottom - a.top };
	Rect rect = outerrect; DeflateRect(rect, offsetX, offsetY);

	Graphics graphicsDC(lpDrawItemStruct->hDC);
	Bitmap bitmap(outerrect.Width, outerrect.Height, &graphicsDC);
	Graphics graphics(&bitmap);

	render(graphics,rect);

	graphicsDC.DrawImage(&bitmap, outerrect);
}
BEGIN_MESSAGE_MAP(my_graphics::Plot, CStatic)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void my_graphics::Plot::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	accommodate();
	// TODO: добавьте свой код обработчика сообщений
}
