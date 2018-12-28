#ifndef __GVCOORD_H__
#define __GVCOORD_H__

struct GVCoord 
{
	double lon;
	double lat;
	double alt;

	GVCoord() {	lat = 39.9;	lon = 116.3;	alt = 1000.0;};
	GVCoord(double lon, double lat, double alt) 
	{
		this->lon = lon;
		this->lat = lat;
		this->alt = alt;
	};
	bool equal(const GVCoord &c) const 
	{
		return c.lat == lat && c.lon == lon && c.alt == alt;
	};
	bool vaild()  const
	{
		if(lat > 90.0 || lat < -90.0)
		return false;
		return true;
	};

	GVCoord operator+(const GVCoord & coord) const 
	{
		double nlon = lon + coord.lon;
		double nlat = lat + coord.lat;
		double nalt = alt + coord.alt;
		while(nlon > 180.0)
			nlon -= 360.0;
		while(nlon < -180)
			nlon += 360.0;
		return GVCoord(nlon, nlat,nalt);
	};
		;
};






#endif // __GVCOORD_H__