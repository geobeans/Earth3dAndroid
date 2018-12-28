package osgearth.Example;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

import layers.ImageLayer;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import cn.geobeans.earth.EGLview;
import cn.geobeans.earth.GEarth;
import cn.geobeans.earth.GeoPoint;
import cn.geobeans.earth.Place;

public class osgViewer extends Activity implements View.OnKeyListener, OnSharedPreferenceChangeListener,GEarth.OnClickListener
{
	 enum Oper{
		AddNull,
		AddPlace,
		AddModel
	};
	
	Oper m_oper = Oper.AddNull;
	
	public SharedPreferences prefs;
	public TreeMap<String, ImageLayer> ilMap;
	
	private GVPTApp app;
	private static String TAG = "osgViewer";
	//Ui elements
    EGLview mView;
    Button uiCenterViewButton;
    Button uibtnDelLyr;
    Button uibtnDist;
    Button uibtnClearMeasure;
    Button uibtnArea;
    Button uibtnLos;
    Button uibtnRadial;
    Button uibtnArmy10;
    Button uibtnArmy13;
    Button uibtnArmy8;
    Button uibtnVecterLayer;
    Button uibtnPlace;
    Button uibtnLineColor;
    Button uibtnLineWidth;
    Button uibtnFillColor;
    Button uibtnModel;
    Button uibtnPlaceTxt;
    Button uibtnPlaceIcon;
    Button uibtnJbLine;
    Button uibtnLine;
    Button uibtnDeclutter;
    Button uibtnAddLine;
    Button uibtnLineStyle;
    Button uibtnLineDash;
    
    AlertDialog loadLayerAddress;
    FileBrowser fileDialog;
    
