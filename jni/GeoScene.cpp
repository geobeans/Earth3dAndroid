#include <GeoScene.h>
#include <osgDB/Registry>

#include <osg/linewidth>
#include <osg/Point>

#include <osgEarthDrivers/wms/WMSOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarthDrivers/kml/KMLNodeInfo>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#include <osgEarth/ImageLayer>

#include <osgEarthAnnotation/Decluttering>

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/PlaceNode>

#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/AltitudeSymbol>
#include <osgEarthSymbology/LineSymbol>

#include <osgEarth/ShaderGenerator>
#include <osgEarth/GeoMath>

#include <cmath>

#include <osgEarthAnnotation/AnnotationSettings>
#include <osgEarthUtil/ObjectPlacer>

#ifndef ANDROID
#include <osgViewer/api/IOS/GraphicsWindowIOS>
#else
#include <osgEarth/AndroidCapabilities>
#endif
#include <GeoAndroidNotifyHandler.hpp>
#include "Compute.h"

#include "GVGeometry.h"
#include "GV00010.h"
#include "GV00008.h"

#include "PlaceNodeEditor.h"

double CalArea(std::vector<osg::Vec3d>& points)
{
	double dLowX,dLowY,dMiddleX,dMiddleY,dHighX,dHighY,dSum1,dSum2;
	int iCount1 = 0;
	int iCount2 = 0;
	int iCount = points.size();
	for (int i = 0; i < iCount; i++)
	    {
	        if (i == 0)
	        {
	            //换算成弧度;
	            dLowX = osg::DegreesToRadians(points.at(iCount - 1).x());
	            dLowY = osg::DegreesToRadians(points.at(iCount - 1).y());
	            dMiddleX = osg::DegreesToRadians(points.at(0).x());
	            dMiddleY = osg::DegreesToRadians(points.at(0).y());
	            dHighX = osg::DegreesToRadians(points.at(1).x());
	            dHighY = osg::DegreesToRadians(points.at(1).y());
	        }
	        else if (i == iCount - 1)
	        {
	            dLowX = osg::DegreesToRadians(points.at(iCount - 2).x());
	            dLowY = osg::DegreesToRadians(points.at(iCount - 2).y());
	            dMiddleX = osg::DegreesToRadians(points.at(iCount - 1).x());
	            dMiddleY = osg::DegreesToRadians(points.at(iCount - 1).y());
	            dHighX = osg::DegreesToRadians(points.at(0).x());
	            dHighY = osg::DegreesToRadians(points.at(0).y());
	        }
	        else
	        {
	            dLowX = osg::DegreesToRadians(points.at(i - 1).x());
	            dLowY = osg::DegreesToRadians(points.at(i - 1).y());
	            dMiddleX = osg::DegreesToRadians(points.at(i).x());
	            dMiddleY = osg::DegreesToRadians(points.at(i).y());
	            dHighX = osg::DegreesToRadians(points.at(i + 1).x());
	            dHighY = osg::DegreesToRadians(points.at(i + 1).y());
	        }
	        double AM = cos(dMiddleY) * cos(dMiddleX);
	        double BM = cos(dMiddleY) * sin(dMiddleX);
	        double CM = sin(dMiddleY);
	        double AL = cos(dLowY) * cos(dLowX);
	        double BL = cos(dLowY) * sin(dLowX);
	        double CL = sin(dLowY);
	        double AH = cos(dHighY) * cos(dHighX);
	        double BH = cos(dHighY) * sin(dHighX);
	        double CH = sin(dHighY);

	        double dCoefficientL = (AM*AM + BM*BM + CM*CM) / (AM*AL + BM*BL + CM*CL);
	        double dCoefficientH = (AM*AM + BM*BM + CM*CM) / (AM*AH + BM*BH + CM*CH);

	        double dALtangent = dCoefficientL * AL - AM;
	        double dBLtangent = dCoefficientL * BL - BM;
	        double dCLtangent = dCoefficientL * CL - CM;
	        double dAHtangent = dCoefficientH * AH - AM;
	        double dBHtangent = dCoefficientH * BH - BM;
	        double dCHtangent = dCoefficientH * CH - CM;

	        double dAngleCos = (dAHtangent * dALtangent + dBHtangent * dBLtangent + dCHtangent * dCLtangent) /
	            (sqrt(dAHtangent * dAHtangent + dBHtangent * dBHtangent + dCHtangent * dCHtangent) *
	            sqrt(dALtangent * dALtangent + dBLtangent * dBLtangent + dCLtangent * dCLtangent));

	        dAngleCos = acos(dAngleCos);

	        double dANormalLine = dBHtangent * dCLtangent - dCHtangent * dBLtangent;
	        double dBNormalLine = 0 - (dAHtangent * dCLtangent - dCHtangent * dALtangent);
	        double dCNormalLine = dAHtangent * dBLtangent - dBHtangent * dALtangent;
            double dOrientationValue;
	        if (AM != 0)
	        {
	            dOrientationValue = dANormalLine / AM;
	        }
	        else if (BM != 0)
	        {
	            dOrientationValue = dBNormalLine / BM;
	        }
	        else
	        {
	            dOrientationValue = dCNormalLine / CM;
	        }
	        if (dOrientationValue > 0)
	        {
	            dSum1 += dAngleCos;
	            iCount1++;
	        }
	        else
	        {
	            dSum2 += dAngleCos;
	            iCount2++;
	        }
	    }
	    double dSum;
	    if (dSum1 > dSum2)
	    {
	        dSum = dSum1 + (2 * osg::PI*iCount2 - dSum2);
	    }
	    else
	    {
	        dSum = (2 * osg::PI*iCount1 - dSum1) + dSum2;
	    }

	    double dTotalArea = (dSum - (iCount - 2)*osg::PI)* osg::WGS_84_RADIUS_EQUATOR *
	        osg::WGS_84_RADIUS_EQUATOR;
	    return dTotalArea;
}

class UpdateFeatureOperation : public osg::Operation
{
public:
	UpdateFeatureOperation( osgEarth::Annotation::FeatureNode* pNode ,bool bDash=false);

	virtual void operator()( osg::Object* );

protected:
	virtual ~UpdateFeatureOperation(){}

protected:
	osg::ref_ptr<osgEarth::Annotation::FeatureNode> m_refNode;
    bool m_bDash;
private:
};

UpdateFeatureOperation::UpdateFeatureOperation( osgEarth::Annotation::FeatureNode* pNode ,bool bDash):m_bDash(bDash)
{
	m_refNode = pNode;
}

void UpdateFeatureOperation::operator()( osg::Object* )
{
	if ( m_refNode.get() != NULL )
	{
		m_refNode->init();
		if(m_bDash){
			osg::Group* grp = m_refNode->getAttachPoint();
			osg::Geode* de = osgEarth::findTopMostNodeOfType<osg::Geode>(grp);
			if (de) {
				osgEarth::Symbology::Geometry* geom = m_refNode->getFeature()->getGeometry();
				osg::Geometry* g = dynamic_cast<osg::Geometry*>(de->getDrawable(0));
				if (g) {
					osg::FloatArray* flts = new osg::FloatArray();
					int num = g->getVertexArray()->getNumElements();
					flts->reserve(num);
					float dis = 0.0;
					flts->push_back(0.0);
					for (int i = 1; i < num; i++) {
						dis += osgEarth::GeoMath::distance(osg::DegreesToRadians(geom->operator[](i).y()),
										  osg::DegreesToRadians(geom->operator[](i).x()),
																osg::DegreesToRadians(geom->operator[](i - 1).y()),
																osg::DegreesToRadians(geom->operator[](i - 1).x()));
						flts->push_back(dis);
					}
					g->setVertexAttribArray(osg::Drawable::ATTRIBUTE_6, flts);
					g->setVertexAttribBinding(osg::Drawable::ATTRIBUTE_6, osg::Geometry::BIND_PER_VERTEX);
					g->setVertexAttribNormalize(osg::Drawable::ATTRIBUTE_6, false);
				}
			}
		}
	}
}

