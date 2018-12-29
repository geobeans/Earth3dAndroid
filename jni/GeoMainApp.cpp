#include <GeoMainApp.hpp>

GeoMainApp::GeoMainApp(){

    _initialized = false;

}
GeoMainApp::~GeoMainApp(){

}

void GeoMainApp::setJavaEnv(JNIEnv * env){
	_genv = env;
}

void GeoMainApp::setGlobalJObj(jobject jo){
	_gjo = jo;
}

//Initialization function
void GeoMainApp::initOsgWindow(int x,int y,int width,int height){

    __android_log_write(ANDROID_LOG_ERROR, "OSGANDROID",
            "Initializing geometry");
    //

    _scene = new GeoScene(_genv, _gjo);
    _scene->init("", osg::Vec2(width, height), 0);

    _initialized = true;

}

//Draw
void GeoMainApp::draw(){

    _scene->frame();
    
    //clear events for next frame
    _frameTouchBeganEvents = NULL;
    _frameTouchMovedEvents = NULL;
    _frameTouchEndedEvents = NULL;
}

void GeoMainApp::loadModel(std::string grp,std::string id,std::string address,double lon,double lat){
	const osgEarth::SpatialReference* latLong = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint pnt(latLong, lon, lat);
	_scene->addModel(grp,id,address,pnt);
}

void GeoMainApp::addPlace(std::string grp,std::string id,std::string img,std::string txt,double lon,double lat){
	const osgEarth::SpatialReference* latLong = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint pnt(latLong, lon, lat);
	_scene->addPlace( grp,id, img, txt, pnt);
}

void GeoMainApp::setBaseLayerWMS(std::string url){
	_scene->setBaseLayerWMS(url);
}

void GeoMainApp::setBaseLayerTMS(std::string url){
	_scene->setBaseLayerTMS(url);
}

void GeoMainApp::setBaseLayerXYZ(std::string url){
	_scene->setBaseLayerXYZ(url);
}

void GeoMainApp::addLayer(std::string url, std::string layer, std::string style, int minLevel){
	_scene->addLayer(url, layer, style, minLevel);
}

void GeoMainApp::removeLayer(std::string layer){
	_scene->removeLayer(layer);
}

//Events
static bool flipy = true;

void GeoMainApp::touchPicker(JNIEnv * env,int touchid,float x,float y){
	if(_scene->getViewer()){
		_scene->getViewer()->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Control_L);
		_scene->getViewer()->getEventQueue()->mouseButtonPress(x, y, 1);
		_scene->getViewer()->getEventQueue()->mouseButtonRelease(x, y, 1);
		_scene->getViewer()->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Control_L);
	}
	_scene->onDoubleTap(env, x, y);
}

void GeoMainApp::touchPosition(JNIEnv * env,int touchid,float x,float y){
	if(_scene->getViewer()){
		//_scene->onMove(x, y); lijp modified
		_scene->onTap(env,x, y);
	}
}

void GeoMainApp::touchBeganEvent(int touchid,float x,float y){
    if (!_frameTouchBeganEvents.valid()) {
        if(_scene->getViewer()){
            _frameTouchBeganEvents = _scene->getViewer()->getEventQueue()->touchBegan(touchid,
            		osgGA::GUIEventAdapter::TOUCH_BEGAN, x, flipy ? _bufferHeight-y : y);
        }
    } else {
        _frameTouchBeganEvents->addTouchPoint(touchid, osgGA::GUIEventAdapter::TOUCH_BEGAN, x, flipy ? _bufferHeight-y : y);
    }
}
void GeoMainApp::touchMovedEvent(int touchid,float x,float y){
    if (!_frameTouchMovedEvents.valid()) {
        if(_scene->getViewer()){
            _frameTouchMovedEvents = _scene->getViewer()->getEventQueue()->touchMoved(touchid,
            		osgGA::GUIEventAdapter::TOUCH_MOVED, x, flipy ? _bufferHeight-y : y);
        }
    } else {
        _frameTouchMovedEvents->addTouchPoint(touchid, osgGA::GUIEventAdapter::TOUCH_MOVED, x, flipy ? _bufferHeight-y : y);
    }
}
void GeoMainApp::touchEndedEvent(int touchid,float x,float y,int tapcount){
    if (!_frameTouchEndedEvents.valid()) {
        if(_scene->getViewer()){
            _frameTouchEndedEvents = _scene->getViewer()->getEventQueue()->touchEnded(touchid,
            		osgGA::GUIEventAdapter::TOUCH_ENDED, x, flipy ? _bufferHeight-y : y,tapcount);
        }
    } else {
        _frameTouchEndedEvents->addTouchPoint(touchid, osgGA::GUIEventAdapter::TOUCH_ENDED, x, flipy ? _bufferHeight-y : y,tapcount);
    }
}
void GeoMainApp::keyboardDown(int key){
    _scene->getViewer()->getEventQueue()->keyPress(key);
}
void GeoMainApp::keyboardUp(int key){
    _scene->getViewer()->getEventQueue()->keyRelease(key);
}

