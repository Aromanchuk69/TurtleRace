#pragma once
#include "afx.h"
#include <vector>
#include "afxwin.h"

class CSpliner : public CObject
{
public:
	CSpliner();
	virtual ~CSpliner();

	
	void RemoveAll();					// Удаление всех точек	
	BOOL AddPoint(double X, double Y);	// Добавление новой точки	
	void RemoveAt(int Pos);				// Удаление точки в указанной позиции
	int GetSize();						// Справка о количестве заданных точек	
	double GetX(int Pos);				// Справка о координате X точки в указанной позиции
	double GetY(int Pos);				// Справка о координате Y точки в указанной позиции
	BOOL SplineIt();					// Сплайн-аппроксимация заданного набора точек
	double SplinedY(double X);			// Выдача сплайновой координаты Y в позиции X
	double Radius(double X);			// Выдача радиуса сплайна в позиции X
	double Uklon(double X);				// Выдача уклона сплайна в позиции X
	// Параметры сплайна
	double	B(int iPos) { return(points_[iPos].B); };
	double	C(int iPos) { return(points_[iPos].C); };
	double	D(int iPos) { return(points_[iPos].D); };
	void	FirstB(double fB);
	void	FirstC(double fC);
	void	FirstD(double fD);

private:
	class CSplinerBasePoint
	{
	public:
		CSplinerBasePoint() { X = Y = B = C = D = dL = Right = Ap = Bp = Radius = 0; };
		virtual ~CSplinerBasePoint() {};

		double	X;		//	Координата X сплайновой линии
		double	Y;		//	Координата Y сплайновой линии, она же - A
		double	B;		//	Параметр сплайна, он же - уклон 
		double	C;		//	Параметр сплайна
		double	D;		//	Параметр сплайна
		double	dL;
		double	Right;	//	Правая часть уравнения
		double	Ap;		//	Прогоночный коэффициент
		double	Bp;		//	Прогоночный коэффициент
		double	Radius;

	};

	std::vector<CSplinerBasePoint>	points_;
};

class B_Spliner
{
public:
	B_Spliner(void);
	~B_Spliner(void);

private:
	class BsPoint
	{
	public:
		BsPoint() { X = Y = T = Ax = Bx = Cx = Dx = Ay = By = Cy = Dy = 0; };
		~BsPoint() {};
		double	X;
		double	Y;
		double	T;

		double	Ax, Bx, Cx, Dx;
		double	Ay, By, Cy, Dy;
	};

	std::vector<BsPoint>	points_;
	std::vector<CPoint>		generated_track_;
public:
	void	clear()
	{
		generated_track_.clear();
		points_.clear();
	};

	bool get_step(CPoint& point)
	{
		if (!generated_track_.size())
			return false;

		point = *(generated_track_.begin());
		return true;
	};

	bool remove_point()
	{
		for (int i = 0; i < 4; i++)
		{
			if (!generated_track_.size())
				return false;

			generated_track_.erase(generated_track_.begin());
		}

		return true;
	};

	void draw(CDC* pDC)
	{
		std::vector<CPoint>::iterator it;
		if (generated_track_.size())
		{
			it = generated_track_.begin();
			pDC->MoveTo(it->x, it->y);

			for (it = generated_track_.begin(); it != generated_track_.end(); it++)
				pDC->LineTo(it->x, it->y);
		}
	}

	BOOL	add_point(double X, double Y);
	BOOL	spline_it(void);
	void	generate_track();
};
