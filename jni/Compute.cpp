#include "Compute.h"



double flattening = (RADIUSEQUATOR - RADIUSPOLAR) / RADIUSEQUATOR;
double eccentricitySquared = 2 * flattening - flattening*flattening;

// compute the midpoint from lonlat_radin;
osg::Vec3d getMidPoint( osg::Vec3d& p1_rad, osg::Vec3d& p2_rad)
{
	osg::Vec3d outmidp_rad;

	double dLon = (p2_rad.x() - p1_rad.x());

	double cosLat1 = cos(p1_rad.y());
	double cosLat2 = cos(p2_rad.y());
	double sinLat1 = sin(p1_rad.y());
	double sinLat2 = sin(p2_rad.y());

	double Bx = cosLat2 * cos(dLon);
	double By = cosLat2 * sin(dLon);

	outmidp_rad.y() = atan2(sinLat1 + sinLat2,
		sqrt((cosLat1 + Bx)*(cosLat1 + Bx) + By*By));
	outmidp_rad.x() = p1_rad.x() + atan2(By, cosLat1 + Bx);

	return outmidp_rad;
}

//convert rad_point to deg_point
osg::Vec3d convertRadinToDegrees(osg::Vec3d radinPoint)
{
	osg::Vec3d degreesPoint;
	degreesPoint.x() = osg::RadiansToDegrees(radinPoint.x());
	degreesPoint.y() = osg::RadiansToDegrees(radinPoint.y());
	//degreesPoint.z() = osg::RadiansToDegrees(radinPoint.z());

	return degreesPoint;

}

//convert deg_point to rad_point
osg::Vec3d convertDegreesToRadin(osg::Vec3d degPoint)
{
	osg::Vec3d radPoint;
	radPoint.x() = osg::DegreesToRadians(degPoint.x());
	radPoint.y() = osg::DegreesToRadians(degPoint.y());
	//radPoint.z() = osg::DegreesToRadians(degPoint.z());

	return radPoint;

}

//convert LongLatHeight to XYZ;
osg::Vec3d convertLongLatHeightToXYZ(const osg::Vec3d& longLatPoint)
{
	osg::Vec3d XYZpoint;

	double sin_latitude = sin(longLatPoint.y());
	double cos_latitude = cos(longLatPoint.y());
	double N = RADIUSEQUATOR / sqrt(1.0 - eccentricitySquared*sin_latitude*sin_latitude);

	XYZpoint.x() = (N + longLatPoint.z())*cos_latitude*cos(longLatPoint.x());
	XYZpoint.y() = (N + longLatPoint.z())*cos_latitude*sin(longLatPoint.x());
	XYZpoint.z() = (N*(1 - eccentricitySquared) + longLatPoint.z())*sin_latitude;

	return XYZpoint;
}


// it is about computing the curve;

Points::Points() :
	_precision		(1.0),
	_isXYZ			(false),
	_isHeight		(false),
	_isHigh_precision(false)
{
}

bool Points::buildSegment(std::vector<osg::Vec3d>& points)
{
	if (points.size() < 2)
	{
		return false;
	}
	else
	{
		_points = points;
		for (int i = 0;i < _points.size();i++)
		{
			_points[i] = convertDegreesToRadin(_points[i]);
		}
	}

}
void Points::setPrecision(double precision)
{
	_precision = precision;
}

void Points::setIsHigh_precision(bool value)
{	
	_isHigh_precision = value;
}

void Points::LODSegment()
{

	while (_points[1].x() - _points[0].x() >= _precision)
	{
		int n = _points.size();
		int j = 0;
		for (int i = 1;i < n;i++)
		{
			osg::Vec3d midPoint = getMidPoint(_points[j], _points[j + 1]);
			_points.insert((_points.begin() + j + 1), midPoint);

			j += 2;
		}
	}
}


double Points::getAllLength()
{
	double allLength = 0.0;
	for (int i = 1;i < _points.size();i++)
	{
		double XYZ_square = pow(_points[i].x() - _points[i - 1].x(),2) 
			+ pow(_points[i].y() - _points[i - 1].y(),2)
			+ pow(_points[i].z() - _points[i - 1].z(),2);
		allLength += sqrt(XYZ_square);
	}

	return allLength;
}

void Points::isXYZ()
{
	//将角度值转换为弧度值
	for (int i = 0; i < _points.size(); i++)
	{
		_points[i] = convertDegreesToRadin(_points[i]);
	}

	for (int i = 0;i < _points.size();i++)
	{
		_points[i] = convertLongLatHeightToXYZ(_points[i]);
	}
	_isXYZ = true;
}

void Points::isHeight(osgEarth::ElevationQuery& query, osgEarth::MapNode* mapNode)
{
	std::vector<double> elevations;

	for (int i = 0; i < _points.size(); i++)
	{
		_points[i] = convertRadinToDegrees(_points[i]);
	}

	if (!_isHigh_precision)
	{
		for (int i = 0; i < _points.size();i++)
		{
			double Height = 0.0;
			if (mapNode->getTerrain()->getHeight(mapNode->getMapSRS(), _points[i].x(), _points[i].y(), &Height))
			{
				_points[i].z() = Height;
			}
		}
		
	}
	else
	{
		if (query.getElevations(_points, mapNode->getTerrain()->getSRS(), elevations))
		{
			for (int i = 0; i < _points.size(); i++)
			{
				_points[i].z() = elevations[i];
			}
		}

	}

	_isHeight = true;
	
}