class UpdatePlaceOperation : public osg::Operation
{
public:
	UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osgEarth::GeoPoint pnt,std::string strTxt );
	UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osgEarth::GeoPoint);
	UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,std::string strTxt );
	UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osg::Image* img );

	virtual void operator()( osg::Object* );

protected:
	virtual ~UpdatePlaceOperation(){}

protected:
	osg::ref_ptr<osgEarth::Annotation::PlaceNode> m_refNode;
	osgEarth::optional<osgEarth::GeoPoint> m_pnt;
	osgEarth::optional<std::string> m_strTxt;
	osgEarth::optional<osg::Image*> m_image;

private:
};

UpdatePlaceOperation::UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osgEarth::GeoPoint pnt,std::string strTxt )
{
	m_refNode = pNode;
	m_pnt = pnt;
	m_strTxt = strTxt;
}

UpdatePlaceOperation::UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osgEarth::GeoPoint pnt)
{
	m_refNode = pNode;
	m_pnt = pnt;
}

UpdatePlaceOperation::UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,std::string strTxt )
{
	m_refNode = pNode;
	m_strTxt = strTxt;
}

UpdatePlaceOperation::UpdatePlaceOperation( osgEarth::Annotation::PlaceNode* pNode,osg::Image* img )
{
	m_refNode = pNode;
	m_image = img;
}

void UpdatePlaceOperation::operator()( osg::Object* )
{
	if ( m_refNode.get() != NULL )
	{
		if(m_pnt.isSet())
			m_refNode->setPosition(m_pnt.value());
		if(m_strTxt.isSet())
			m_refNode->setText(m_strTxt.value());
		if(m_image.isSet())
			m_refNode->setIconImage(m_image.value());
	}
}

class UpdateModelOperation : public osg::Operation
{
public:
	UpdateModelOperation( osgEarth::Annotation::ModelNode* pNode,osgEarth::GeoPoint);

	virtual void operator()( osg::Object* );

protected:
	virtual ~UpdateModelOperation(){}

protected:
	osg::ref_ptr<osgEarth::Annotation::ModelNode> m_refNode;
	osgEarth::optional<osgEarth::GeoPoint> m_pnt;

private:
};

UpdateModelOperation::UpdateModelOperation( osgEarth::Annotation::ModelNode* pNode,osgEarth::GeoPoint pnt)
{
	m_refNode = pNode;
	m_pnt = pnt;
}


void UpdateModelOperation::operator()( osg::Object* )
{
	if ( m_refNode.get() != NULL )
	{
		if(m_pnt.isSet())
			m_refNode->setPosition(m_pnt.value());
	}
}

class AddVectorLayer : public osg::Operation
{
public:
	AddVectorLayer( osgEarth::MapNode* pMapNode ,osgEarth::ModelLayer* player);

	virtual void operator()( osg::Object* );

protected:
	virtual ~AddVectorLayer(){}

protected:
	osg::ref_ptr<osgEarth::MapNode> m_refMapNode;
	osg::ref_ptr<osgEarth::ModelLayer> m_pLayer;
private:
};

AddVectorLayer::AddVectorLayer( osgEarth::MapNode* pMapNode ,osgEarth::ModelLayer* player)
{
	m_refMapNode = pMapNode;
	m_pLayer = player;
}

void AddVectorLayer::operator()( osg::Object* )
{
	if ( m_refMapNode.get() != NULL && m_pLayer!=NULL)
	{
		m_refMapNode->getMap()->addModelLayer(m_pLayer);
		osgUtil::GLES2ShaderGenVisitor* gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();
		gles2ShaderGen->apply(*m_refMapNode);
	}
}

class RemoveVectorLayer : public osg::Operation
{
public:
	RemoveVectorLayer( osgEarth::MapNode* pMapNode ,osgEarth::ModelLayer* player);

	virtual void operator()( osg::Object* );

protected:
	virtual ~RemoveVectorLayer(){}

protected:
	osg::ref_ptr<osgEarth::MapNode> m_refMapNode;
	osg::ref_ptr<osgEarth::ModelLayer> m_pLayer;
private:
};

RemoveVectorLayer::RemoveVectorLayer( osgEarth::MapNode* pMapNode ,osgEarth::ModelLayer* player)
{
	m_refMapNode = pMapNode;
	m_pLayer = player;
}

void RemoveVectorLayer::operator()( osg::Object* )
{
	if ( m_refMapNode.get() != NULL && m_pLayer!=NULL)
	{
		m_refMapNode->getMap()->removeModelLayer(m_pLayer);
	}
}

class UpdateLineofSight : public osg::Operation
{
public:
	UpdateLineofSight( osgEarth::Util::LinearLineOfSightNode* pNode ,osgEarth::GeoPoint pnt);

	virtual void operator()( osg::Object* );

protected:
	virtual ~UpdateLineofSight(){}

protected:
	osg::ref_ptr<osgEarth::Util::LinearLineOfSightNode> m_refNode;
	osgEarth::GeoPoint m_pntEnd;
private:
};

UpdateLineofSight::UpdateLineofSight( osgEarth::Util::LinearLineOfSightNode* pNode ,osgEarth::GeoPoint pnt)
{
	m_refNode = pNode;
	m_pntEnd = pnt;
}

void UpdateLineofSight::operator()( osg::Object* )
{
	if ( m_refNode.get() != NULL )
	{
		m_refNode->setEnd(m_pntEnd);
		//osgEarth::GeoPoint pnt = m_refNode->getHit();
		osgUtil::GLES2ShaderGenVisitor* gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();
		gles2ShaderGen->apply(*m_refNode);
	}
}

GeoScene::GeoScene(JNIEnv * env, jobject gjo)
    :osg::Referenced()
{
    _genv = env;
    _gjo = gjo;

    _gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();
    _oper = 0;
    _finished = false;
    _gotFirstLocation = false;
    m_iArmyID = 10;

    m_strLineColor = "#FF0000B0";
    m_strFillColor = "#FF0000B0";
    m_iLineWidth = 2.0;
    m_strFontColor = "#FFFFFFFF";

    m_iLineDash = 100;
    m_iLineGap = 100;
}

GeoScene::~GeoScene()
{
    
}

void GeoScene::setLineColor(std::string col)
{
	m_strLineColor = col;
}

void GeoScene::setFillColor(std::string col)
{
	m_strFillColor = col;
}

void GeoScene::setLineWidth(int width)
{
	m_iLineWidth = width;
}

void GeoScene::setLineStyle(unsigned short pattern)
{
	if(pattern == 0){
		m_lineStyle.unset();
	}else{
		m_lineStyle = pattern;
	}
}

void GeoScene::setFontColor(std::string col)
{
	m_strFontColor = col;
}

void GeoScene::setArmyID(int iID)
{
	m_iArmyID = iID;
}

void GeoScene::addUpdateOper(osg::ref_ptr< osg::Operation > refOperation)
{
	m_sceneMutex.readLock();
	if ( _viewer )
	{
		_viewer->addUpdateOperation( refOperation );
	}
	else
	{
		refOperation->operator()( NULL );
	}
	m_sceneMutex.readUnlock();
}

void GeoScene::setOperation(int oper)
{
	_oper = oper;
	if(_gotFirstLocation){
		_finished = true;
		_gotFirstLocation = false;
	}
}

int GeoScene::getOperation()
{
	return _oper;
}