    GEarth nativeLib;
    Place _pla;
    //Main Android Activity life cycle
    @Override protected void onCreate(Bundle icicle) {
    	
    	String appPath = getApplication().getApplicationContext().getFilesDir().getAbsolutePath();
    	System.out.println(appPath);
    	Log.w("osgNativeLib", "About to load lib");
    	System.load(appPath+"/../lib/libradiEarthLib.so");
    	//System.loadLibrary("osgNativeLib");
    	Log.w("osgNativeLib", "Lib loaded");
    	
        super.onCreate(icicle);
        setContentView(R.layout.ui_layout_gles);
        
        app = (GVPTApp) getApplication();
        //GEarth.mainActivity = this;
        GEarth.earthListener = this;
        //get gl view
	    mView= (EGLview) findViewById(R.id.surfaceGLES);
		//mView.setOnTouchListener(this);
		mView.setOnKeyListener(this);
		
		prefs = PreferenceManager.getDefaultSharedPreferences(this.getApplicationContext());
		app.activeOverlays.put("over_arm", prefs.getBoolean("over_arm", false));
		app.activeOverlays.put("over_pos", prefs.getBoolean("over_pos", false));
		app.activeOverlays.put("over_ca", prefs.getBoolean("over_ca", false));
		prefs.registerOnSharedPreferenceChangeListener(this);
	    
		//get center camera button
	    uiCenterViewButton = (Button) findViewById(R.id.uiButtonCenter);
	    uiCenterViewButton.setOnClickListener(uiListenerCenterView);
	    
	    uibtnDelLyr = (Button) findViewById(R.id.uibtnDelLyr);
	    uibtnDelLyr.setOnClickListener(uiListenerDelLyr);
	    
	    uibtnDist = (Button) findViewById(R.id.uibtnDistance);
	    uibtnDist.setOnClickListener(uiListenerDistance);
	    
	    uibtnClearMeasure = (Button) findViewById(R.id.uiBtnClearMeasure);
	    uibtnClearMeasure.setOnClickListener(uiClearMeasure);
	    
	    uibtnArea = (Button) findViewById(R.id.uibtnArea);
	    uibtnArea.setOnClickListener(uiListenerArea);
	    
	    uibtnLos = (Button) findViewById(R.id.uibtnLos);
	    uibtnLos.setOnClickListener(uiListenerLos);
	    
	    uibtnRadial = (Button) findViewById(R.id.uibtnRadial);
	    uibtnRadial.setOnClickListener(uiListenerRadial);
	    
	    uibtnArmy10 = (Button) findViewById(R.id.uibtnArmy10);
	    uibtnArmy10.setOnClickListener(uiListenerArmy);
	    
	    uibtnArmy13 = (Button) findViewById(R.id.uibtnArmy13);
	    uibtnArmy13.setOnClickListener(uiListenerArmy13);
	    
	    uibtnArmy8 = (Button) findViewById(R.id.uibtnArmy8);
	    uibtnArmy8.setOnClickListener(uiListenerArmy8);
	    
	    uibtnPlace  = (Button) findViewById(R.id.uibtnPlace);
	    uibtnPlace.setOnClickListener(uiListenerPlace);
	    	    
	    uibtnVecterLayer = (Button) findViewById(R.id.uibtnVecterLayer);
	    uibtnVecterLayer.setOnClickListener(uiListenerVectorLayer);
	    
	    uibtnLineColor = (Button) findViewById(R.id.uibtnLineColor);
	    uibtnLineColor.setOnClickListener(uiListenerLineColor);
	    
	    uibtnLineWidth = (Button) findViewById(R.id.uibtnLineWidth);
	    uibtnLineWidth.setOnClickListener(uiListenerLineWidth);
	    
	    uibtnFillColor = (Button) findViewById(R.id.uibtnFillColor);
	    uibtnFillColor.setOnClickListener(uiListenerFillColor);
	    
	    uibtnModel = (Button) findViewById(R.id.uibtnModel);
	    uibtnModel.setOnClickListener(uiListenerModel);
	    
	    uibtnPlaceTxt = (Button) findViewById(R.id.uibtnPlaceTxt);
	    uibtnPlaceTxt.setOnClickListener(uiListenerPlaceTxt);
	    
	    uibtnPlaceIcon = (Button) findViewById(R.id.uibtnPlaceIcon);
	    uibtnPlaceIcon.setOnClickListener(uiListenerPlaceIcon);
	    
	    uibtnJbLine = (Button) findViewById(R.id.uibtnJbLine);
	    uibtnJbLine.setOnClickListener(uiListenerJBLine);
	    
	    uibtnLine = (Button) findViewById(R.id.uibtnLine);
	    uibtnLine.setOnClickListener(uiListenerLine);
	    
	    uibtnDeclutter = (Button) findViewById(R.id.uibtnDeclutter);
	    uibtnDeclutter.setOnClickListener(uiListenerDeclutter);
	    
	    uibtnAddLine = (Button) findViewById(R.id.uibtnAddLine);
	    uibtnAddLine.setOnClickListener(uiListenerAddLine);
	    
	    uibtnLineStyle = (Button) findViewById(R.id.uibtnLineStyle);
	    uibtnLineStyle.setOnClickListener(uiListenerLineStyle);
	    
	    uibtnLineDash = (Button) findViewById(R.id.uibtnLineDash);
	    uibtnLineDash.setOnClickListener(uiListenerLineDash);
	    
	    LayoutInflater factory = LayoutInflater.from(getApplicationContext());
	    final View textEntryView = factory.inflate(R.layout.dialog_text_entry, null);
	    AlertDialog.Builder loadLayerDialogBuilder = new AlertDialog.Builder(this);
	    loadLayerDialogBuilder.setIcon(R.drawable.web_browser);
	    loadLayerDialogBuilder.setTitle(R.string.uiDialogTextAddress);
	    loadLayerDialogBuilder.setView(textEntryView);
	    loadLayerDialogBuilder.setPositiveButton(R.string.uiDialogOk, new DialogInterface.OnClickListener() {

	    	@Override
	    	public void onClick(DialogInterface dialog, int which) {
	    		EditText address;
	    		address = (EditText) textEntryView.findViewById(R.id.uiEditTextInput);
	    		//osgNativeLib.loadModel(address.getText().toString());
	    	}
	    });
	    loadLayerDialogBuilder.setNegativeButton(R.string.uiDialogCancel, new DialogInterface.OnClickListener() {

	    	@Override
	    	public void onClick(DialogInterface dialog, int which) {
	    	}
	    });

	    loadLayerAddress = loadLayerDialogBuilder.create();
	    
	    addActiveLayers();
    }
    
    @Override 
    protected void onPause() {
        super.onPause();
        //mView.onPause();
    }
    
    @Override 
    protected void onResume() {
        super.onResume();
        //mView.onResume();
    }
    
