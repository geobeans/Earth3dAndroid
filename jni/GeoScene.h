#pragma once

#include <OpenThreads/ReadWriteMutex>
#include "osgPlugins.h"

#include <osgViewer/Viewer>

#include <osgDB/FileUtils>

#include <osgEarth/Viewpoint>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ObjectLocator>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgEarthDrivers/kml/KML>
#include <osgEarthDrivers/wms/SceneGraphChanges>

#include <osgEarthAnnotation/TrackNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthUtil/LinearLineOfSight>
#include <osgEarthUtil/RadialLineOfSight>

#include "EarthMultiTouchManipulator.h"
#include "GLES2ShaderGenVisitor.h"

#include "GVCoord.h"

#include <jni.h>

#ifndef ANDROID
#import <UIKit/UIKit.h>//to acces app delegate
#else
typedef void UIView;
#endif

class GeoScene : public osg::Referenced
{
public:
    GeoScene(JNIEnv * env, jobject gjo);
    
    void init(const std::string& file, osg::Vec2 viewSize, UIView* view=0);
    
    void frame();
    
    //return the view
    osgViewer::Viewer* getViewer(){
        return _viewer.get();
    }
    
    void setOperation(int oper);
    int getOperation();

    void setArmyID(int iID);

    void addModel(std::string grp,std::string id,std::string address,osgEarth::GeoPoint pnt);
    void addPlace(std::string grp,std::string id,std::string img,std::string txt,osgEarth::GeoPoint pnt);
    void addArmyLine(std::string grp,std::string id,int key,std::vector<GVCoord> &vtxCtrls);

    void setBaseLayerWMS(std::string url);
    void setBaseLayerTMS(std::string url);
    void setBaseLayerXYZ(std::string url);
    void addLayer(std::string url, std::string layer, std::string style, int minLevel);
    void removeLayer(std::string name);

    void addVectorLayer(std::string name,std::string shp,int tp);
	void removeVectorLayer(std::string name);

	void addTMSLayer(std::string strName,std::string strUrl);
	void removeTMSLayer(std::string strName);

	void addElevationLayer(std::string strName, std::string strUrl);
	void removeElevationLayer(std::string strName);

    void onMove(float x, float y);
    void onTap( JNIEnv * env,float x, float y );
    void onDoubleTap( JNIEnv * env,float x, float y );
    void addUpdateOper(osg::ref_ptr< osg::Operation > refOperation);

    void addLine(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls);
    void addPolygon(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls);
    void setLineColor(std::string col);
    void setFillColor(std::string col);
    void setLineWidth(int width);
    void setLineStyle(unsigned short pattern);
    void setLineDash(int dash,int gap);
    void setFontColor(std::string col);

    void setPlacePositon(std::string grp,std::string id,osgEarth::GeoPoint pnt);
    void setPlaceText(std::string grp,std::string id,std::string txt);
    void setPlaceIcon(std::string grp,std::string id,osg::Image* img);
    osgEarth::GeoPoint getPlacePosition(std::string grp,std::string id);

    void setActiveLayer(std::string strLyr);

    void setModelPositon(std::string grp,std::string id,osgEarth::GeoPoint pnt);
    osgEarth::GeoPoint getModelPosition(std::string grp,std::string id);

    osg::Matrixd* getWorldCoords( osg::Node* node );
    
	bool toMapPoint(int x, int y, double &dlon, double &dlat, double &dele);
	bool fromMapPoint(double dlon, double dlat, double dele, int &x, int &y);

	void setViewPoint(const osgEarth::Viewpoint &v,double duration);
	osgEarth::Viewpoint getViewpoint();

	void setDeclutter(std::string grp);
	void startEdit(std::string grp,std::string id);

    osgEarth::Util::MapNode* getMapNode();
    JNIEnv * _genv;
    jobject _gjo;
    int     _oper;

protected:
    virtual ~GeoScene();
    
    //
    // implemented by new file in each target so we can recycle
    // all the base code for each app
    void initDemo(const std::string& file);
    void addDemoLine(osgEarth::Map* map);
    void buildLine(std::string strGrp);
    void buildPolygon(std::string strGrp);
    void buildLineOfSight(osgEarth::GeoPoint start,osgEarth::GeoPoint end);
    void buildRadialLineOfSight(osgEarth::GeoPoint center);
    void buildArmyLine(int key);
    osg::Node* drawJB(int key, std::vector<GVCoord>& vtxCtrls,osgEarth::Symbology::Style geomStyle, bool isFill);
    void addCompass();
    osg::MatrixTransform* createCompassPart(const std::string& image, float radius, float height);
    void setLineDashStyle(osg::Node* nd);
protected:
    
    osg::ref_ptr<osgViewer::Viewer> _viewer;
    osgEarth::Util::EarthManipulator* _manip;
    osgUtil::GLES2ShaderGenVisitor* _gles2ShaderGen;
    osg::ref_ptr<osgEarth::MapNode> _mapNode;
    osgEarth::Drivers::KMLOptions* _kml_options;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::Group> _editors;
    SceneGraphChanges* _changesQueue;

    osg::ref_ptr< osgEarth::Annotation::FeatureNode > _featureNode;
    osg::ref_ptr< osgEarth::Features::Feature >  _feature;
    osg::ref_ptr< osgEarth::Annotation::FeatureNode > _featureNodeP;
    osg::ref_ptr< osgEarth::Features::Feature >  _featureP;
    osg::ref_ptr< osgEarth::Annotation::PlaceNode> _place;
    osg::ref_ptr< osgEarth::Util::LinearLineOfSightNode > _lineofSight;
    osg::ref_ptr< osgEarth::Util::RadialLineOfSightNode > _radialLineofSight;
    osg::ref_ptr< osg::Node > _armyLine;
    bool _gotFirstLocation;
    bool _finished;

    OpenThreads::ReadWriteMutex m_sceneMutex;

    std::vector<GVCoord> m_vtxCtrls;
    int m_iArmyID;

    std::string m_strLineColor;
    std::string m_strFillColor;
    int m_iLineWidth;
    int m_iLineDash;
    int m_iLineGap;
    std::string m_strFontColor;
    std::string m_strActiveGrp;
    osgEarth::optional<unsigned short> m_lineStyle;

    std::map<std::string,std::map<std::string,osg::Node*> > _annObjs;
};