void GeoScene::addModel(std::string grp,std::string id,std::string address,osgEarth::GeoPoint pnt){
	osgEarth::Util::MapNode* mapNode = this->getMapNode();
	const osgEarth::SpatialReference *latLong = mapNode->getMapSRS();

	osg::ref_ptr<osg::Node> mdl = osgDB::readNodeFile(address);

//	std::string fileNameOut("/storage/emulated/0/tree.osgb");
//    osgDB::ReaderWriter::WriteResult result = osgDB::Registry::instance()->writeNode(*mdl,fileNameOut,osgDB::Registry::instance()->getOptions());
//     if (result.success())
//     {
//         osg::notify(osg::NOTICE)<<"Data written to '"<<fileNameOut<<"'."<< std::endl;
//     }
//     else if  (result.message().empty())
//     {
//         osg::notify(osg::NOTICE)<<"Warning: file write to '"<<fileNameOut<<"' not supported."<< std::endl;
//     }
//     else
//     {
//         osg::notify(osg::NOTICE)<<result.message()<< std::endl;
//     }

	osgEarth::Symbology::Style style;
	style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mdl);
	osg::ref_ptr<osgEarth::Annotation::ModelNode> model = new osgEarth::Annotation::ModelNode(mapNode, style);
	model->setPosition(pnt);
    model->setName(id);
    //_gles2ShaderGen->apply(*model);
	model->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	_annObjs[grp][id] = model.get();
	osgEarth::GeoExtent bounds;
	_changesQueue->put( grp, bounds, model );

//	osgEarth::Util::ObjectPlacer* objPlacer = new osgEarth::Util::ObjectPlacer( _mapNode );
//	osg::Node* hzlOnEarth = objPlacer->placeNode( mdl, pnt.y(),pnt.x(), 50);//lat,lon表示模型在地球的纬度和经度,elevation是海拔
//    hzlOnEarth->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
//	//_gles2ShaderGen->(*hzlOnEarth);
//    hzlOnEarth->accept(*_gles2ShaderGen);
//	//osgUtil::Optimizer optimizer;
//    //optimizer.optimize(hzlOnEarth);
//       //
//	//_mapNode->getParent(0)->addChild(hzlOnEarth);
//
//	osgEarth::GeoExtent bounds;
//	_changesQueue->put( grp, bounds, hzlOnEarth );




//    {
//        osgEarth::Geometry* path = new osgEarth::LineString();
//        path->push_back( osg::Vec3d(116, 40.714,1500) );   // New York
//        path->push_back( osg::Vec3d(118.75, 40.714,1500) ); // Tokyo
//
//        osgEarth::Symbology::Style pathStyle;
//        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color::Red;
//        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 8.0f;
//        pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;
//        //pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
//
//        osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(path, latLong, pathStyle);
//        pathFeature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;
//
//        //OE_INFO << "Path extent = " << pathFeature->getExtent().toString() << std::endl;
//        //osgEarth::Annotation::AnnotationSettings::setApplyDepthOffsetToClampedLines(false);
//        osgEarth::Annotation::FeatureNode* pathNode = new osgEarth::Annotation::FeatureNode(mapNode, pathFeature);
//        mapNode->addChild( pathNode );
//    }

//	{
//		osgEarth::Geometry* geom = new osgEarth::Polygon();
//		geom->push_back(-160., -30.);
//		geom->push_back(150., -20.);
//		geom->push_back(160., -45.);
//		geom->push_back(-150., -40.);
//		osgEarth::Symbology::Style geomStyle;
//		geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color::Red;
//		geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 8.0f;
//		geomStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::White, 0.8);
//		geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
//		//geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;
//		osgEarth::Annotation::FeatureNode* gnode = new osgEarth::Annotation::FeatureNode(_mapNode, new osgEarth::Features::Feature(geom, latLong, geomStyle));
//		mapNode->addChild( gnode );
//	}

	/*
    {
    	osg::Group * grp = new osg::Group;
	    osg::Geode* geode = new osg::Geode();
	    grp->addChild(geode);
	    //_root->addChild(grp);
	    //mapNode->addChild(grp);
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* linesGeom = new osg::Geometry();

        linesGeom->setUseVertexBufferObjects(true);

        //osg::ref_ptr<osg::LineWidth> width = new osg::LineWidth;

        osg::Vec3d startWorld;
        osg::Vec3d endWorld;
        osgEarth::GeoPoint startL(latLong,116.0,40.0,1500.0,osgEarth::ALTMODE_ABSOLUTE);
        osgEarth::GeoPoint endL(latLong,118.0,40.0,1500.0,osgEarth::ALTMODE_ABSOLUTE);

        startL.toWorld(startWorld);
        endL.toWorld(endWorld);
        // this time we'll preallocate the vertex array to the size we
        // need and then simple set them as array elements, 8 points
        // makes 4 line segments.
        osg::Vec3Array* vertices = new osg::Vec3Array(2);
        (*vertices)[0].set(startWorld);
        (*vertices)[1].set(endWorld);


        // pass the created vertex array to the points geometry object.
        linesGeom->setVertexArray(vertices);
//        if (vertices->getVertexBufferObject())
//        	vertices->getVertexBufferObject()->setUsage(GL_STATIC_DRAW_ARB);
        // set the colors as before, plus using the above
        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
        colors->push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
        linesGeom->setColorArray(colors);
        linesGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);


        // set the normal in the same way color.
//        osg::Vec3Array* normals = new osg::Vec3Array;
//        normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
//        linesGeom->setNormalArray(normals);
//        linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
        //linesGeom->addPrimitiveSet(new osg::DrawArrays(3,0,2));

        //width->setWidth(8.0);
        //linesGeom->getOrCreateStateSet()->setAttributeAndModes(width);
        // add the points geometry to the geode.
        geode->addDrawable(linesGeom);

        float size = 6.0;
        //geode->getOrCreateStateSet()->setAttribute( new osg::Point(size), osg::StateAttribute::ON );
        geode->getOrCreateStateSet()->setAttribute( new osg::LineWidth(size), osg::StateAttribute::ON );
        //geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED);

        _gles2ShaderGen->apply(*grp);
        //osgEarth::ShaderGenerator gen( 0L );
        //grp->accept( gen );

        osgEarth::GeoExtent bounds;
        _changesQueue->put( "user", bounds, grp );
    }
*/

//	if( mapNode )
//	{
//		osgDB::Options* opt = new osgDB::Options;
//		osg::ref_ptr<osg::Node> nodekml = osgEarth::Drivers::KML::load( osgEarth::URI( address ), mapNode,
//				opt, *_kml_options );
//
//		osgEarth::GeoExtent bounds;
//		_changesQueue->put( "user", bounds, nodekml );
//
//		osg::Group* asGroup = nodekml.get()->asGroup();
//		bool found = false;
//		for( int i = 0; i < asGroup->getNumChildren() && !found; i++ )
//		{
//			KMLNodeInfo* ni = dynamic_cast<KMLNodeInfo*>(
//					asGroup->getChild( i )->asGroup()->getUserData() );
//			if ( ni )
//			{
//				_manip->setViewpoint( osgEarth::Viewpoint( osg::Vec3d( ni->getPosition().x(),
//						ni->getPosition().y(), 0.0 ), 0.0, -45.0, 50 ), 15.0f);
//				found = true;
//			}
//		}
//	}
}

void GeoScene::addPlace(std::string grp,std::string id,std::string img,std::string txt,osgEarth::GeoPoint pnt)
{
	osgEarth::Util::MapNode* mapNode = this->getMapNode();
	if( mapNode )
	{
		osgEarth::Symbology::Style pin;
		pin.getOrCreate<osgEarth::Symbology::IconSymbol>()->url()->setLiteral(img);
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()="/storage/emulated/0/simhei.ttf";
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 40;
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color(0xff0000ff);
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() = osgEarth::Symbology::Color(0xffffffff);
		pin.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->width() = 16.0;
		_place = new osgEarth::Annotation::PlaceNode(_mapNode, pnt, txt, pin);
		_place->setName(id);

		_annObjs[grp][id] = _place.get();

		osgEarth::GeoExtent bounds;
		_changesQueue->put( grp, bounds, _place );
	}
}