void GeoMainApp::clearEventQueue()
{
    //clear our groups
    _frameTouchBeganEvents = NULL;
    _frameTouchMovedEvents = NULL;
    _frameTouchEndedEvents = NULL;
    
    //clear the viewers queue
    _scene->getViewer()->getEventQueue()->clear();
}

void GeoMainApp::setArmyID(int iID)
{
	_scene->setArmyID(iID);
}

void GeoMainApp::setOperation(int oper)
{
	_scene->setOperation( oper);
}

int GeoMainApp::getOperation()
{
    return _scene->getOperation();
}

void GeoMainApp::addVectorLayer(std::string name,std::string shp,int tp)
{
	_scene->addVectorLayer(name, shp,tp);
}

void GeoMainApp::removeVectorLayer(std::string name)
{
	_scene->removeVectorLayer(name);
}

void GeoMainApp::addTMSLayer(std::string strName,std::string strUrl)
{
	_scene->addTMSLayer(strName,strUrl);
}
void GeoMainApp::removeTMSLayer(std::string strName)
{
	_scene->removeTMSLayer(strName);
}

void GeoMainApp::addElevationLayer(std::string strName, std::string strUrl)
{
	_scene->addElevationLayer(strName,strUrl);
}
void GeoMainApp::removeElevationLayer(std::string strName)
{
	_scene->removeElevationLayer(strName);
}

void GeoMainApp::setLineColor(std::string col)
{
	_scene->setLineColor(col);
}

void GeoMainApp::setFillColor(std::string col)
{
	_scene->setFillColor(col);
}

void GeoMainApp::setLineWidth(int width)
{
	_scene->setLineWidth(width);
}

void GeoMainApp::setFontColor(std::string col)
{
	_scene->setFontColor(col);
}

void GeoMainApp::setPlacePositon(std::string grp,std::string id,double lon,double lat)
{
	const osgEarth::SpatialReference* latLong = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint pnt(latLong, lon, lat);
	_scene->setPlacePositon(grp,id,pnt);
}

void GeoMainApp::setPlaceText(std::string grp,std::string id,std::string txt)
{
	_scene->setPlaceText(grp,id,txt);
}

void GeoMainApp::setPlaceIcon(std::string grp,std::string id,std::string imgfile)
{
	osg::Image* img = osgDB::readImageFile(imgfile);
	_scene->setPlaceIcon(grp,id,img);
}

osgEarth::GeoPoint GeoMainApp::getPlacePosition(std::string grp,std::string id)
{
	_scene->getPlacePosition(grp, id);
}

void GeoMainApp::setModelPositon(std::string grp,std::string id,osgEarth::GeoPoint pnt)
{
	_scene->setPlacePositon(grp,id,pnt);
}

osgEarth::GeoPoint GeoMainApp::getModelPosition(std::string grp,std::string id)
{
	_scene->getPlacePosition(grp, id);
}

void GeoMainApp::addArmyLine(std::string grp,std::string id,int key,std::vector<GVCoord> &vtxCtrls)
{
	_scene->addArmyLine(grp,id,key,vtxCtrls);
}

void GeoMainApp::setActiveLayer(std::string strLyr)
{
	_scene->setActiveLayer(strLyr);
}

void GeoMainApp::addLine(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls)
{
	_scene->addLine(grp, id, vtxCtrls);
}

void GeoMainApp::addPolygon(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls)
{
	_scene->addPolygon(grp, id, vtxCtrls);
}

bool GeoMainApp::toMapPoint(int x, int y, double &dlon, double &dlat, double &dele)
{
	_scene->toMapPoint(x, y, dlon, dlat, dele);
}

bool GeoMainApp::fromMapPoint(double dlon, double dlat, double dele, int &x, int &y)
{
    _scene->fromMapPoint(dlon, dlat, dele, x, y);
}

void GeoMainApp::startEdit(std::string grp,std::string id)
{
	 _scene->startEdit(grp,id);
}

void GeoMainApp::setDeclutter(std::string grp)
{
	_scene->setDeclutter(grp);
}

void GeoMainApp::setLineStyle(unsigned short pattern)
{
	_scene->setLineStyle(pattern);
}

void GeoMainApp::setLineDash(int dash,int gap)
{
	_scene->setLineDash(dash,gap);
}

void GeoMainApp::setViewPoint(const osgEarth::Viewpoint &v,double duration)
{
	_scene->setViewPoint(v, duration);
}

osgEarth::Viewpoint GeoMainApp::getViewpoint()
{
	return _scene->getViewpoint();
}