    @Override
	public boolean onKey(View v, int keyCode, KeyEvent event) {
		return true;
	}
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event){
    	return true;
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event){
    	switch (keyCode){
    	case KeyEvent.KEYCODE_BACK:
    		super.onDestroy();
    		this.finish();
    		break;
    	case KeyEvent.KEYCODE_SEARCH:
    		break;
    	case KeyEvent.KEYCODE_MENU:
    		this.openOptionsMenu();
    		break;
    	default:
    		int keyChar = event.getUnicodeChar();
    		GEarth.keyboardDown(keyChar);
    		GEarth.keyboardUp(keyChar);    		
    	}
    	
    	return true;
    }
   
    //Ui Listeners
    OnClickListener uiListenerCenterView = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.keyboardDown(32);
        	GEarth.keyboardUp(32);
        }
    };
    
    OnClickListener uiListenerDelLyr = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.removeLayer("place");
        }
    };
    
    OnClickListener uiListenerDistance = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 1;
        	GEarth.setOperation(oper);
        }
    };

    OnClickListener uiClearMeasure = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.removeLayer("measure");
        }
    };
    
    OnClickListener uiListenerArea = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 2;
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerLos = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 3;
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerRadial = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 4;
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerArmy = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 5;
        	GEarth.setArmyID(10);
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerArmy13 = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 5;
        	GEarth.setArmyID(13);
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerArmy8 = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 5;
        	GEarth.setArmyID(8);
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerPlace = new OnClickListener() {
        public void onClick(View v) {
        	m_oper = Oper.AddPlace;
        }
    };
    
    OnClickListener uiListenerVectorLayer = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.addVectorLayer("railway", "/storage/emulated/0/shp/dt.shp",1);
        }
    };
    
    OnClickListener uiListenerLineColor = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setLineColor("#FFFF00B0");
        }
    };
    
    OnClickListener uiListenerLineWidth = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setLineWidth(4);
        }
    };
    
    OnClickListener uiListenerFillColor = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setFillColor("#FFFF00B0");
        }
    };
    
    OnClickListener uiListenerModel = new OnClickListener() {
        public void onClick(View v) {
        	m_oper = Oper.AddModel;
        }
    };
    
    OnClickListener uiListenerPlaceTxt = new OnClickListener() {
        public void onClick(View v) {
        	Place p = new Place("place","123456");
        	p.setLabel("Ìì½ò");
        }
    };
    
    OnClickListener uiListenerPlaceIcon = new OnClickListener() {
        public void onClick(View v) {
        	//Place p = new Place("place","123456");
        	_pla.setImage("/storage/emulated/0/ship.png");
        }
    };
        
    OnClickListener uiListenerJBLine = new OnClickListener() {
        public void onClick(View v) {
        	GeoPoint pnts[] = new GeoPoint[3];
        	pnts[0] = new GeoPoint(116.396,39.9,50,0);
        	pnts[1] = new GeoPoint(116.401,39.96,50,0);
        	pnts[2] = new GeoPoint(116.402,39.96,50,0);
        	//List<GeoPoint> lst = new ArrayList<GeoPoint>();
        	//lst.add(new GeoPoint(116.396,39.9,50,0));
        	//lst.add(new GeoPoint(116.401,39.96,50,0));
        	GEarth.addArmySymbol("army","001", 10, pnts);
        }
    };
    
    OnClickListener uiListenerLine = new OnClickListener() {
        public void onClick(View v) {
        	int oper = 6;
        	GEarth.setOperation(oper);
        }
    };
    
    OnClickListener uiListenerDeclutter = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setDeclutter("place");
        }
    };
    
    OnClickListener uiListenerAddLine = new OnClickListener() {
        public void onClick(View v) {
        	GeoPoint pnts[] = new GeoPoint[3];
        	pnts[0] = new GeoPoint(116.396,39.9,50,0);
        	pnts[1] = new GeoPoint(116.401,39.96,50,0);
        	pnts[2] = new GeoPoint(116.402,39.96,50,0);
        	//List<GeoPoint> lst = new ArrayList<GeoPoint>();
        	//lst.add(new GeoPoint(116.396,39.9,50,0));
        	//lst.add(new GeoPoint(116.401,39.96,50,0));
        	GEarth.addLine("graphics", "007", pnts);
        }
    };
        
    OnClickListener uiListenerLineStyle = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setLineStyle(0x1111);
        }
    };
    
    OnClickListener uiListenerLineDash = new OnClickListener() {
        public void onClick(View v) {
        	GEarth.setLineDash(200,100);
        }
    };
    
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.appmenu, menu);
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
	        case R.id.menuMyLocation:
	            return true;
	        case R.id.menuSettings:
	        	this.startActivity(new Intent(this, SettingsActivity.class));
	            return true;
	        case R.id.menuKML:
	        	File mPath = new File(Environment.getExternalStorageDirectory() + "//DIR//");
	        	fileDialog = new FileBrowser(this, mPath, ".kml");
	            fileDialog.addFileListener(new FileBrowser.FileSelectedListener() {
	                public void fileSelected(File file) {
	                    Log.d(getClass().getName(), "selected file " + file.toString());
	                    //osgNativeLib.loadModel(file.toString());
	                }
	            });
	            fileDialog.showDialog();
	        	return true;
	        case R.id.menuExit:
	    		this.finish();
	            return true;
	        default:
	            return super.onOptionsItemSelected(item);
        }
    }
    
    public void addActiveLayers(){
    	if(prefs.getBoolean("over_pos", false)){
    		GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms", "sig3d:SIG3D_POSTES",
    				"osg_poste", 14);
    	}

    	if(prefs.getBoolean("over_arm", false)){
    		GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms", "sig3d:SIG3D_ARMARIOS",
    				"osg_armario", 14);
    	}

    	if(prefs.getBoolean("over_ca", false)){
    		GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms",
    				"sig3d:SIG3D_TAEREOS", "", 14);
    	}
    }

	@Override
	public void onSharedPreferenceChanged(SharedPreferences sharedPrefs, String key)
	{
		if( key == null ) return;
		
		if( key.equals(GVPTApp.PREFERENCE_KEY_IMAGE_LAYERS) )
		{
			ImageLayer il = app.ilMap.get(prefs.getString(GVPTApp.PREFERENCE_KEY_IMAGE_LAYERS, ""));
			if( il != null )
			{
				if( il.getDriver().equals("wms") )
					GEarth.setBaseLayerWMS(il.getUrl());
				if( il.getDriver().equals("tms") )
					GEarth.setBaseLayerTMS(il.getUrl());
				if( il.getDriver().equals("xyz") )
					GEarth.setBaseLayerXYZ(il.getUrl());
			}
			System.out.println("change image layer to " + prefs.getString(GVPTApp.PREFERENCE_KEY_IMAGE_LAYERS, ""));
		}
		
		if( key.equals(GVPTApp.PREFERENCE_KEY_POSTES) )
		{
			if( prefs.getBoolean("over_pos", false) )
			{
				GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms", 
						"sig3d:SIG3D_POSTES", "osg_poste", 14);
			}
			else
			{
				GEarth.removeLayer("sig3d:SIG3D_POSTES");
			}
			app.activeOverlays.put("over_pos", prefs.getBoolean("over_pos", false));
		}
			
		if( key.equals(GVPTApp.PREFERENCE_KEY_ARMARIOS) )
		{
			if( prefs.getBoolean("over_arm", false) )
			{
				GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms", 
						"sig3d:SIG3D_ARMARIOS", "osg_armario", 14);
			}
			else
			{
				GEarth.removeLayer("sig3d:SIG3D_ARMARIOS");
			}
			app.activeOverlays.put("over_arm", prefs.getBoolean("over_arm", false));
		}
			
		if( key.equals(GVPTApp.PREFERENCE_KEY_CABOSA) )
		{
			if (prefs.getBoolean("over_ca", false) )
			{
				GEarth.addLayer("http://webgis.di.uminho.pt:8080/geoserver2/sig3d/wms", 
						"sig3d:SIG3D_TAEREOS", "", 14);
			}
			else
			{
				GEarth.removeLayer("sig3d:SIG3D_TAEREOS");
			}
			app.activeOverlays.put("over_ca", prefs.getBoolean("over_ca", false));
		}
	}

	@Override
	public void onEarthClick(double dlon, double dlat, double dalt) {
		// TODO Auto-generated method stub
		Log.d(TAG, "lon="+dlon+" lat="+dlat+" alt="+dalt);

		switch(m_oper) {
		case AddPlace:
			{
				_pla = GEarth.addPlace("place","123456", "/storage/emulated/0/placemark64.png", "±±¾©", dlon, dlat);
//				GeoPoint pnt  = GEarth.fromMapPoint(dlon, dlat, dalt);	
//				int x = (int)pnt.lon;
//				int y = (int)pnt.lat;
//				GeoPoint pntMap = GEarth.toMapPoint(x, y);
//				double lon = pntMap.lon;
			}
		    break;
		case AddModel:
			GEarth.loadModel("Model", "001","/storage/emulated/0/ifcf2.osg", dlon, dlat);
			//GEarth.loadModel("Model", "/storage/emulated/0/tree4.osgb.10,10,10.scale", dlon, dlat);
			break;
		}
		m_oper = Oper.AddNull;
	}

	@Override
	public void onEarthPick(String layer, String id,int tp) {
		// TODO Auto-generated method stub
    	System.out.println(layer + " " + id +" " + tp);	
	}
	
	@Override
	public void onEarthInit()
	{
		//GEarth.addVectorLayer("railway", "/storage/emulated/0/shp/dt.shp",1);
	}
}