void GeoScene::setBaseLayerWMS(std::string url)
{
	osgEarth::Util::MapNode* mapNode = this->getMapNode();

	if( mapNode )
	{
		osgEarth::ImageLayerVector layers;
		mapNode->getMap()->getImageLayers(layers);

		for( osgEarth::ImageLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it )
		{
			if( ((*it)->getTerrainLayerRuntimeOptions().name()).compare("BASE") == 0 )
				mapNode->getMap()->removeImageLayer( (*it).get() );
		}

		osgEarth::Drivers::WMSOptions wms;
		wms.url() = url;
		osgEarth::ImageLayer* layer = new osgEarth::ImageLayer( "BASE", wms );
		mapNode->getMap()->addImageLayer( layer );

		_gles2ShaderGen->apply(*mapNode);

		OE_WARN << "DemoScene::setBaseLayerWMS	" << url << std::endl;
	}
}

void GeoScene::setBaseLayerTMS( std::string url )
{
	osgEarth::Util::MapNode* mapNode = this->getMapNode();

	if( mapNode )
	{
		osgEarth::ImageLayerVector layers;
		mapNode->getMap()->getImageLayers(layers);

		for( osgEarth::ImageLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it )
		{
			OE_NOTICE << "DEBUG: " << (*it)->getTerrainLayerRuntimeOptions().name() << std::endl;
			if( ((*it)->getTerrainLayerRuntimeOptions().name()).compare("BASE") == 0 )
				mapNode->getMap()->removeImageLayer( (*it).get() );
		}

		osgEarth::Drivers::TMSOptions tms;
		tms.url() = url;
		osgEarth::ImageLayer* layer = new osgEarth::ImageLayer( "BASE", tms );
		mapNode->getMap()->addImageLayer( layer );

		_gles2ShaderGen->apply(*mapNode);

		OE_WARN << "DemoScene::setBaseLayerTMS	" << url << std::endl;
	}
}

void GeoScene::setBaseLayerXYZ(std::string url)
{
	osgEarth::Util::MapNode* mapNode = this->getMapNode();

	if( mapNode )
	{
		osgEarth::ImageLayerVector layers;
		mapNode->getMap()->getImageLayers(layers);

		for( osgEarth::ImageLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it )
		{
			OE_NOTICE << "DEBUG: " << (*it)->getTerrainLayerRuntimeOptions().name() << std::endl;
			if( ((*it)->getTerrainLayerRuntimeOptions().name()).compare("BASE") == 0 )
				mapNode->getMap()->removeImageLayer( (*it).get() );
		}

		osgEarth::Drivers::XYZOptions xyz;
		xyz.url() = url;
		//xyz.profile() = "spherical-mercator";
		osgEarth::ImageLayer* layer = new osgEarth::ImageLayer( "BASE", xyz );
		mapNode->getMap()->addImageLayer( layer );

		_gles2ShaderGen->apply(*mapNode);

		OE_WARN << "DemoScene::setBaseLayerXYZ	" << url << std::endl;
	}
}

void GeoScene::removeLayer( std::string name )
{
	OE_NOTICE <<"DEBUG: addLayer->" << name << std::endl;
	osgEarth::Util::MapNode* mapNode = this->getMapNode();

	if( mapNode )
	{
		osgEarth::ImageLayerVector layers;
		mapNode->getMap()->getImageLayers(layers);

		for( osgEarth::ImageLayerVector::const_iterator it = layers.begin(); it != layers.end(); ++it )
		{
			OE_NOTICE << "DEBUG: " << (*it)->getTerrainLayerRuntimeOptions().name() << std::endl;
			if( ((*it)->getTerrainLayerRuntimeOptions().name()).compare(name) == 0 )
				mapNode->getMap()->removeImageLayer( (*it).get() );
		}

		_annObjs.erase(name);

		_changesQueue->cleanNodes(name);
	}
}

void GeoScene::addLayer( std::string url, std::string layer, std::string style, int minLevel )
{
	OE_NOTICE << "DEBUG: addLayer->" << layer << std::endl;
    osgEarth::Drivers::WMSOptions wms;
    wms.url() = url;
    wms.layers() = layer;
    wms.format() = "kml";
    wms.style() = style;
    wms.tileSize() = 256;
    wms.mapNode() = (void*) (_mapNode.get());
    wms.kmlOptions() = (void*) (_kml_options);

    wms.root() = (void*) (_root.get());

    osgEarth::ImageLayerOptions* imgLayerOpt = new osgEarth::ImageLayerOptions(layer, wms);
    imgLayerOpt->minLevel() = minLevel;

    osgEarth::ImageLayer* newlayer = new osgEarth::ImageLayer( *imgLayerOpt );
    _mapNode->getMap()->addImageLayer( newlayer );
}

void GeoScene::onMove( float x, float y )
{
	osgEarth::Util::EarthManipulator* manip =
			dynamic_cast<osgEarth::Util::EarthManipulator*>(_viewer->getCameraManipulator());

	if( manip )
	{
		osg::Vec3d out;
		bool valid = manip->screenToWorld(x, y, _viewer, out );

		if( valid )
		{
			osgEarth::Util::MapNode* mapNode = this->getMapNode();
			osg::Vec3d outll;
			mapNode->getMap()->getSRS()->transformFromWorld(out, outll);

			if( abs(outll.z()) < 500 )
			{
				_changesQueue->onMove( outll, 2000, mapNode->getMap()->getSRS() );
			}
		}
	}
}

void GeoScene::setLineDash(int dash,int gap)
{
	m_iLineDash = dash;
	m_iLineGap = gap;
}

void GeoScene::setLineDashStyle(osg::Node* nd)
{
	if(m_lineStyle.isSet()){
//		const char* vert =
//			"#version " GLSL_VERSION_STR "\n"
//			"varying vec3 v_pos; \n"
//			"void oe_stipple_vert(inout vec4 VertexVIEW) \n"
//			"{ \n"
//			//"    gl_Position = gl_Vertex; \n"
//			//"    v_pos = VertexVIEW.xyz; \n"
//			"} \n";
//
//	    const char *s_VertShader =
//	        "#version " GLSL_VERSION_STR "\n"
//	        "varying vec3 v_pos; \n"
//	        //"uniform mat4 osg_ModelViewMatrix; \n"
//	        "void oe_stipple_vert() \n"
//	        "{ \n"
//	        //"    v_pos = vec3(osg_ModelViewMatrix * osg_Vertex); \n"
//			"		v_pos = osg_Vertex.xyz; \n"
//	        "} \n";

//		const char* frag =
//		    "#version " GLSL_VERSION_STR "\n"
//			"precision mediump float;\n"
//			"varying vec3 v_pos; \n"
//			"void oe_stipple_frag(inout vec4 color)\n"
//			"{\n"
//			"  if (step(sin(0.1*length(gl_FragCoord.xy)), 0.5))\n"
//			"    color.a = 0.0;\n"
//			"  else \n"
//			"    color = vec4(0.0,1.0,0.0,1.0);\n"
//			"}\n";

//		const char* frag =
//		    "#version " GLSL_VERSION_STR "\n"
//			"precision highp float; \n"
//			"void oe_stipple_frag(inout vec4 color)\n"
//			"{\n"
//			"  float len = length(gl_FragCoord.xy);\n"
//			"  if ( mod(len,40.0) < 20.0)\n"
//			"  {\n"
//			"    color.a = 0.0;\n"
//			"  }\n"
//			"  else \n"
//			"  {\n"
//			"    color = vec4(0.0,1.0,0.0,1.0);\n"
//			"  }\n"
//			"}\n";

		const char* vert =
		    "#version " GLSL_VERSION_STR "\n"
			"attribute float oe_vertex_length; \n"
			"varying float v_len; \n"
			"void oe_index_vert() \n"
			"{ \n"
			"    v_len = oe_vertex_length; \n"
			"} \n";

		const char* frag =
		    "#version " GLSL_VERSION_STR "\n"
			"precision mediump float;\n"
			"varying float v_len; \n"
			"uniform float dash_len;\n"
			"uniform float gap_len;\n"
			"void oe_stipple_frag(inout vec4 color)\n"
			"{\n"
			"    float f = mod(v_len, dash_len+gap_len); \n"
			"    if(f>dash_len) \n "
		    "        color.a = 0.0; \n"
		    "}\n";

        osgEarth::VirtualProgram* vp = new osgEarth::VirtualProgram();
        vp->installDefaultColoringAndLightingShaders();

		vp->addBindAttribLocation("oe_vertex_length", osg::Drawable::ATTRIBUTE_6);
		vp->setFunction("oe_index_vert", vert, osgEarth::ShaderComp::LOCATION_VERTEX_PRE_COLORING);
		vp->setFunction("oe_stipple_frag", frag, osgEarth::ShaderComp::LOCATION_FRAGMENT_PRE_LIGHTING);

        vp->setInheritShaders(false);
        nd->getOrCreateStateSet()->setAttributeAndModes( vp, osg::StateAttribute::ON );

        osg::ref_ptr<osg::Uniform> uniDash = new osg::Uniform(osg::Uniform::FLOAT, "dash_len");
		float fDash = m_iLineDash;
        uniDash->set(fDash);
		//uniDash->setUpdateCallback(new ColorCallback(viewer.getCamera(), man.get()));
        nd->getOrCreateStateSet()->addUniform(uniDash.get());

        osg::ref_ptr<osg::Uniform> uniGap = new osg::Uniform(osg::Uniform::FLOAT, "gap_len");
		float fGap = m_iLineGap;
		uniGap->set(fGap);
		nd->getOrCreateStateSet()->addUniform(uniGap.get());
	}
}

