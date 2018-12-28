package cn.geobeans.earth;

public class ViewPoint {
	public String name;
	public double lon;
	public double lat;
	public double alt;
	public double heading;
	public double pitch;
	public double range;
	
	public ViewPoint(String name,double lon,double lat,double alt,double heading,double pitch,double range)
	{
		this.name = name;
		this.lon = lon;
		this.lat = lat;
		this.alt = alt;
		this.heading = heading;
		this.pitch = pitch;
		this.range = range;
	}
}
