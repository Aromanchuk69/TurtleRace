#include "BSpliner.h"

#include <math.h>

CSpliner::CSpliner()
{

}

CSpliner::~CSpliner()
{
	RemoveAll();
}

BOOL CSpliner::SplineIt()
{
	CSplinerBasePoint	P, Pn, Pp;
	int					i, n;

	n = int(points_.size());

	if (n < 3)
		return(0);

	// Приступаем к формированию параметров для прогонки
		// Заполняем A и dL
	for (i = 0; i < n - 1; i++)
	{
		P	= points_[i];
		Pn	= points_[i + 1];

		points_[i].dL = Pn.X - P.X;
	}

	Pn.dL = 0;

	// Заполняем Правые части
	for (i = 1; i < n - 1; i++)
	{
		P	=	points_[i];
		Pn	=	points_[i + 1];
		Pp	=	points_[i - 1];

		points_[i].Right = 3 * ((Pn.Y - P.Y) / P.dL - (P.Y - Pp.Y) / Pp.dL);
	}

	// Заполняем прогоночные коэффициенты
	P		= points_[1];
	Pn		= points_[2];
	Pp		= points_[0];
	points_[1].Ap	= -P.dL / 2 / (P.dL + Pp.dL);
	points_[1].Bp	= P.Right / 2 / (P.dL + Pp.dL);

	for (i = 2; i < n - 1; i++)
	{
		Pp	=	points_[i - 1];
		P	=	points_[i];
		Pn	=	points_[i + 1];
		double a, b, c, e;
		a	= Pp.dL;
		b	= 2 * (P.dL + Pp.dL);
		c	= P.dL;
		e	= a * Pp.Ap + b;
		points_[i].Ap = -c / e;
		points_[i].Bp = (P.Right - a * Pp.Bp) / e;
	}

	/////////////////////////////////////////////////////////////////
		// Все прогоночные коэффициенты найдены, определяем C
	Pp	= points_[n - 2];
	P	= points_[n - 1];
	points_[n - 1].C	= (P.Right - P.dL * Pp.Bp) /
		(2 * (Pp.dL + P.dL) + P.dL * Pp.Ap);

	for (i = n - 2; i > 0; i--)
	{
		P	= points_[i];
		Pn	= points_[i + 1];
		points_[i].C	= (P.Ap * Pn.C + P.Bp);
	}

	// C найдены, находим B, D
	P		= points_[n - 2];
	Pn		= points_[n - 1];
	points_[n - 1].D	= -P.C / (3 * P.dL);
	points_[n - 1].B	= (Pn.Y - P.Y) / P.dL - 2 / 3 * P.dL * P.C;

	for (i = 0; i < n - 1; i++)
	{
		P	= points_[i];
		Pn	= points_[i + 1];

		points_[i].D = (Pn.C - P.C) / (3 * P.dL);
		points_[i].B = (Pn.Y - P.Y) / P.dL - P.dL / 3
			* (Pn.C + 2 * P.C);
	}

	// Находим радиусы
	for (i = 0; i < n - 1; i++)
	{
		P = points_[i];
		if (P.C < 0.00001)
			points_[i].Radius = pow(1 + P.B * P.B, 1.5) / (2 * P.C);
		else
			points_[i].Radius = 0;
	}

	P		= points_[n - 2];
	Pn		= points_[n - 1];
	Pn.B	= P.B + 2 * P.C * P.dL + 3 * P.D * P.dL * P.dL;
	if (2 * P.C + 6 * P.D * P.dL < 0.00001)
		points_[n - 1].Radius = pow(1 + P.B * P.B, 1.5) / (2 * P.C + 6 * P.D * P.dL);
	else
		points_[n - 1].Radius = 0;

	return(1);
}

BOOL CSpliner::AddPoint(double X, double Y)
{
	CSplinerBasePoint point, new_point;
	INT_PTR					i, n;

	new_point.X = X;
	new_point.Y = Y;

	n = int(points_.size());
	for (i = 0; i < n; i++)
	{
		point = points_[i];

		if (point.X > X)
			break;

		if (point.X == X)
		{
			point.Y = Y;
			return(0);
		}
	}

	points_.push_back(new_point);

	return(1);
}

int CSpliner::GetSize()
{
	return((int)points_.size());
}

void CSpliner::RemoveAll()
{
	points_.clear();
}

void CSpliner::FirstB(double fB)
{
	if (!points_.size())
		return;

	points_[0].B = fB;
}

void CSpliner::FirstC(double fC)
{
	if (!points_.size())
		return;

	points_[0].C = fC;
}

void CSpliner::FirstD(double fD)
{
	if (!points_.size())
		return;

	points_[0].D = fD;
}

void CSpliner::RemoveAt(int Pos)
{
	if (points_.size() < Pos)
		return;

	std::vector<CSplinerBasePoint>::iterator	it = points_.begin() + Pos;
	points_.erase(it);
}

double CSpliner::SplinedY(double X)
{
	CSplinerBasePoint	P, Pn;
	INT_PTR				i, n;

	n = points_.size();

	if (n < 3)
		return(0);

	P = points_[0];
	if (X == P.X)
		return(P.Y);

	for (i = 1; i < n; i++)
	{
		Pn = points_[i];

		if (X == Pn.X)
			return(Pn.Y);

		if (Pn.X > X)
		{
			double L;
			L = X - P.X;
			return(P.Y + L * P.B + L * L * P.C + L * L * L * P.D);
		}

		P = Pn;
	}

	return(0);
}