void GeoScene::buildLine(std::string strGrp)
{
	   // Define the path feature:
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color::Red;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
	//pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;
	_feature = new osgEarth::Features::Feature(new osgEarth::Features::LineString(), getMapNode()->getMapSRS(),pathStyle);
	_feature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

	_featureNode = new osgEarth::Annotation::FeatureNode( getMapNode(), _feature.get() );
	_featureNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	setLineDashStyle(_featureNode.get());

	osgEarth::GeoExtent bounds;
	_changesQueue->put( strGrp, bounds, _featureNode );
}

void GeoScene::buildPolygon(std::string strGrp)
{
	   // Define the path feature:
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color::Red;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
	pathStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(m_strFillColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::White, 0.8);
	//
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;

	_featureP = new osgEarth::Features::Feature(new osgEarth::Features::Polygon(), getMapNode()->getMapSRS(),pathStyle);

	_featureNodeP = new osgEarth::Annotation::FeatureNode( getMapNode(), _featureP.get() );
	_featureNodeP->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osgEarth::GeoExtent bounds;
	_changesQueue->put( strGrp, bounds, _featureNodeP );
}

void GeoScene::buildLineOfSight(osgEarth::GeoPoint start,osgEarth::GeoPoint end)
{
	_lineofSight = new osgEarth::Util::LinearLineOfSightNode(
	        _mapNode,
	        start,
	        end );

	_lineofSight->setTerrainOnly(true);
	_lineofSight->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	//_lineofSight->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	_lineofSight->getOrCreateStateSet()->setAttribute(new osg::LineWidth(6.0f));

	//osgUtil::GLES2ShaderGenVisitor* gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();
	//_gles2ShaderGen->apply(*_lineofSight);

	osgEarth::GeoExtent bounds;
	_changesQueue->put( "analysis", bounds, _lineofSight );
}

void GeoScene::buildRadialLineOfSight(osgEarth::GeoPoint center)
{
	_radialLineofSight = new osgEarth::Util::RadialLineOfSightNode( _mapNode );
	_radialLineofSight->setCenter( center );
	_radialLineofSight->setTerrainOnly(true);
	_radialLineofSight->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	_radialLineofSight->setRadius( 2000 );

    //This RadialLineOfSightNode is going to be filled, so set some alpha values for the colors so it's partially transparent
	_radialLineofSight->setGoodColor(osgEarth::Symbology::Color(0,255,0,0.8));
	_radialLineofSight->setBadColor(osgEarth::Symbology::Color(255,0,0,0.8));
	_radialLineofSight->setFill( true );
    _radialLineofSight->setNumSpokes( 100 );

	_gles2ShaderGen->apply(*_radialLineofSight);

	osgEarth::GeoExtent bounds;
	_changesQueue->put( "analysis", bounds, _radialLineofSight );
}

void GeoScene::buildArmyLine(int key)
{
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =  osgEarth::Symbology::Color(m_strFillColor); //osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;
	pathStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 10;

	_armyLine = drawJB(key, m_vtxCtrls, pathStyle, true);
	osgEarth::GeoExtent bounds;
	_changesQueue->put( "army", bounds, _armyLine );
}

void GeoScene::addArmyLine(std::string grp,std::string id,int key,std::vector<GVCoord> &vtxCtrls)
{
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =  osgEarth::Symbology::Color(m_strFillColor); //osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;
	pathStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 10;

	_armyLine = drawJB(key, vtxCtrls, pathStyle, true);
	_armyLine->setName(id);
	osgEarth::GeoExtent bounds;
	_changesQueue->put( grp, bounds, _armyLine );
}

void GeoScene::addLine(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls)
{
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.8);
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
//	if(m_lineStyle.isSet())
//		pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->stipple() = m_lineStyle;// 0x1111;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;

	osg::ref_ptr<osgEarth::Symbology::LineString> path = new osgEarth::Symbology::LineString();
	for(int i=0;i<vtxCtrls.size();i++){
		path->push_back(osg::Vec3d(vtxCtrls[i].lon, vtxCtrls[i].lat, vtxCtrls[i].alt));   // New York
	}

	osg::ref_ptr<osgEarth::Features::Feature> pathFeature = new osgEarth::Features::Feature(path, getMapNode()->getMapSRS(), pathStyle);
	pathFeature->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

	osg::ref_ptr<osgEarth::Annotation::FeatureNode> featureNode = new osgEarth::Annotation::FeatureNode( getMapNode(), pathFeature.get() );
	featureNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	setLineDashStyle(featureNode.get());

	featureNode->setName(id);
	osgEarth::GeoExtent bounds;
	_changesQueue->put( grp, bounds, featureNode );
}

void GeoScene::addPolygon(std::string grp,std::string id,std::vector<GVCoord> &vtxCtrls)
{
	osgEarth::Symbology::Style pathStyle;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color::Red;
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = m_iLineWidth;//6.0f;
	pathStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(m_strFillColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::White, 0.8);
	//
	pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellation() = 20;

	osg::ref_ptr<osgEarth::Symbology::Polygon> path = new osgEarth::Symbology::Polygon();
	for(int i=0;i<vtxCtrls.size();i++){
		path->push_back(osg::Vec3d(vtxCtrls[i].lon, vtxCtrls[i].lat, vtxCtrls[i].alt));   // New York
	}

	osg::ref_ptr<osgEarth::Features::Feature> featureP = new osgEarth::Features::Feature(path, getMapNode()->getMapSRS(),pathStyle);

	osg::ref_ptr<osgEarth::Annotation::FeatureNode> featureNode = new osgEarth::Annotation::FeatureNode( getMapNode(), featureP.get() );
	featureNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	featureNode->setName(id);

	osgEarth::GeoExtent bounds;
	_changesQueue->put( grp, bounds, featureNode );
}

