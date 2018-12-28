#ifndef GEOMAINAPP_HPP_
#define GEOMAINAPP_HPP_

//Android log
#include <android/log.h>
#include <GeoScene.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

//Standard libraries
#include <string>

//osg
#include <osgViewer/Viewer>

#include <jni.h>


//Static plugins Macro
#include "osgPlugins.h"

//demoscene

#define  LOG_TAG    "osgNativeLib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)



class GeoMainApp{
private:
    
    osg::ref_ptr<GeoScene> _scene;
    int _bufferWidth;
    int _bufferHeight;
    bool _initialized;
    
    //OsgAndroidNotifyHandler *_notifyHandler;

    //events for each touch phase, these are cleared each frame
    osg::ref_ptr<osgGA::GUIEventAdapter> _frameTouchBeganEvents;
    osg::ref_ptr<osgGA::GUIEventAdapter> _frameTouchMovedEvents;
    osg::ref_ptr<osgGA::GUIEventAdapter> _frameTouchEndedEvents;
    
public:
    GeoMainApp();
    ~GeoMainApp();

    //Initialization function
    void initOsgWindow(int x,int y,int width,int height);
    //Draw
    void draw();
    //Events
    void touchPicker(JNIEnv * env,int touchid,float x,float y);
    void touchPosition(JNIEnv * env,int touchid,float x,float y);
    void touchBeganEvent(int touchid,float x,float y);
    void touchMovedEvent(int touchid,float x,float y);
    void touchEndedEvent(int touchid,float x,float y,int tapcount=0);
    void keyboardDown(int key);
    void keyboardUp(int key);
    void clearEventQueue();

    //Operation
    void setOperation(int oper);
    int getOperation();

    void addLine(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls);
    void addPolygon(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls);
    void setLineColor(std::string col);
    void setFillColor(std::string col);
    void setLineWidth(int width);
    void setFontColor(std::string col);
    void setLineStyle(unsigned short pattern);
    void setLineDash(int dash,int gap);

    void setArmyID(int iID);

    void loadModel(std::string grp,std::string id,std::string address,double lon,double lat);
    void addPlace(std::string grp,std::string id,std::string img,std::string txt,double lon,double lat);
    void addArmyLine(std::string grp,std::string id,int key,std::vector<GVCoord> &vtxCtrls);

    void setActiveLayer(std::string strLyr);

    void setBaseLayerWMS(std::string url);
    void setBaseLayerTMS(std::string url);
    void setBaseLayerXYZ(std::string url);

    void addLayer(std::string url, std::string layer, std::string style, int minLevel);
    void removeLayer(std::string layer);

    void addVectorLayer(std::string name,std::string shp,int tp);
	void removeVectorLayer(std::string name);

	void addTMSLayer(std::string strName,std::string strUrl);
	void removeTMSLayer(std::string strName);

	void addElevationLayer(std::string strName, std::string strUrl);
	void removeElevationLayer(std::string strName);

    void setPlacePositon(std::string grp,std::string id,double lon,double lat);
	void setPlaceText(std::string grp,std::string id,std::string txt);
	void setPlaceIcon(std::string grp,std::string id,std::string imgfile);
    osgEarth::GeoPoint getPlacePosition(std::string grp,std::string id);

    void setModelPositon(std::string grp,std::string id,osgEarth::GeoPoint pnt);
    osgEarth::GeoPoint getModelPosition(std::string grp,std::string id);

    void setJavaEnv(JNIEnv * env);
    void setGlobalJObj(jobject jo);

	bool toMapPoint(int x, int y, double &dlon, double &dlat, double &dele);
	bool fromMapPoint(double dlon, double dlat, double dele, int &x, int &y);

	void startEdit(std::string grp,std::string id);
	void setDeclutter(std::string grp);
    JNIEnv * _genv;
    jobject _gjo;

};


#endif /* GEOMAINAPP_HPP_ */
