#pragma once
#include <vector>
#include <gdiplus.h>


namespace my_graphics
{
	class Plot :  public CStatic
	{
		//DECLARE_DYNAMIC(Graph)

		Gdiplus::GdiplusStartupInput input;
		ULONG_PTR token;

	protected:
		std::vector<std::vector<Gdiplus::PointF>> points;

		Gdiplus::Font* font1;
		Gdiplus::Font* font2;
		Gdiplus::Font* font3;

		Gdiplus::Pen* gridPen;
		Gdiplus::Pen* mainGridPen;

		Gdiplus::Pen* graphPen1;
		Gdiplus::Pen* graphPen2;
		Gdiplus::Pen* graphPen3;
		Gdiplus::Pen* graphPen4;
		Gdiplus::Pen* graphPen5;

		Gdiplus::StringFormat* stringformat;

		Gdiplus::Matrix* transform_matrix;

		void render(Gdiplus::Graphics& graphics, Gdiplus::Rect rect);

		virtual void render_grid(Gdiplus::Graphics& graphics);
		virtual void render_main(Gdiplus::Graphics& graphics, void* program = nullptr);
		virtual void render_main_grid(Gdiplus::Graphics& graphics);
		virtual void render_after_main(Gdiplus::Graphics& graphics, Gdiplus::Rect rect);

		virtual void render_captions(Gdiplus::Graphics& graphics, Gdiplus::Rect rect);
		virtual void render_title(Gdiplus::Graphics& graphics, Gdiplus::Rect rect);

	public:
		struct accommodate_settings
		{
			int create_matrix : 1;
			int accommodate_plot_bounds : 1;
			int accommodate_grid_scale : 1;
			int x_axis_symmetry : 1;
			int y_axis_symmetry : 1;

			enum plot_bounds_detect
			{
				log,
				linear,
				max_min
			} bounds_detection;
		} acc_settings;

		void accommodate();
		void accommodate(accommodate_settings settings);

		template	<typename InputIt>
		void add_points(InputIt begin, InputIt end, float width = 1, float left = 0);

		template	<typename InputIt>
		void add_points(std::initializer_list<std::pair<InputIt,InputIt>> list, float width = 1, float left = 0);

		template	<typename InputIt>
		void set_points(InputIt begin, InputIt end, float width = 1, float left = 0);

		template	<typename InputIt>
		void set_points(std::initializer_list<std::pair<InputIt, InputIt>> list, float width = 1, float left = 0);


		Gdiplus::REAL offsetX = 30;
		Gdiplus::REAL offsetY = 30;

		Gdiplus::RectF worldRect = { 0,-1,2,2 };

		Gdiplus::REAL grid_step_x = 1.0;
		Gdiplus::REAL grid_step_y = 1.0;

		unsigned grid_count_x = 10;
		unsigned grid_count_y = 10;

		CString title;
		CString captionX;
		CString captionY;

		void* program = nullptr;

		Plot();
		virtual ~Plot();
		virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
	};



	// Useful methods
	template<typename T>
	static T CeilMantissa(T value)
	{
		int sign = value > 0 ? 1.0 : -1.0;
		value = abs(value);
		if (value < FLT_EPSILON) return 0.0;
		return sign * pow(10, floor(log10(value))) * ceil(pow(10, log10(value) - floor(log10(value))));
	}
	template<typename T>
	static T FloorMantissa(T value)
	{
		int sign = value > 0 ? 1.0 : -1.0;
		value = abs(value);
		if (value < FLT_EPSILON) return 0.0;
		return sign * pow(10, floor(log10(value))) * floor(pow(10, log10(value) - floor(log10(value))));
	}
	template<typename T>
	static T GetMantissa(T value)
	{
		int sign = value > 0 ? 1.0 : -1.0;
		value = abs(value);
		if (value < FLT_EPSILON) return 0.0;
		return sign * pow(10, log10(value) - floor(log10(value)));
	}
	template <typename T>
	static T Get10thPower(T value)
	{
		value = abs(value);
		if (value < FLT_EPSILON) return 0.0;
		return  pow(10, floor(log10(value)));
	}

	template<typename InputIt, typename T = InputIt::value_type>
	static T CeilMantissa(InputIt begin, InputIt end)
	{
		T fmax = *(std::max_element(begin, end));
		T fmin = *(std::min_element(begin, end));
		T fextr = (abs(fmax) > abs(fmin)) ? fmax : fmin;
		return CeilMantissa(fextr);
	}

	template<typename InputIt, typename T = InputIt::value_type>
	static T FloorMantissa(InputIt begin, InputIt end)
	{
		T fmax = *(std::max_element(begin, end));
		T fmin = *(std::min_element(begin, end));
		T fextr = (abs(fmax) > abs(fmin)) ? fmax : fmin;
		return FloorMantissa(fextr);
	}


	static void DeflateRect(Gdiplus::Rect& rect, Gdiplus::REAL offset_x, Gdiplus::REAL offset_y)
	{
		rect.X += offset_x; rect.Y += offset_y; rect.Width -= offset_x * 2; rect.Height -= offset_y * 2;
	};

	template<typename InputIt>
	inline void Plot::add_points(InputIt begin, InputIt end, float width, float left)
	{
		std::vector<Gdiplus::PointF> p(end - begin);
		for (auto it = begin; it != end; it++)
		{
			p[it - begin] = { left + (it - begin) * width / (end - begin),(float)*it };
		}

		points.emplace_back(p);
	}
	template<typename InputIt>
	inline void Plot::add_points(std::initializer_list<std::pair<InputIt, InputIt>> list, float width, float left)
	{
		for (auto& i : list)
		{
			add_points(i.first, i.second, width, left);
		}
	}
	template<typename InputIt>
	inline void Plot::set_points(InputIt begin, InputIt end, float width, float left)
	{
		points = std::vector<std::vector<Gdiplus::PointF>>(
			1,
			std::vector<Gdiplus::PointF>(end-begin)
		);
		//points[0].resize(end - begin);

		for (auto it = begin; it != end; it++)
		{
			points[0][it - begin] = { left + (it - begin) * width / (end - begin),(float)*it };
		}
	}
	template<typename InputIt>
	inline void Plot::set_points(std::initializer_list<std::pair<InputIt, InputIt>> list, float width, float left)
	{
		if (list.size() > 0) set_points(list.begin()->first, list.begin()->first, width, left);
		if (list.size() > 1) 
			for (auto it = list.begin() + 1; it != list.end(); it++)
			{
				add_points(it->first, it->second, width, left);
			}
	}
}