osg::Node* GeoScene::drawJB(int key, std::vector<GVCoord>& vtxCtrls,osgEarth::Symbology::Style geomStyle, bool isFill)
{
	GVGeometry* line=NULL;
	switch(key)
	{
	//case 5:
	//	line = new GV00005();
	//	break;
	case 6:
		//line = new GV00006();
		break;
	//case 7:
	//	line = new GV00007();
	//	break;
	case 8:
		line = new GV00008(2);
		break;
	case 9:
		//line = new GV00009();
		break;
	case 10:
		line = new GV00010(0);
		break;
	case 11:
		line = new GV00010(1);
		break;
	case 12:
		line = new GV00010(3);
		break;
	case 13:
		line = new GV00010(4);
		break;
	case 14:
		line = new GV00010(5);
		break;
	case 15:
		//line = new GV00008(3);
		break;
	}
	if(line!=NULL)
	{
		osg::ref_ptr<osg::Group> annoGroup = new osg::Group();
		std::vector<GVCoord> vtxBuffer;
		std::vector<int> vtxBuffer2;
		line->setControlPoints(vtxCtrls);
		if(line->toVertice(vtxBuffer,&vtxBuffer2,_mapNode))
		{
			unsigned int count = 0;
			for(int i =0; i < vtxBuffer2.size(); i++)
			{
				osg::ref_ptr<osgEarth::Symbology::Geometry> geom;
				if(isFill==false)
					geom= new osgEarth::Symbology::LineString;
				else
					geom= new osgEarth::Symbology::Polygon;
				for(int j =0; j < vtxBuffer2[i]; j++)
				{
					//geom->push_back(osg::Vec3d(vtxBuffer[count].lon,vtxBuffer[count].lat,vtxBuffer[count].alt));
					geom->push_back(osg::Vec3d(vtxBuffer[count].lon,vtxBuffer[count].lat,50));
					count++;
				}
				osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(geom,_mapNode->getMapSRS(),geomStyle);
				osg::ref_ptr<osgEarth::Annotation::FeatureNode> gnode = new osgEarth::Annotation::FeatureNode(_mapNode,feature);
				gnode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED);
				//gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
				//_gles2ShaderGen->apply(*gnode);
				annoGroup->addChild( gnode );
			}
			//mAnnoGroup->addChild(annoGroup);

		}
		return annoGroup.release();
	}
	return NULL;
}

void GeoScene::onTap( JNIEnv * env,float x, float y )
{
	osgEarth::Util::EarthManipulator* manip =
			dynamic_cast<osgEarth::Util::EarthManipulator*>(_viewer->getCameraManipulator());
    y = _viewer->getCamera()->getViewport()->height()-y;
	if( manip )
	{
		osg::Vec3d out;
		bool valid = manip->screenToWorld(x, y, _viewer, out );

		if( valid )
		{
			const osgEarth::SpatialReference* latLong = osgEarth::SpatialReference::get("wgs84");
			osgEarth::Util::MapNode* mapNode = this->getMapNode();
			osgEarth::ElevationQuery query(_mapNode->getMap());
			osg::Vec3d outll;
			mapNode->getMap()->getSRS()->transformFromWorld(out, outll);
			outll.z() += 10;
			osgEarth::GeoPoint pnt(latLong, outll,osgEarth::ALTMODE_ABSOLUTE);
			switch(_oper){
			case 1:
				{
					if(!_gotFirstLocation){
						_finished = false;
						buildLine("measure");

						addPlace("measure","id","/storage/emulated/0/placemark32.png","0米",pnt);
						_gotFirstLocation = true;
						_feature->getGeometry()->push_back( outll );
					}else{
						_feature->getGeometry()->push_back( outll );
						addUpdateOper(new UpdateFeatureOperation(_featureNode));
						//_featureNode->init();
						if(_finished){
							_gotFirstLocation = false;
						}
						double distance = osgEarth::GeoMath::distance(_feature->getGeometry()->asVector());
						char strBuf[125];
						sprintf(strBuf,"%.3f米",distance);
						addPlace("measure","id","/storage/emulated/0/placemark32.png",strBuf,pnt);
					}
				}
				break;
			case 6:
				{
					if(!_gotFirstLocation){
						_finished = false;
						if(m_strActiveGrp!="")
							buildLine(m_strActiveGrp);
						else
							buildLine("Graphics");
						_gotFirstLocation = true;
						_feature->getGeometry()->push_back( outll );
					}else{
						_feature->getGeometry()->push_back( outll );
						addUpdateOper(new UpdateFeatureOperation(_featureNode,m_lineStyle.isSet()));
						if(_finished){
							_gotFirstLocation = false;
						}
					}
				}
				break;
			case 2:
				{
					if(!_gotFirstLocation){
						_finished = false;
						buildPolygon("measure");

						addPlace("measure","id","/storage/emulated/0/placemark32.png","0平方米",pnt);
						_place->setDynamic(true);
						_gotFirstLocation = true;
						_featureP->getGeometry()->push_back( outll );
					}else{
						_featureP->getGeometry()->push_back( outll );
						addUpdateOper(new UpdateFeatureOperation(_featureNodeP));
						//_featureNodeP->init();
						if(_finished){
							_gotFirstLocation = false;
						}

						if(_featureP->getGeometry()->asVector().size()>2){
//
//							Triangles triangles = Triangles();
//							double area = 0.0;
//							if (triangles.buildTriangles(_featureP->getGeometry()->asVector()))
//							{
//								//triangles.setPrecision(5);	//设置迭代次数
//								triangles.LODTriangles();
//
//								triangles.isHeight(query, mapNode);
//								if (triangles.getIsHeight())
//									triangles.isXYZ();
//
//								if (triangles.getIsXYZ())
//									area = triangles.getAllArea();
//							}

							double area = CalArea(_featureP->getGeometry()->asVector());
//
							char strBuf[125];
							sprintf(strBuf,"%.3f",area);

							osg::Vec3d ct = _featureNodeP->getBound().center();

							osgEarth::GeoPoint pos;//(latLong, _featureNodeP->getBound().center().x(),_featureNodeP->getBound().center().y());
							pos.fromWorld(latLong, ct);
							//_place->setPosition(pos);
							//_place->setText(strBuf);
							std::string strA = strBuf;
							//strA+="平方米";
							addUpdateOper(new UpdatePlaceOperation(_place,pos,strA));
						}

					}
				}
				break;
			case 7:
				{
					if(!_gotFirstLocation){
						_finished = false;
						if(m_strActiveGrp!="")
							buildPolygon(m_strActiveGrp);
						else
							buildPolygon("Graphics");

						_gotFirstLocation = true;
						_featureP->getGeometry()->push_back( outll );
					}else{
						_featureP->getGeometry()->push_back( outll );
						addUpdateOper(new UpdateFeatureOperation(_featureNodeP));
						//_featureNodeP->init();
						if(_finished){
							_gotFirstLocation = false;
						}
					}
				}
				break;
			case 3:
				{
					if(!_gotFirstLocation){
						_finished = false;
						buildLineOfSight(pnt,pnt);

						addPlace("analysis","id","/storage/emulated/0/placemark32.png","起点",pnt);
						_gotFirstLocation = true;
					}else{
						addUpdateOper(new UpdateLineofSight(_lineofSight,pnt));

						_oper = 0;
						_finished = true;
						_gotFirstLocation = false;
						addPlace("analysis","id","/storage/emulated/0/placemark32.png","终点",pnt);
					}
				}
				break;
			case 4:
				{
					buildRadialLineOfSight(pnt);
					addPlace("analysis","id","/storage/emulated/0/placemark32.png","观察点",pnt);
					_oper = 0;
				}
				break;
			case 5:
				{
					if(!_gotFirstLocation){
						_finished = false;
						_gotFirstLocation = true;
						GVCoord cord(outll.x(),outll.y(),outll.z());
						m_vtxCtrls.push_back(cord);
					}else{
						GVCoord cord(outll.x(),outll.y(),outll.z());
						m_vtxCtrls.push_back(cord);
						if(m_vtxCtrls.size()==2 && m_iArmyID==10){
							_oper = 0;
							_finished = true;
							_gotFirstLocation = false;

							buildArmyLine(m_iArmyID);
							m_vtxCtrls.clear();
						}else if(m_vtxCtrls.size()==4 && m_iArmyID==8){
							_oper = 0;
							_finished = true;
							_gotFirstLocation = false;

							buildArmyLine(m_iArmyID);
							m_vtxCtrls.clear();
						}
						else if(m_vtxCtrls.size()==3 && m_iArmyID==13){
							_oper = 0;
							_finished = true;
							_gotFirstLocation = false;

							buildArmyLine(m_iArmyID);
							m_vtxCtrls.clear();
						}
					}
				}
				break;
			default:
				{
					jclass clazz = env->FindClass("cn/geobeans/earth/GEarth");
					if( !clazz )
					{
						OE_WARN << "[Demoscene tap]\tJNI getClass error." << std::endl;
						return ;
					}
					jmethodID method = env->GetStaticMethodID(clazz, "fireTapped", "(DDD)V");
					if( !method )
					{
						OE_WARN << "[Demoscene tap]\tJNI getMethod error." << std::endl;
						return ;
					}

					env->CallStaticVoidMethod( _gjo, method,  outll.x(),outll.y(),outll.z()); // @suppress("Invalid arguments")
				}
				break;
			}

		}
	}
}

