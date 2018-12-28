package cn.geobeans.earth;

public class Model {
	private String _layer;
	private String _id;
	
	private Model()
	{
		
	}
	
	public Model(String lyr,String id)
	{
		this._layer = lyr;
		this._id = id;
	}
	//public String getName() { return _id; }
	
	public String getLayer()
	{
		return _layer;
	}
	
	public String getName()
	{
		return _id;
	}

	//public double getLongitude() { return _lon; }

	public void setGeographicPositon(double lon,double lat)
	{ 
		GEarth.setModelPositon(_layer, _id, lon, lat);
	}
	
	public GeoPoint getGeographicPositon()
	{ 
		return GEarth.getModelPositon(_layer, _id);
	}
}