double CSpliner::GetX(int Pos)
{
	return(points_[Pos].X);
}

double CSpliner::GetY(int Pos)
{
	return(points_[Pos].Y);
}

double CSpliner::Uklon(double X)
{
	CSplinerBasePoint	P, Pn;
	INT_PTR				i, n;
	double				L;

	n = int(points_.size());

	if (n < 3)
		return(0);

	P = points_[0];
	if (X == P.X)
		return(P.B);

	for (i = 1; i < n - 1; i++)
	{
		Pn = points_[i];

		if (X == Pn.X)
			return(Pn.B);

		if (Pn.X > X)
		{
			L = X - P.X;
			return(P.B + 2 * P.C * L + 3 * P.D * L * L);
		}

		P = Pn;
	}

	Pn = points_[n - 1];
	if (X > Pn.X)
		return(0);

	return(P.B + 2 * P.C * P.dL + 3 * P.D * P.dL * P.dL);
}

double CSpliner::Radius(double X)
{
	CSplinerBasePoint P, Pn;
	INT_PTR				i, n;
	double				L;

	n = int(points_.size());

	if (n < 3)
		return(0);

	P = points_[0];
	if (X == P.X)
		return(P.Radius);

	for (i = 1; i < n - 1; i++)
	{
		Pn = points_[i];

		if (X == Pn.X)
			return(Pn.Radius);

		if (Pn.X > X)
		{
			L = X - P.X;
			if (2 * P.C + 6 * P.D * L < 0.00001)
				return(pow(1 + P.B * P.B, 1.5) / (2 * P.C + 6 * P.D * L));
			else
				return(0);
		}

		P = Pn;
	}

	Pn = points_[n - 1];
	if (X > Pn.X)
		return(0);

	if (2 * P.C + 6 * P.D * P.dL < 0.00001)
		return(pow(1 + P.B * P.B, 1.5) / (2 * P.C + 6 * P.D * P.dL));
	else
		return(0);
}



/////////////////////////////////////////////

B_Spliner::B_Spliner(void)
{
}

B_Spliner::~B_Spliner(void)
{
}

BOOL B_Spliner::add_point(double X, double Y)
{
	if (points_.size() > 30000)
		return(1);

	BsPoint point;

	point.X = X;
	point.Y = Y;

	points_.push_back(point);

	spline_it();

	return(0);
}

void B_Spliner::generate_track()
{
	BsPoint Point, Prev;
	INT_PTR		i, n, iDT, iT;
	int			X, Y, X1, Y1;

	n = int(points_.size());
	if (!n)
		return;

	Point = Prev = points_[0];
	X1 = (int)Point.X;
	Y1 = (int)Point.Y;

	for (i = 1; i < n; i++)
	{
		Point = points_[i];

		if (n < 3)
		{
			X = (int)Point.X;
			Y = (int)Point.Y;

			CPoint	point(X, Y);
			generated_track_.push_back(point);
		}
		else
		{
			iDT = (int)(Point.T - Prev.T);

			for (iT = 1; iT < iDT; iT++)
			{
				X = (int)(X1 + Prev.Bx * iT + Prev.Cx * iT * iT + Prev.Dx * iT * iT * iT);
				Y = (int)(Y1 + Prev.By * iT + Prev.Cy * iT * iT + Prev.Dy * iT * iT * iT);

				CPoint	point(X, Y);
				generated_track_.push_back(point);
			}
		}

		Prev = Point;
		X1 = int(Point.X);
		Y1 = int(Point.Y);
	}
}

/*
void B_Spliner::Draw(CDC* pDC, BOOL bShowPoints)
{

	pDC->MoveTo((int)pPoint->X, (int)pPoint->Y);
	if (bShowPoints)
		DrawPoint(pDC, (int)pPoint->X, (int)pPoint->Y);
}
*/

BOOL B_Spliner::spline_it(void)
{
	CSpliner	splSplineX, splSplineY;
	BsPoint		Point, Prev;
	int			i, n;
	double		X1, Y1, T;

	n = int(points_.size());

	if (n < 3)
		return(1);

	//  Рассчитываем параметры T и заносим их в сплайнеры
	T = 0;
	Prev = Point = points_[0];
	X1 = (int)Prev.X;
	Y1 = (int)Prev.Y;
	for (i = 0; i < n; i++)
	{
		Prev = Point;
		Point = points_[i];
		T = T + sqrt((Point.X - Prev.X) * (Point.X - Prev.X) +
			(Point.Y - Prev.Y) * (Point.Y - Prev.Y));
		points_[i].T = T;
		splSplineX.AddPoint(T, Point.X);
		splSplineY.AddPoint(T, Point.Y);

	}

	if (Prev.X == Point.X && Prev.Y == Point.Y)
		return(0);

	splSplineX.SplineIt();
	splSplineY.SplineIt();

	for (i = 0; i < n; i++)
	{
		Point = points_[i];

		points_[i].Bx = splSplineX.B(i);
		points_[i].By = splSplineY.B(i);
		points_[i].Cx = splSplineX.C(i);
		points_[i].Cy = splSplineY.C(i);
		points_[i].Dx = splSplineX.D(i);
		points_[i].Dy = splSplineY.D(i);
	}

	return 0;
}
