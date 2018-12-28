package cn.geobeans.earth;



public class Place {
	private String _layer;
	private String _id;
	
	private Place()
	{
		
	}
	
	public Place(String lyr,String id)
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

	public void setLabel(String lbl)
	{ 
		GEarth.setPlaceText(_layer, _id, lbl); 
	}
	
	//public String getImage() { return _img; }

	public void setImage(String img)
	{ 
		GEarth.setPlaceIcon(_layer, _id,img); 
	}
	
	//public double getLongitude() { return _lon; }

	public void setGeographicPositon(double lon,double lat)
	{ 
		GEarth.setPlacePositon(_layer, _id, lon, lat);
	}
	
	public GeoPoint getGeographicPositon()
	{ 
		return GEarth.getPlacePositon(_layer, _id);
	}
		
}