void GeoScene::onDoubleTap( JNIEnv * env,float x, float y )
{
	switch(_oper){
	case 1:
	case 2:
	case 6:
	case 7:
		{
			if(_gotFirstLocation){
				_finished = true;
				_gotFirstLocation = false;
				_oper = 0;
			}
		}
		break;
	case 5:
		{
			if(m_vtxCtrls.size()>=3){
				_oper = 0;
				_finished = true;
				_gotFirstLocation = false;

				buildArmyLine(m_iArmyID);
				m_vtxCtrls.clear();
			}
		}
		break;
	default:
		break;
	}

}

void GeoScene::init(const std::string& file, osg::Vec2 viewSize, UIView* view)
{
	GeoAndroidNotifyHandler* notifyHandler = new GeoAndroidNotifyHandler();
	notifyHandler->setTag("osgEarth Viewer");
    osg::setNotifyHandler(notifyHandler);
    osgEarth::setNotifyHandler(notifyHandler);

    osg::setNotifyLevel(osg::WARN);
    osgEarth::setNotifyLevel(osg::WARN);

    osgDB::Registry::instance()->addFileExtensionAlias("w3ds", "wms");
    osgDB::Registry::instance()->addMimeTypeExtensionMapping("vnd.google-earth.kml+xml", "kml");
    osgEarth::Registry::instance()->setDefaultTerrainEngineDriverName("mp");
	//osgEarth::Registry::instance()->setCapabilities(new osgEarth::AndroidCapabilities());

    //create the viewer
	_viewer = new osgViewer::Viewer();
    
#ifndef ANDROID
    UIScreen* screen = [UIScreen mainScreen];
    float scale = 1.0f;
#if defined(__IPHONE_4_0) && (__IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0)
    scale = [screen scale];
#endif
    
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    
	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = viewSize.x();
	traits->height = viewSize.y();
	traits->depth = 24; //keep memory down, default is currently 24
	traits->alpha = 8;
    traits->samples = 4;
    traits->sampleBuffers = 2;
	traits->stencil = 0;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
    
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowIOS::WindowData( view,
			osgViewer::GraphicsWindowIOS::WindowData::PORTRAIT_ORIENTATION, scale );
	traits->inheritedWindowData = windata;
    
	// Create the Graphics Context
	osg::ref_ptr<osg::GraphicsContext> graphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());
	
	if(graphicsContext)
	{
        _viewer->getCamera()->setGraphicsContext(graphicsContext);
    }
    
#else
    _viewer->setUpViewerAsEmbeddedInWindow(0, 0, viewSize.x(), viewSize.y());
#endif
    
    
    _viewer->getCamera()->setViewport(new osg::Viewport(0, 0, viewSize.x(), viewSize.y()));
    _viewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _viewer->getCamera()->setClearColor(osg::Vec4(0.098f,0.098f,0.439f,1.0f));
    //_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
    _viewer->getCamera()->setProjectionMatrixAsPerspective(45.0f,(float)viewSize.x()/viewSize.y(),
                                                           0.1, 1000.0);

    _viewer->getEventQueue()->getCurrentEventState()->setMouseYOrientation(osgGA::GUIEventAdapter::Y_INCREASING_UPWARDS);
    
    // configure the near/far so we don't clip things that are up close
    _viewer->getCamera()->setNearFarRatio(0.00002);
    //_viewer->getCamera()->setSmallFeatureCullingPixelSize(10.0f );
    //optimize viewer and db pager
    _viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    _viewer->getCamera()->setLODScale(_viewer->getCamera()->getLODScale()*1.0);
    
    // osgEarth benefits from pre-compilation of GL objects in the pager. In newer versions of
    // OSG, this activates OSG's IncrementalCompileOpeartion in order to avoid frame breaks.
    _viewer->getDatabasePager()->setIncrementalCompileOperation(new osgUtil::IncrementalCompileOperation());
    _viewer->getDatabasePager()->setDoPreCompile( true );
    //_viewer->getDatabasePager()->setTargetMaximumNumberOfPageLOD(0);
    //_viewer->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,true);
    _viewer->setRunFrameScheme( osgViewer::ViewerBase::ON_DEMAND );
    
    //init scene
    this->initDemo(file);
}

#include <signal.h>
#include <setjmp.h>

jmp_buf env;

void recvSignal(int sig)
{
    siglongjmp(env,1);
}

void GeoScene::frame()
{
	//std::cout << "FrameNumber: " << _viewer->getViewerFrameStamp()->getFrameNumber() << std::endl;
    if (_viewer->getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS ||
        _viewer->checkNeedToDoFrame() )
    {
    	int r = sigsetjmp(env,1);
    	if(  r  == 0)
    	{
    	//初次执行 ，那么可以执行 可能会发生错误的代码
    	//给信号注册一个处理函数
    		signal(SIGSEGV, recvSignal);
        }

        _viewer->frame();
    }else{

    }
}

void GeoScene::addVectorLayer(std::string name,std::string shp,int tp)
{
    // create a feature source to load the street shapefile.
	osgEarth::Drivers::OGRFeatureOptions feature_opt;
    feature_opt.name() = name;
    feature_opt.url() = shp;
    //feature_opt.buildSpatialIndex() = true;

    // a resampling filter will ensure that the length of each segment falls
    // within the specified range. That can be helpful to avoid cropping
    // very long lines segments.
    //feature_opt.filters().push_back( new ResampleFilter(0.0, 25.0) );

    // a style:
    osgEarth::Symbology::Style style;
    style.setName( name );

    if(tp==0){
    	osg::Image* image = osgDB::readImageFile("/storage/emulated/0/placemark32.png");
    	style.getOrCreate<osgEarth::Symbology::IconSymbol>()->setImage(image);
    }else{
   		// Render the data as translucent yellow lines that are 7.5m wide.
		osgEarth::Symbology::LineSymbol* line = style.getOrCreate<osgEarth::Symbology::LineSymbol>();
		line->stroke()->color() = osgEarth::Symbology::Color(m_strLineColor);//osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 1.0f);
		line->stroke()->width() = m_iLineWidth;//8.0f;
		//line->stroke()->widthUnits() = Units::METERS;
		//line->tessellation() = 20;
	    if(tp==2){
	    	style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(m_strFillColor);
	    }

		// Clamp the lines to the terrain.
		osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
		alt->clamping() = alt->CLAMP_TO_TERRAIN;
		//alt->technique() = AltitudeSymbol::TECHNIQUE_GPU;
		// Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
		// apparent offset (towards the camera) of the geometry from its actual position.
		// The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
		osgEarth::Symbology::RenderSymbol* render = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
		render->depthOffset()->minBias() = 6.6f;
    }
    // Set up a paging layout. The tile size factor and the visibility range combine
    // to determine the tile size, such that tile radius = max range / tile size factor.
//    FeatureDisplayLayout layout;
//    layout.tileSizeFactor() = 7.5f;
//    layout.maxRange()       = 5000.0f;

    // create a model layer that will render the buildings according to our style sheet.
    osgEarth::Drivers::FeatureGeomModelOptions fgm_opt;
    fgm_opt.featureOptions() = feature_opt;
    //fgm_opt.layout() = layout;
    fgm_opt.styles() = new osgEarth::Symbology::StyleSheet();
    fgm_opt.styles()->addStyle( style );

    if(_mapNode){
    	osg::ref_ptr<osgEarth::ModelLayer> lyr = new osgEarth::ModelLayer(name, fgm_opt);
    	addUpdateOper(new AddVectorLayer(_mapNode,lyr.get()));
    	//_mapNode->getMap()->addModelLayer( new osgEarth::ModelLayer(name, fgm_opt) );
    	//_gles2ShaderGen->apply(*_mapNode);
    }
}

