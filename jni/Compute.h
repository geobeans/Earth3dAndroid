#include <iostream>
#include <vector>
#include <osg/Vec3d>
#include <math.h>

#include <osgEarth/GeoData>
#include <osgEarth/MapNode>
#include <osgEarth/Terrain>
#include <osgEarth/Map>
#include <osgEarth/ElevationQuery>




#define RADIUSEQUATOR   6378137.0
#define RADIUSPOLAR      6356752.3142  

class Triangle{
public:
	osg::Vec3d p1;
	osg::Vec3d p2;
	osg::Vec3d p3;

	Triangle() {}

	Triangle(osg::Vec3d a,osg::Vec3d b,osg::Vec3d c) 
	{
		p1 = a;
		p2 = b;
		p3 = c;
	}
	double computeArea()
	{
		double side[3];

		side[0] = sqrt((pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2) + pow(p1.z() - p2.z(), 2)));
		side[1] = sqrt((pow(p2.x() - p3.x(), 2) + pow(p2.y() - p3.y(), 2) + pow(p2.z() - p3.z(), 2)));
		side[2] = sqrt((pow(p3.x() - p1.x(), 2) + pow(p3.y() - p1.y(), 2) + pow(p3.z() - p1.z(), 2)));

		double p = (side[0] + side[1] + side[2]) / 2;

		double area = sqrt(p*(p - side[0])*(p - side[1])*(p - side[2]));

		return area;
	}
	
};

typedef std::vector<Triangle> TRIANGLEVECTOR;

using namespace std;

class Points {
public:
	Points();

	~Points(){ }

	bool buildSegment(std::vector<osg::Vec3d>& points);

	void setPrecision(double precision);

	std::vector<osg::Vec3d> getPoints() const { return _points; }

	void LODSegment();

	double getAllLength();

	void isXYZ();

	void isHeight(osgEarth::ElevationQuery& query,osgEarth::MapNode* mapNode);

	bool getIsXYZ() const { return _isXYZ; }

	bool getIsHeight() const { return _isHeight; }

	bool getIsHigh_precision() const { return _isHigh_precision; }

	void setIsHigh_precision(bool value);

protected:
	std::vector<osg::Vec3d> _points;
	double					_precision;
	bool					_isHigh_precision;
	bool					_isXYZ;
	bool					_isHeight;
	
};

class Triangles {
public:
	Triangles();

	~Triangles(){ }

	void setPrecision(double precision);

	bool buildTriangles(const std::vector<osg::Vec3d>& points);

	TRIANGLEVECTOR getTriangles() const { return _triangles; }
	
	void LODTriangles();

	double getAllArea();

	void isXYZ();

	void isHeight(osgEarth::ElevationQuery& query, osgEarth::MapNode* mapNode);

	bool getIsXYZ() const { return _isXYZ; }

	bool getIsHeight() const { return _isHeight; }

	bool getIsHigh_precision() const { return _isHigh_precision; }

	void setIsHigh_precision(bool value);

protected:
	TRIANGLEVECTOR	_triangles;
	double			_precision;
	bool			_isXYZ;
	bool			_isHeight;
	bool			_isHigh_precision;
};

