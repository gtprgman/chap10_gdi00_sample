#pragma once
namespace _special_gdi_routines {
	struct _gdi_routines {

		_gdi_routines() {}

		_gdi_routines(HDC gContext, HINSTANCE mInst) : _mHDC(gContext), _mInst(mInst), _mLogPen( {} ), _mLogBrush( {} ),
			_mHWND(WindowFromDC(gContext)), _mCOLORef(RGB(0, 0, 0)),oldGDI(NULL),defGDIPen(NULL), defGDIBrush(NULL),defGDIOBJ(NULL) {
			
			defGDIPen = GetCurrentObject(_mHDC, OBJ_PEN);
			defGDIBrush = GetCurrentObject(_mHDC, OBJ_BRUSH);
			defGDIOBJ = GetCurrentObject(_mHDC, OBJ_DC);

			_mLogPen.lopnColor = _mCOLORef;
			_mLogPen.lopnStyle = PS_SOLID;
			_mLogPen.lopnWidth = POINT{ 1,0 };

			_mLogBrush.lbStyle = BS_SOLID;
			_mLogBrush.lbColor = _mCOLORef;
			_mLogBrush.lbHatch = 0;
		}

		~_gdi_routines() { SelectObject(_mHDC, defGDIOBJ); ReleaseDC(_mHWND, _mHDC); DeleteObject(_mPen);
		DeleteObject(_mHBRUSH); DeleteObject(oldGDI); DeleteObject(defGDIPen); DeleteObject(defGDIBrush);
		DeleteObject(defGDIOBJ);
		}

		BOOL free_draw(int _x, int _y, COLORREF _color) {
			return SetPixel(_mHDC, _x, _y, _color); 
		}

		BOOL draw_rect(int _xTop, int _yTop, int _xBottom, int _yBottom) {
			return Rectangle(_mHDC, _xTop, _yTop, _xBottom, _yBottom);
		}

		BOOL draw_arc(int _xTop, int _yTop, int _xBottom, int _yBottom, int _xStartArc, int _yStartArc, int _xEndArc, int _yEndArc) {
			return Arc(_mHDC, _xTop, _yTop, _xBottom, _yBottom, _xStartArc, _yStartArc, _xEndArc, _yEndArc);
		}

		BOOL draw_ellipse(int _xTopLeft, int _yTopLeft, int _xBottomRight, int _yBottomRight) {
			return Ellipse(_mHDC, _xTopLeft, _yTopLeft, _xBottomRight, _yBottomRight);
		}

		BOOL draw_elliptic_arc(int _xLeft, int _yTop, int _xBottomLeft, int _yBottomPoint, int _xRad1, int _yRad1,
			int _xRad2, int _yRad2) {
			return ArcTo(_mHDC, _xLeft, _yTop, _xBottomLeft, _yBottomPoint, _xRad1, _yRad1, _xRad2, _yRad2);
		}

		BOOL draw_poly_arc(POINT const* _lpPoints, BYTE const* _lpByte, int _ptCounts) {
			return PolyDraw(_mHDC, _lpPoints, _lpByte, _ptCounts);
		}

		BOOL draw_poly(const POINT* _lpPoints, INT32 _pointsNum) {
			return PolylineTo(_mHDC, _lpPoints, _pointsNum);
		}

		BOOL draw_poly_bezier(POINT const* _lpPoints, INT32 _ptsNum) {
			return PolyBezier(_mHDC, _lpPoints, _ptsNum);
		}


		int render_gdi_text(LPCTSTR _lpStr, int _nCount, LPRECT _lpRect, UINT _uFormat) {
			return DrawText(_mHDC, _lpStr, _nCount, _lpRect, _uFormat);
		}

		BOOL plot_gdi_text(int _xStart, int _yStart, LPCTSTR _lpStr, int _nLen) {
			return TextOut(_mHDC, _xStart, _yStart, _lpStr, _nLen);
		}

		HDC const& GetInternalDC() const { return _mHDC; }

		COLORREF const& GetInternalColorScheme() const { return _mCOLORef; }

		void SetPen(int _unWidth) {
			if (_mHBRUSH) DeleteObject(_mHBRUSH);
			oldGDI = SelectObject(_mHDC, defGDIOBJ); 
			oldGDI = SelectObject(_mHDC, defGDIPen); 
			_mPen = NewPen(_unWidth);
			oldGDI = SelectObject(_mHDC, _mPen);
			UpdateWindow(_mHWND);
		}

		void SetBrush() {
			if (_mPen)DeleteObject(_mPen);
			oldGDI = SelectObject(_mHDC, defGDIOBJ); 
			oldGDI = SelectObject(_mHDC, defGDIBrush); 
			_mHBRUSH = NewBrush();
			oldGDI = SelectObject(_mHDC, _mHBRUSH);
			UpdateWindow(_mHWND);
		}

		void SetColorSketch(COLORREF _uColor) {
			_mCOLORef = _uColor;
		}

		void SetPenStyle(LOGPEN _uPenType) { _mLogPen = {};  _mLogPen = _uPenType; }

		void SetBrushStyle(LOGBRUSH _uBrushType) {
			_mLogBrush = {};
			_mLogBrush = _uBrushType;
		}

		void SetHandle(HWND _uHandle) { _mHWND = _uHandle;  }

		void SetHDC(HDC _uHDC) {
			_mHDC = _uHDC;
		}

		void SetInstance(HINSTANCE _uInstance) { _mInst = _uInstance;  }

		
		
	protected:
		HBRUSH NewBrush() {
			return CreateBrushIndirect(&_mLogBrush);
		}

		HPEN NewPen(int _unWidth) {
			_mLogPen.lopnWidth = { _unWidth,0 };
			_mLogPen.lopnColor = _mCOLORef;
			return CreatePenIndirect(&_mLogPen);
		}

	private:
		HDC _mHDC;
		HPEN _mPen;
		HINSTANCE _mInst;
		HGDIOBJ oldGDI,defGDIPen, defGDIBrush,defGDIOBJ;
		HWND _mHWND;
		HBRUSH _mHBRUSH;
		COLORREF _mCOLORef;
		LOGPEN _mLogPen;
		LOGBRUSH _mLogBrush;
	};
};
