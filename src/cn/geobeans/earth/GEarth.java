package cn.geobeans.earth;

import java.util.List;

//import android.app.Activity;
//import android.content.Intent;
//import android.os.Bundle;
//import android.view.View;

public class GEarth {
	
    public interface OnClickListener {
        void onEarthClick(double dlon,double dlat,double dalt);
        void onEarthPick(String layer, String id, int ty);
        void onEarthInit();
    }
    
	//static public Activity mainActivity;
    static public OnClickListener earthListener;
   /**
    * @param width the current view width
    * @param height the current view height
    */
    public static native void 		init(int width, int height);
    public static native void 		step();
    public static native void 		touchPicker(int touchid, float x,float y);
    public static native void 		touchPosition(int touchid, float x,float y);
    public static native void 		touchBeganEvent(int touchid, float x,float y);
    public static native void 		touchMovedEvent(int touchid, float x,float y);
    public static native void 		touchEndedEvent(int touchid, float x,float y, int tapcount);
    public static native void 		keyboardDown(int key);
    public static native void 		keyboardUp(int key);
    public static native void 		clearEventQueue();
    
    public static native void 		loadModel(String grp,String id,String address,double lon,double lat);
    public static native void 		setBaseLayerWMS(String url);
    public static native void 		setBaseLayerTMS(String url);
    public static native void 		setBaseLayerXYZ(String url);
    
    public static native void 		addLayer(String url, String layer, String style, int minLevel);
    public static native void 		removeLayer(String layer);
    
    public static native void		loadAsyncReq(String req);
    public static native Place 		addPlace(String grp,String id,String img,String txt,double lon,double lat);
    
    public static native void 		setOperation(int oper);
    public static native void 		setArmyID(int id);
    public static native void 		addVectorLayer(String name, String shp,int tp);
    public static native void 		setLineColor(String col);
    public static native void 		setFillColor(String col);
    public static native void 		setLineWidth(int width);
    public static native void 		setLineStyle(int pattern);
    public static native void 		setLineDash(int dash,int gap);
    public static native void 		setFontColor(String col);
    public static native void       removeVectorLayer(String name);

    public static native void       addTMSLayer(String strName,String strUrl);
    public static native void       removeTMSLayer(String strName);

    public static native void       addElevationLayer(String strName, String strUrl);
    public static native void       removeElevationLayer(String strName);
    
    public static native void       setPlacePositon(String grp,String id,double lon,double lat);
    public static native void       setPlaceText(String grp,String id,String txt);
    public static native void       setPlaceIcon(String grp,String id,String imgfile);
    public static native GeoPoint   getPlacePositon(String grp,String id);
    
    public static native void       setModelPositon(String grp,String id,double lon,double lat);
    public static native GeoPoint   getModelPositon(String grp,String id);
    public static native void       addArmySymbol(String grp,String id,int key,GeoPoint[] pnts);
    public static native void       addLine(String grp,String id,GeoPoint[] pnts);
    public static native void       addPolygon(String grp,String id,GeoPoint[] pnts);
    
    public static native GeoPoint   toMapPoint(int x,int y);
    public static native GeoPoint   fromMapPoint(double lon,double lat,double ele);
    public static native void       startEdit(String grp,String id);
    
    public static native void       setActiveLayer(String grp);
    public static native void       setDeclutter(String grp);
    
    public static native void       setViewPoint(String name,double lon,double lat,double alt,double heading,double pitch,double range,double duration);
    public static native ViewPoint  getViewPoint();
    
    public static void displayInfo(String layer, String id, int ty){
//    	System.out.println(lat + ";" + lon);
//    	earthListener.onEarthPick(layer, id, lat, lon, description);
//    	
//    	Intent intent = new Intent(mainActivity, PickerActivity.class);
//    	
//    	Bundle b = new Bundle();
//    	b.putString("layer", layer);
//    	b.putString("id", id);
//    	b.putDouble("lat", lat);
//    	b.putDouble("lon", lon);
//    	b.putString("description", description);
//    	
//    	intent.putExtras(b);
//    	
//    	mainActivity.startActivityForResult(intent, 1);
    	earthListener.onEarthPick(layer, id, ty);
    }
    
    public static void fireTapped(double lat, double lon,double dalt) {
    	earthListener.onEarthClick(lat, lon, dalt);
    }
    
    public static void fireInit() {
    	earthListener.onEarthInit();
    }
    
    public static void newAsyncReq(String req){
    	System.out.println(req);
    }
}