/*===========================================================================================================================
=============================================================================================================================
=============================================================================================================================*/

// it is about computing the polygon;
Triangles::Triangles():
	_precision			(2),
	_isXYZ				(false),
	_isHeight			(false)
{

}

bool Triangles::buildTriangles(const std::vector<osg::Vec3d>& points)
{
	if (points.size() < 3)
		return false;
	else 
	{
		
		for (int i = 1;i < (points.size() - 1); i++)
		{
			Triangle triangle = Triangle(convertDegreesToRadin(points[0]), convertDegreesToRadin(points[i]), convertDegreesToRadin(points[i+1]));

			_triangles.push_back(triangle);
		}

		return true;
	}
}

void Triangles::setPrecision(double precision)
{
	_precision = precision;
}

void Triangles::setIsHigh_precision(bool value)
{
	_isHigh_precision = value;
}

void Triangles::LODTriangles()
{

	for (int i = 0;i < _precision;i++)
	{
		int n = _triangles.size();
		int k = 0;
		for (int j = 0;j <n ;j++)
		{
			osg::Vec3d midp1, midp2, midp3;

			midp1 = getMidPoint(_triangles[k].p1, _triangles[k].p2);
			midp2 = getMidPoint(_triangles[k].p2, _triangles[k].p3);
			midp3 = getMidPoint(_triangles[k].p3, _triangles[k].p1);

			Triangle t1 = Triangle(_triangles[k].p1, midp1, midp3);
			Triangle t2 = Triangle(_triangles[k].p2, midp1, midp2);
			Triangle t3 = Triangle(_triangles[k].p3, midp2, midp3);

			_triangles.insert((_triangles.begin() + k + 1), t1);
			_triangles.insert((_triangles.begin() + k + 1), t2);
			_triangles.insert((_triangles.begin() + k + 1), t3);
			_triangles[k] = Triangle(midp1, midp2, midp3);

			k += 4;
		}
	}
}

double Triangles::getAllArea()
{
	double sumArea = 0.0;
	for (int i = 0;i < _triangles.size();i++)
	{
		sumArea += _triangles[i].computeArea();
	}

	return sumArea;
}

void Triangles::isXYZ()
{
	//将角度值转换为弧度值
	for (int i = 0; i < _triangles.size(); i++)
	{
		_triangles[i].p1 = convertDegreesToRadin(_triangles[i].p1);
		_triangles[i].p2 = convertDegreesToRadin(_triangles[i].p2);
		_triangles[i].p3 = convertDegreesToRadin(_triangles[i].p3);
	}


	for (int i = 0;i < _triangles.size();i++)
	{
		_triangles[i].p1 = convertLongLatHeightToXYZ(_triangles[i].p1);
		_triangles[i].p2 = convertLongLatHeightToXYZ(_triangles[i].p2);
		_triangles[i].p3 = convertLongLatHeightToXYZ(_triangles[i].p3);
	}

	_isXYZ = true;
}

void Triangles::isHeight(osgEarth::ElevationQuery& query, osgEarth::MapNode* mapNode)
{
	std::vector<double> elevations;
	std::vector<osg::Vec3d> points;

	//To prevent vector subscript out of range;
	osg::Vec3d a(1, 2, 3);
	points.push_back(a);
	points.push_back(a);
	points.push_back(a);

	//将弧度值转换为角度值
	for (int i = 0;i < _triangles.size();i++)
	{
		_triangles[i].p1 = convertRadinToDegrees(_triangles[i].p1);
		_triangles[i].p2 = convertRadinToDegrees(_triangles[i].p2);
		_triangles[i].p3 = convertRadinToDegrees(_triangles[i].p3);
	}

	for (int i = 0;i < _triangles.size();i++)
	{
		points[0] = _triangles[i].p1;
		points[1] = _triangles[i].p2;
		points[2] = _triangles[i].p3;

		if (!_isHigh_precision)
		{
			double height = 0.0;
			for (int j = 0; j < points.size(); j++)
			{
				if (mapNode->getTerrain()->getHeight(mapNode->getMapSRS(), points[j].x(), points[j].y(), &height))
				{
					points[j].z() = height;
				}
			}
			_triangles[i].p1.z() = points[0].z();
			_triangles[i].p2.z() = points[1].z();
			_triangles[i].p3.z() = points[2].z();
		}
		else
		{
			if (query.getElevations(points, mapNode->getTerrain()->getSRS(), elevations))
			{
				_triangles[i].p1.z() = elevations[elevations.size() - 3];
				_triangles[i].p2.z() = elevations[elevations.size() - 2];
				_triangles[i].p3.z() = elevations[elevations.size() - 1];
			}
		}
	}
	_isHeight = true;
}