void GeoScene::removeVectorLayer(std::string name)
{
	osgEarth::ModelLayer* pLayer = _mapNode->getMap()->getModelLayerByName(name);
	addUpdateOper(new RemoveVectorLayer(_mapNode,pLayer));
	//_mapNode->getMap()->removeModelLayer(pLayer);
}

void GeoScene::addTMSLayer(std::string strName,std::string strUrl)
{
	osgEarth::Drivers::TMSOptions imagery;
	imagery.url() = strUrl;
	_mapNode->getMap()->addImageLayer(new osgEarth::ImageLayer(strName, imagery));
}

void GeoScene::removeTMSLayer(std::string strName)
{
	osgEarth::ImageLayer* pLayer = _mapNode->getMap()->getImageLayerByName(strName);
	_mapNode->getMap()->removeImageLayer(pLayer);
}

void GeoScene::addElevationLayer(std::string strName, std::string strUrl)
{
	osgEarth::Drivers::TMSOptions imagery;
	imagery.url() = strUrl;
	_mapNode->getMap()->addElevationLayer(new osgEarth::ElevationLayer(strName, imagery));
}

void GeoScene::removeElevationLayer(std::string strName)
{
	osgEarth::ElevationLayer* pLayer = _mapNode->getMap()->getElevationLayerByName(strName);
	_mapNode->getMap()->removeElevationLayer(pLayer);
}

void GeoScene::setPlacePositon(std::string grp,std::string id,osgEarth::GeoPoint pnt)
{
	osgEarth::Annotation::PlaceNode* nd = _annObjs[grp][id];
	if(nd)
		addUpdateOper(new UpdatePlaceOperation(nd,pnt));
}

void GeoScene::setPlaceText(std::string grp,std::string id,std::string txt)
{
	osgEarth::Annotation::PlaceNode* nd = _annObjs[grp][id];
	if(nd)
		addUpdateOper(new UpdatePlaceOperation(nd,txt));
}

void GeoScene::setPlaceIcon(std::string grp,std::string id,osg::Image* img)
{
	osgEarth::Annotation::PlaceNode* nd = _annObjs[grp][id];
	if(nd)
		addUpdateOper(new UpdatePlaceOperation(nd,img));
}

osgEarth::GeoPoint GeoScene::getPlacePosition(std::string grp,std::string id)
{
	osgEarth::Annotation::PlaceNode* nd = _annObjs[grp][id];
	if(nd)
	    return nd->getPosition();
	return osgEarth::GeoPoint();
}

void GeoScene::setModelPositon(std::string grp,std::string id,osgEarth::GeoPoint pnt)
{
	osgEarth::Annotation::ModelNode* nd = _annObjs[grp][id];
	if(nd)
		addUpdateOper(new UpdateModelOperation(nd,pnt));
}

osgEarth::GeoPoint GeoScene::getModelPosition(std::string grp,std::string id)
{
	osgEarth::Annotation::ModelNode* nd = _annObjs[grp][id];
	if(nd)
	    return nd->getPosition();
	return osgEarth::GeoPoint();
}

void GeoScene::setActiveLayer(std::string strLyr)
{
	m_strActiveGrp = strLyr;
}

bool GeoScene::toMapPoint(int x, int y, double &dlon, double &dlat, double &dele)
{
	osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
		new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
	osgUtil::IntersectionVisitor iv(intersector.get());
	_viewer->getCamera()->accept(iv);
	if (intersector->containsIntersections())
	{
		osgEarth::GeoPoint map;
		osg::Vec3d  v = intersector->getIntersections().begin()->getWorldIntersectPoint();
		map.fromWorld(_mapNode->getMapSRS(), v);
		dlon = map.x();
		dlat = map.y();
		dele = map.z();
		return true;
	}
	return false;
}

bool GeoScene::fromMapPoint(double dlon, double dlat, double dele, int &x, int &y)
{
	osgEarth::GeoPoint pnt(_mapNode->getMapSRS(), dlon, dlat, dele,0);

	osg::Vec3d vWorld;
	pnt.toWorld(vWorld);
	osg::Camera* cam = _viewer->getCamera();
	osg::Matrix VPW = cam->getViewMatrix() *
		cam->getProjectionMatrix() *
		cam->getViewport()->computeWindowMatrix();
	osg::Vec3d vWin = vWorld * VPW;
	float wy = cam->getViewport()->height() - vWin.y();
	float wx = vWin.x();
	x = wx;
	y = wy;
	return true;
}

void GeoScene::startEdit(std::string grp,std::string id)
{
	osgEarth::Annotation::PositionedAnnotationNode* nd = dynamic_cast<osgEarth::Annotation::PositionedAnnotationNode*>(_annObjs[grp][id]);
	if(nd){
		_editors->addChild(new PlaceNodeEditor(nd));
	}

}

osg::Group* getorcreateChild(osg::Group* node,std::string strGrp)
{
	osg::ref_ptr<osg::Group> grp;
	for(int i=0;i<node->getNumChildren();i++){
		if(node->getChild(i)->getName()==strGrp){
			grp = dynamic_cast<osg::Group*>(node->getChild(i));
			if(grp.valid())
				return grp.release();
		}
	}
	grp = new osg::Group;
	grp->setName(strGrp);
	node->addChild(grp);
	return grp.release();
}

class SetDeclutterOperation : public osg::Operation
{
public:
	SetDeclutterOperation( osg::Node* pNode):m_refNode(pNode){
		if(m_refNode.valid()){
				osgEarth::Annotation::Decluttering::setEnabled( m_refNode->getOrCreateStateSet(), true );
			}
	}

	virtual void operator()( osg::Object* ){

	}

protected:
	virtual ~SetDeclutterOperation(){}

protected:
	osg::ref_ptr<osg::Node> m_refNode;

private:
};

void GeoScene::setDeclutter(std::string grp)
{
	osg::ref_ptr<osg::Group> nd = getorcreateChild(_mapNode,grp);
	if(nd.valid()){
		addUpdateOper(new SetDeclutterOperation(nd.get()));
		//osgEarth::Annotation::Decluttering::setEnabled( nd->getOrCreateStateSet(), true );
	}
}

void GeoScene::setViewPoint(const osgEarth::Viewpoint &v,double duration)
{
	if(_manip){
		_manip->setViewpoint(v, duration);
	}
}

osgEarth::Viewpoint GeoScene::getViewpoint()
{
	if(_manip){
		return _manip->getViewpoint();
	}
}
