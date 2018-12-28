package cn.geobeans.earth;

public final class GeoPoint {
	public double lon;
	public double lat;
	public double alt;
	public int    altMode;
	
	public GeoPoint(double lon,double lat,double alt,int altMode)
	{
		this.lon = lon;
		this.lat = lat;
		this.alt = alt;
		this.altMode = altMode;
	}
}
