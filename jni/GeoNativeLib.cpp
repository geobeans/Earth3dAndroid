#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <GeoMainApp.hpp>

#include <iostream>


GeoMainApp mainApp;

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    __android_log_write(ANDROID_LOG_ERROR, "OSGANDROID",
            "Entered JNI_OnLoad");

    //mainApp.setJavaVM(vm);

	return JNI_VERSION_1_6;
}

extern "C" {
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchPicker(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchPosition(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchBeganEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchMovedEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchEndedEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y, jint tapcount);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_keyboardDown(JNIEnv * env, jobject obj, jint key);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_keyboardUp(JNIEnv * env, jobject obj, jint key);
    
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_clearEventQueue(JNIEnv * env, jobject obj);
    
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_loadModel(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring address,jdouble lon,jdouble lat);
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_addPlace(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring img,jstring txt,jdouble lon,jdouble lat);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerWMS(JNIEnv * env, jobject obj, jstring url);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerTMS(JNIEnv * env, jobject obj, jstring url);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerXYZ(JNIEnv * env, jobject obj, jstring url);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addLayer(JNIEnv * env, jobject obj, jstring url,
    		jstring layer, jstring style, jint minLevel);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeLayer(JNIEnv * env, jobject obj, jstring layer);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setOperation(JNIEnv * env, jobject obj, jint oper);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setArmyID(JNIEnv * env, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addVectorLayer(JNIEnv * env, jobject obj, jstring name, jstring shp,jint tp);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineColor(JNIEnv * env, jobject obj, jstring col);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setFillColor(JNIEnv * env, jobject obj, jstring col);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineWidth(JNIEnv * env, jobject obj, jint width);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineStyle(JNIEnv * env, jobject obj, jint pattern);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineDash(JNIEnv * env, jobject obj, jint dash,jint gap);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setFontColor(JNIEnv * env, jobject obj, jstring col);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeVectorLayer(JNIEnv * env, jobject obj, jstring name);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addTMSLayer(JNIEnv * env, jobject obj, jstring strName,jstring strUrl);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeTMSLayer(JNIEnv * env, jobject obj, jstring strName);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addElevationLayer(JNIEnv * env, jobject obj, jstring strName, jstring strUrl);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeElevationLayer(JNIEnv * env, jobject obj, jstring strName);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlacePositon(JNIEnv * env, jobject obj, jstring grp,jstring id,jdouble lon,jdouble lat);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlaceText(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring txt);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlaceIcon(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring imgfile);
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getPlacePosition(JNIEnv * env, jobject obj, jstring grp,jstring id);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setModelPositon(JNIEnv * env, jobject obj, jstring grp,jstring id,jdouble lon,jdouble lat);
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getModelPosition(JNIEnv * env, jobject obj, jstring grp,jstring id);

    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addArmySymbol(JNIEnv * env, jobject obj, jstring grp,jstring id,jint key,jobjectArray pnts);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setActiveLayer(JNIEnv * env, jobject obj, jstring grp);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addLine(JNIEnv * env, jobject obj, jstring grp,jstring id,jobjectArray pnts);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addPolygon(JNIEnv * env, jobject obj, jstring grp,jstring id,jobjectArray pnts);
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_toMapPoint(JNIEnv * env, jobject obj, jint x,jint y);
    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_fromMapPoint(JNIEnv * env, jobject obj, jdouble lon,jdouble lat,jdouble ele);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_startEdit(JNIEnv * env, jobject obj, jstring grp,jstring id);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setDeclutter(JNIEnv * env, jobject obj, jstring grp);

    JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getViewPoint(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setViewPoint(JNIEnv * env, jobject obj, jstring name,jdouble lon,jdouble lat,jdouble alt,jdouble heading,jdouble pitch,jdouble range,jdouble duration);

};

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_init(JNIEnv * env, jobject obj, jint width, jint height){
	jobject jo = env->NewGlobalRef(obj);
	mainApp.setGlobalJObj(jo);
	mainApp.setJavaEnv(env);
    mainApp.initOsgWindow(0,0,width,height);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setOperation(JNIEnv * env, jobject obj, jint oper)
{
	mainApp.setOperation(oper);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setArmyID(JNIEnv * env, jobject obj, jint id)
{
	mainApp.setArmyID(id);
}


JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_step(JNIEnv * env, jobject obj){
    mainApp.draw();
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchPicker(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y){
    mainApp.touchPicker(env,touchid,x,y);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchPosition(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y){
	mainApp.touchPosition(env,touchid,x,y);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchBeganEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y){
    mainApp.touchBeganEvent(touchid,x,y);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchMovedEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y){
    mainApp.touchMovedEvent(touchid,x,y);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_touchEndedEvent(JNIEnv * env, jobject obj, jint touchid, jfloat x, jfloat y, jint tapcount){
    mainApp.touchEndedEvent(touchid,x,y,tapcount);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_keyboardDown(JNIEnv * env, jobject obj, jint key){
    mainApp.keyboardDown(key);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_keyboardUp(JNIEnv * env, jobject obj, jint key){
    mainApp.keyboardUp(key);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_clearEventQueue(JNIEnv * env, jobject obj)
{
    mainApp.clearEventQueue();
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_loadModel(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring address,jdouble lon,jdouble lat)
{
	//Import Strings from JNI
	const char *nativegrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeAddress = env->GetStringUTFChars(address, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
	mainApp.loadModel(std::string(nativegrp),std::string(nativeID),std::string(nativeAddress),lon,lat);

	jclass cls = env->FindClass("cn/geobeans/earth/Model");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

	jobject paramOut = env->NewObject(cls, mid, grp,id);
	//Release Strings to JNI
	env->ReleaseStringUTFChars(grp, nativegrp);
	env->ReleaseStringUTFChars(address, nativeAddress);
	env->ReleaseStringUTFChars(id, nativeID);
	return paramOut;
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_addPlace(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring img,jstring txt,jdouble lon,jdouble lat)
{
	//Import Strings from JNI
	const char *nativegrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
	const char *nativeimg = env->GetStringUTFChars(img, NULL);
	const char *nativetxt = env->GetStringUTFChars(txt, NULL);
	mainApp.addPlace(nativegrp,nativeID,nativeimg,nativetxt,lon,lat);

	jclass cls = env->FindClass("cn/geobeans/earth/Place");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");

	jobject paramOut = env->NewObject(cls, mid, grp,id);
	//Release Strings to JNI
	env->ReleaseStringUTFChars(grp, nativegrp);
	env->ReleaseStringUTFChars(id, nativeID);
	env->ReleaseStringUTFChars(img, nativeimg);
	env->ReleaseStringUTFChars(txt, nativetxt);
	return paramOut;
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerWMS(JNIEnv * env, jobject obj, jstring url)
{
	//Import Strings from JNI
	const char *nativeUrl = env->GetStringUTFChars(url, NULL);
	mainApp.setBaseLayerWMS(std::string(nativeUrl));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(url, nativeUrl);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerTMS(JNIEnv * env, jobject obj, jstring url)
{
	//Import Strings from JNI
	const char *nativeUrl = env->GetStringUTFChars(url, NULL);
	mainApp.setBaseLayerTMS(std::string(nativeUrl));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(url, nativeUrl);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setBaseLayerXYZ(JNIEnv * env, jobject obj, jstring url)
{
	//Import Strings from JNI
	const char *nativeUrl = env->GetStringUTFChars(url, NULL);
	mainApp.setBaseLayerXYZ(std::string(nativeUrl));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(url, nativeUrl);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addLayer(JNIEnv * env, jobject obj, jstring url,
		jstring layer, jstring style, jint minLevel)
{
	//Import Strings from JNI
	const char *nativeUrl = env->GetStringUTFChars(url, NULL);
	const char *nativeLayer = env->GetStringUTFChars(layer, NULL);
	const char *nativeStyle = env->GetStringUTFChars(style, NULL);

	mainApp.addLayer(std::string(nativeUrl), std::string(nativeLayer), std::string(nativeStyle), minLevel);

	//Release Strings to JNI
	env->ReleaseStringUTFChars(url, nativeUrl);
	env->ReleaseStringUTFChars(layer, nativeLayer);
	env->ReleaseStringUTFChars(style, nativeStyle);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeLayer(JNIEnv * env, jobject obj, jstring layer)
{
	//Import Strings from JNI
	const char *nativeLayer = env->GetStringUTFChars(layer, NULL);

	mainApp.removeLayer(std::string(nativeLayer));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(layer, nativeLayer);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addVectorLayer(JNIEnv * env, jobject obj, jstring name, jstring shp,jint tp)
{
	const char *nativeName = env->GetStringUTFChars(name, NULL);
	const char *nativeShp = env->GetStringUTFChars(shp, NULL);

	mainApp.addVectorLayer(std::string(nativeName), std::string(nativeShp),tp);

	//Release Strings to JNI
	env->ReleaseStringUTFChars(name, nativeName);
	env->ReleaseStringUTFChars(shp, nativeShp);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineColor(JNIEnv * env, jobject obj, jstring col)
{
	const char *nativeCol = env->GetStringUTFChars(col, NULL);

	mainApp.setLineColor(std::string(nativeCol));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(col, nativeCol);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setFillColor(JNIEnv * env, jobject obj, jstring col)
{
	const char *nativeCol = env->GetStringUTFChars(col, NULL);

	mainApp.setFillColor(std::string(nativeCol));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(col, nativeCol);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineWidth(JNIEnv * env, jobject obj, jint width)
{
	mainApp.setLineWidth(width);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setFontColor(JNIEnv * env, jobject obj, jstring col)
{
	const char *nativeCol = env->GetStringUTFChars(col, NULL);

	mainApp.setFontColor(std::string(nativeCol));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(col, nativeCol);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeVectorLayer(JNIEnv * env, jobject obj, jstring name)
{
	const char *nativeName = env->GetStringUTFChars(name, NULL);

	mainApp.removeVectorLayer(std::string(nativeName));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(name, nativeName);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addTMSLayer(JNIEnv * env, jobject obj, jstring strName,jstring strUrl)
{
	const char *nativeName = env->GetStringUTFChars(strName, NULL);
	const char *nativeUrl = env->GetStringUTFChars(strUrl, NULL);
	mainApp.addTMSLayer(std::string(nativeName),std::string(nativeUrl));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(strName, nativeName);
	env->ReleaseStringUTFChars(strUrl, nativeUrl);
}
JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeTMSLayer(JNIEnv * env, jobject obj, jstring strName)
{
	const char *nativeName = env->GetStringUTFChars(strName, NULL);

	mainApp.removeTMSLayer(std::string(nativeName));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(strName, nativeName);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addElevationLayer(JNIEnv * env, jobject obj, jstring strName, jstring strUrl)
{
	const char *nativeName = env->GetStringUTFChars(strName, NULL);
	const char *nativeUrl = env->GetStringUTFChars(strUrl, NULL);
	mainApp.addTMSLayer(std::string(nativeName),std::string(nativeUrl));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(strName, nativeName);
	env->ReleaseStringUTFChars(strUrl, nativeUrl);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_removeElevationLayer(JNIEnv * env, jobject obj, jstring strName)
{
	const char *nativeName = env->GetStringUTFChars(strName, NULL);

	mainApp.removeElevationLayer(std::string(nativeName));

	//Release Strings to JNI
	env->ReleaseStringUTFChars(strName, nativeName);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlacePositon(JNIEnv * env, jobject obj, jstring grp,jstring id,jdouble lon,jdouble lat)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

	mainApp.setPlacePositon(nativeGrp,nativeID,lon,lat);

	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);

}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlaceText(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring txt)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
	const char *nativeTxt = env->GetStringUTFChars(txt, NULL);

	mainApp.setPlaceText(nativeGrp,nativeID,nativeTxt);

	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
	env->ReleaseStringUTFChars(txt, nativeTxt);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setPlaceIcon(JNIEnv * env, jobject obj, jstring grp,jstring id,jstring imgfile)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
	const char *nativeImg = env->GetStringUTFChars(imgfile, NULL);

	mainApp.setPlaceIcon(nativeGrp,nativeID,nativeImg);

	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
	env->ReleaseStringUTFChars(imgfile, nativeImg);
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getPlacePosition(JNIEnv * env, jobject obj, jstring grp,jstring id)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
    osgEarth::GeoPoint pnt = mainApp.getPlacePosition(nativeGrp,nativeID);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);

	jclass cls = env->FindClass("cn/geobeans/earth/GeoPoint");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(DDDI)V");

	int altmode = pnt.altitudeMode();
	jobject paramOut = env->NewObject(cls, mid, pnt.x(),pnt.y(),pnt.z(),altmode);
	return paramOut;
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setModelPositon(JNIEnv * env, jobject obj, jstring grp,jstring id,jdouble lon,jdouble lat)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

	const osgEarth::SpatialReference* latLong = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint pnt(latLong, lon, lat);
	mainApp.setModelPositon(nativeGrp,nativeID,pnt);

	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getModelPosition(JNIEnv * env, jobject obj, jstring grp,jstring id)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);
    osgEarth::GeoPoint pnt = mainApp.getModelPosition(nativeGrp,nativeID);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);

	jclass cls = env->FindClass("cn/geobeans/earth/GeoPoint");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(DDDI)V");

	int altmode = pnt.altitudeMode();
	jobject paramOut = env->NewObject(cls, mid, pnt.x(),pnt.y(),pnt.z(),altmode);
	return paramOut;
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addArmySymbol(JNIEnv * env, jobject obj, jstring grp,jstring id,jint key,jobjectArray pnts)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

    std::vector<GVCoord> vtxCtrls;
	int len = env->GetArrayLength(pnts);
	for(int i=0;i<len;i++){
		double lon,lat,alt;
		jobject obj = env->GetObjectArrayElement(pnts, i);
	    //获得jfieldID 以及 该字段的初始值
	    jfieldID  nameFieldId ;

	    jclass cls = env->GetObjectClass(obj);  //获得Java层该对象实例的类引用，即HelloJNI类引用

	    nameFieldId = env->GetFieldID(cls , "lon" , "D"); //获得属性句柄

	    if(nameFieldId == NULL)
	    {
		   return ;
	    }
	    lon = env->GetDoubleField(obj, nameFieldId);// 获得该属性的值

	    nameFieldId = env->GetFieldID(cls , "lat" , "D"); //获得属性句柄

		if(nameFieldId == NULL)
		{
		   return ;
		}
		lat = env->GetDoubleField(obj ,nameFieldId);  // 获得该属性的值

		GVCoord cord(lon,lat,50);
		vtxCtrls.push_back(cord);
	}
	mainApp.addArmyLine(nativeGrp, nativeID, key, vtxCtrls);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setActiveLayer(JNIEnv * env, jobject obj, jstring grp)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	mainApp.setActiveLayer(nativeGrp);
	env->ReleaseStringUTFChars(grp, nativeGrp);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addLine(JNIEnv * env, jobject obj, jstring grp,jstring id,jobjectArray pnts)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

    std::vector<GVCoord> vtxCtrls;
	int len = env->GetArrayLength(pnts);
	for(int i=0;i<len;i++){
		double lon,lat,alt;
		jobject obj = env->GetObjectArrayElement(pnts, i);
	    //获得jfieldID 以及 该字段的初始值
	    jfieldID  nameFieldId ;

	    jclass cls = env->GetObjectClass(obj);  //获得Java层该对象实例的类引用，即HelloJNI类引用

	    nameFieldId = env->GetFieldID(cls , "lon" , "D"); //获得属性句柄

	    if(nameFieldId == NULL)
	    {
		   return ;
	    }
	    lon = env->GetDoubleField(obj, nameFieldId);// 获得该属性的值

	    nameFieldId = env->GetFieldID(cls , "lat" , "D"); //获得属性句柄

		if(nameFieldId == NULL)
		{
		   return ;
		}
		lat = env->GetDoubleField(obj ,nameFieldId);  // 获得该属性的值

		GVCoord cord(lon,lat,50);
		vtxCtrls.push_back(cord);
	}
	mainApp.addLine(nativeGrp, nativeID, vtxCtrls);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_addPolygon(JNIEnv * env, jobject obj, jstring grp,jstring id,jobjectArray pnts)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

    std::vector<GVCoord> vtxCtrls;
	int len = env->GetArrayLength(pnts);
	for(int i=0;i<len;i++){
		double lon,lat,alt;
		jobject obj = env->GetObjectArrayElement(pnts, i);
	    //获得jfieldID 以及 该字段的初始值
	    jfieldID  nameFieldId ;

	    jclass cls = env->GetObjectClass(obj);  //获得Java层该对象实例的类引用，即HelloJNI类引用

	    nameFieldId = env->GetFieldID(cls , "lon" , "D"); //获得属性句柄

	    if(nameFieldId == NULL)
	    {
		   return ;
	    }
	    lon = env->GetDoubleField(obj, nameFieldId);// 获得该属性的值

	    nameFieldId = env->GetFieldID(cls , "lat" , "D"); //获得属性句柄

		if(nameFieldId == NULL)
		{
		   return ;
		}
		lat = env->GetDoubleField(obj ,nameFieldId);  // 获得该属性的值

		GVCoord cord(lon,lat,50);
		vtxCtrls.push_back(cord);
	}
	mainApp.addPolygon(nativeGrp, nativeID, vtxCtrls);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_toMapPoint(JNIEnv * env, jobject obj, jint x,jint y)
{
    double lon;
    double lat;
    double ele;
    mainApp.toMapPoint(x,y,lon,lat,ele);

	jclass cls = env->FindClass("cn/geobeans/earth/GeoPoint");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(DDDI)V");

	jobject paramOut = env->NewObject(cls, mid, lon,lat,ele,0);
	return paramOut;
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_fromMapPoint(JNIEnv * env, jobject obj, jdouble lon,jdouble lat,jdouble ele)
{
	int x;
	int y;
    mainApp.fromMapPoint(lon, lat, ele, x, y);

	jclass cls = env->FindClass("cn/geobeans/earth/GeoPoint");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(DDDI)V");

	double dx = x;
	double dy = y;
	jobject paramOut = env->NewObject(cls, mid, dx,dy,0.0,0);
	return paramOut;
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_startEdit(JNIEnv * env, jobject obj, jstring grp,jstring id)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);
	const char *nativeID = env->GetStringUTFChars(id, NULL);

	mainApp.startEdit(nativeGrp, nativeID);
	env->ReleaseStringUTFChars(grp, nativeGrp);
	env->ReleaseStringUTFChars(id, nativeID);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setDeclutter(JNIEnv * env, jobject obj, jstring grp)
{
	const char *nativeGrp = env->GetStringUTFChars(grp, NULL);

	mainApp.setDeclutter(nativeGrp);
	env->ReleaseStringUTFChars(grp, nativeGrp);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineStyle(JNIEnv * env, jobject obj, jint pattern)
{
	mainApp.setLineStyle(pattern);
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setLineDash(JNIEnv * env, jobject obj, jint dash,jint gap)
{
	mainApp.setLineDash(dash,gap);
}

JNIEXPORT jobject JNICALL Java_cn_geobeans_earth_GEarth_getViewPoint(JNIEnv * env, jobject obj)
{
    osgEarth::Viewpoint vp = mainApp.getViewpoint();

	jclass cls = env->FindClass("cn/geobeans/earth/ViewPoint");
	jmethodID mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;DDDDDD)V");

	std::string nm = vp.getName();
	double dx = vp.x();
	double dy = vp.y();
	double dz = vp.z();
	double heading = vp.getHeading();
	double pitch = vp.getPitch();
	double range = vp.getRange();
	jstring strNm = env->NewStringUTF(nm.c_str());

	jobject paramOut = env->NewObject(cls, mid, strNm,dx,dy,dz,heading,pitch,range);
	return paramOut;
}

JNIEXPORT void JNICALL Java_cn_geobeans_earth_GEarth_setViewPoint(JNIEnv * env, jobject obj, jstring name,jdouble lon,jdouble lat,jdouble alt,jdouble heading,jdouble pitch,jdouble range,jdouble duration)
{
	const char *nativeName = env->GetStringUTFChars(name, NULL);

	osgEarth::Viewpoint v(name,lon,lat,alt,heading,pitch,range);
	mainApp.setViewPoint(v, duration);
	env->ReleaseStringUTFChars(name, nativeName);
}



