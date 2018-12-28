//
//  osgEarthDemo.cpp
//  osgearthDemos
//

#include <GeoMainApp.hpp>
#include <GeoScene.h>
#include "GLES2ShaderGenVisitor.h"

#include <osg/linewidth>

#include <osgEarth/GeoMath>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthUtil/AutoClipPlaneHandler>

#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/AnnotationRegistry>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/TrackNode>
#include <osgEarthAnnotation/Decluttering>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>

#include <osgEarthSymbology/GeometryFactory>

#include <osgEarth/TerrainEngineNode>
#include <osgEarth/TerrainOptions>

#include <osgEarthDrivers/wms/SceneGraphChanges>

#include <osgEarth/Notify>

#include <osgUtil/Optimizer>
#include <osgEarthUtil/ObjectPlacer>

#include <osgEarth/EntityCache>
#include <osgViewer/ViewerEventHandlers>

#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarthAnnotation/ModelNode>

#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#include <osgEarthUtil/MGRSFormatter>

#include "PickHandler.h"
#include "SceneGraphChangeCallback.hpp"
#include "Compass.h"
#include "QuadOptions.h"

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Drivers;
using namespace osgEarth::Features;
using namespace osgEarth::Symbology;
using namespace osgEarth::Util;

// field names for the track labels
#define FIELD_NAME     "name"
#define FIELD_POSITION "position"
#define FIELD_NUMBER   "number"

// icon to use, and size in pixels
#define ICON_URL       "../data/m2525_air.png"
#define ICON_SIZE      40

// format coordinates as MGRS
static MGRSFormatter s_format(MGRSFormatter::PRECISION_10000M);

// globals for this demo
osg::StateSet*      g_declutterStateSet = 0L;
bool                g_showCoords        = true;
optional<float>     g_duration          = 60.0;
unsigned            g_numTracks         = 100;

/** A little track simulator that goes a simple great circle interpolation */
struct TrackSim : public osg::Referenced
{
    //osgEarth::Annotation::TrackNode* _track;
    double _startLat, _startLon, _endLat, _endLon;
    osgEarth::Annotation::ModelNode* _model;
    double _height;

    void update( double t )
    {
        osg::Vec3d pos;
//        GeoMath::interpolate(
//            _startLat.as(Units::RADIANS), _startLon.as(Units::RADIANS),
//            _endLat.as(Units::RADIANS), _endLon.as(Units::RADIANS),
//            t,
//            pos.y(), pos.x() );
        GeoMath::interpolate(
            osg::DegreesToRadians(_startLat), osg::DegreesToRadians(_startLon),
			osg::DegreesToRadians(_endLat), osg::DegreesToRadians(_endLon),
            t,
            pos.y(), pos.x() );

//        GeoPoint geo(
//        	_model->getMapNode()->getMapSRS(),
//            osg::RadiansToDegrees(pos.x()),
//            osg::RadiansToDegrees(pos.y()),
//			_height,
//            ALTMODE_ABSOLUTE);

        GeoPoint geo(
        	_model->getMapNode()->getMapSRS(),
            osg::RadiansToDegrees(pos.x()),
            osg::RadiansToDegrees(pos.y()));

        // update the position label.
//        _track->setPosition(geo);
        _model->setPosition(geo);

//        if ( g_showCoords )
//        {
//            _track->setFieldValue( FIELD_POSITION, s_format(geo) );
//        }
//        else
//            _track->setFieldValue( FIELD_POSITION, "" );
    }
};
typedef std::vector< TrackSim* > TrackSims;
TrackSims g_trackSims;
/** Update operation that runs the simulators. */
struct TrackSimUpdate : public osg::Operation
{
    TrackSimUpdate(TrackSims& sims) : osg::Operation( "tasksim", true ), _sims(sims) { }

    void operator()( osg::Object* obj ) {
        osg::View* view = dynamic_cast<osg::View*>(obj);
        double t = fmod(view->getFrameStamp()->getSimulationTime(), (double)g_duration.get()) / (double)g_duration.get();
        for( TrackSims::iterator i = _sims.begin(); i != _sims.end(); ++i )
            (*i)->update( t );
    }

    TrackSims& _sims;
};

/**
 * Creates a field schema that we'll later use as a labeling template for
 * TrackNode instances.
 */
void
createFieldSchema( TrackNodeFieldSchema& schema )
{
    // draw the track name above the icon:
    TextSymbol* nameSymbol = new TextSymbol();
    nameSymbol->pixelOffset()->set( 0, 2+ICON_SIZE/2 );
    nameSymbol->alignment() = TextSymbol::ALIGN_CENTER_BOTTOM;
    nameSymbol->halo()->color() = Color::Black;
    nameSymbol->size() = nameSymbol->size().value() + 2.0f;
    schema[FIELD_NAME] = TrackNodeField(nameSymbol, false); // false => static label (won't change after set)

    // draw the track coordinates below the icon:
    TextSymbol* posSymbol = new TextSymbol();
    posSymbol->pixelOffset()->set( 0, -2-ICON_SIZE/2 );
    posSymbol->alignment() = TextSymbol::ALIGN_CENTER_TOP;
    posSymbol->fill()->color() = Color::Yellow;
    posSymbol->size() = posSymbol->size().value() - 2.0f;
    schema[FIELD_POSITION] = TrackNodeField(posSymbol, true); // true => may change at runtime

    // draw some other field to the left:
    TextSymbol* numberSymbol = new TextSymbol();
    numberSymbol->pixelOffset()->set( -2-ICON_SIZE/2, 0 );
    numberSymbol->alignment() = TextSymbol::ALIGN_RIGHT_CENTER;
    schema[FIELD_NUMBER] = TrackNodeField(numberSymbol, false);
}


/** Builds a bunch of tracks. */
void
createTrackNodes( MapNode* mapNode, osg::Group* parent, const TrackNodeFieldSchema& schema, TrackSims& sims )
{
    // load an icon to use:
//    osg::ref_ptr<osg::Image> srcImage = osgDB::readImageFile( ICON_URL );
//    osg::ref_ptr<osg::Image> image;
//    ImageUtils::resizeImage( srcImage.get(), ICON_SIZE, ICON_SIZE, image );

    // make some tracks, choosing a random simulation for each.
    Random prng;
    const SpatialReference* geoSRS = mapNode->getMapSRS();
	osg::Node* mdl = osgDB::readNodeFile("/storage/emulated/0/tank.osg.100,100,100.scale");
    for( unsigned i=0; i<g_numTracks; ++i )
    {
		double lon0 = 115.2 + prng.next() * 1.0;
		double lat0 = 39.6 + prng.next() * 0.7;

        GeoPoint pos(geoSRS, lon0, lat0);

        TrackNode* track = new TrackNode(mapNode, pos, NULL, schema);

        track->setFieldValue( FIELD_NAME,     Stringify() << "Track:" << i );
        track->setFieldValue( FIELD_POSITION, Stringify() << s_format(pos) );
        track->setFieldValue( FIELD_NUMBER,   Stringify() << (1 + prng.next(9)) );

        // add a priority
        AnnotationData* data = new AnnotationData();
        data->setPriority( float(i) );
        track->setAnnotationData( data );

        //parent->addChild( track );

		osgEarth::Symbology::Style style;
		style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mdl);
		osgEarth::Annotation::ModelNode* model = new osgEarth::Annotation::ModelNode(mapNode, style);
		//model->setPosition(osgEarth::GeoPoint(mapNode->getMap()->getSRS(), osg::Vec3d(lon0, lat0, 10000), osgEarth::ALTMODE_ABSOLUTE));
		model->setPosition(pos);
		parent->addChild(model);

        // add a simulator for this guy
		double lon1 = 115.2 + prng.next() * 1.0;
		double lat1 = 39.6 + prng.next() * 0.7;
        TrackSim* sim = new TrackSim();
        //sim->_track = track;
        sim->_model = model;
        sim->_startLat = lat0; sim->_startLon = lon0;
        sim->_endLat = lat1; sim->_endLon = lon1;
		double fw = GeoMath::bearing(osg::DegreesToRadians(lat0), osg::DegreesToRadians(lon0),
			osg::DegreesToRadians(lat1), osg::DegreesToRadians(lon1));
		fw = osg::RadiansToDegrees(fw);
		osg::Quat qu = osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(0, 1, 0)) *
			osg::Quat(osg::DegreesToRadians(0.0), osg::Vec3(1, 0, 0)) *
			osg::Quat(osg::DegreesToRadians(fw), osg::Vec3(0, 0, -1));

		model->setLocalRotation(qu);
		sim->_height = 2000 + prng.next() * 8000;

        sims.push_back( sim );
    }
}

osgEarth::Util::MapNode* GeoScene::getMapNode(){
	return _mapNode.get();
}

void GeoScene::addDemoLine(osgEarth::Map* map)
{
    // create a feature source to load the street shapefile.
    OGRFeatureOptions feature_opt;
    feature_opt.name() = "streets";
    feature_opt.url() = "/storage/emulated/0/shp/dt.shp";
    //feature_opt.buildSpatialIndex() = true;

    // a resampling filter will ensure that the length of each segment falls
    // within the specified range. That can be helpful to avoid cropping
    // very long lines segments.
    //feature_opt.filters().push_back( new ResampleFilter(0.0, 25.0) );

    // a style:
    Style style;
    style.setName( "streets" );

    // Render the data as translucent yellow lines that are 7.5m wide.
    LineSymbol* line = style.getOrCreate<LineSymbol>();
    line->stroke()->color() = Color(Color::Yellow, 1.0f);
    line->stroke()->width() = 8.0f;
    //line->stroke()->widthUnits() = Units::METERS;

    // Clamp the lines to the terrain.
    AltitudeSymbol* alt = style.getOrCreate<AltitudeSymbol>();
    alt->clamping() = alt->CLAMP_TO_TERRAIN;
    //alt->technique() = AltitudeSymbol::TECHNIQUE_GPU;
    // Apply a depth offset to avoid z-fighting. The "min bias" is the minimum
    // apparent offset (towards the camera) of the geometry from its actual position.
    // The value here was chosen empirically by tweaking the "oe_doff_min_bias" uniform.
    RenderSymbol* render = style.getOrCreate<RenderSymbol>();
    render->depthOffset()->minBias() = 6.6f;

    // Set up a paging layout. The tile size factor and the visibility range combine
    // to determine the tile size, such that tile radius = max range / tile size factor.
//    FeatureDisplayLayout layout;
//    layout.tileSizeFactor() = 7.5f;
//    layout.maxRange()       = 5000.0f;

    // create a model layer that will render the buildings according to our style sheet.
    FeatureGeomModelOptions fgm_opt;
    fgm_opt.featureOptions() = feature_opt;
    //fgm_opt.layout() = layout;
    fgm_opt.styles() = new StyleSheet();
    fgm_opt.styles()->addStyle( style );

    map->addModelLayer( new ModelLayer("streets", fgm_opt) );
}

void GeoScene::initDemo(const std::string &file)
{
    OSG_ALWAYS << "----osgEarthDemo----" << std::endl;

    _viewer->setCameraManipulator(  new osgEarth::Util::EarthManipulator() );//new osgEarth::Util::EarthMultiTouchManipulator() );

    osgEarth::Drivers::FileSystemCacheOptions cacheOpts;
    cacheOpts.rootPath() = "/storage/emulated/0/geobeans/";
    osgEarth::MapOptions mapOpts;
    mapOpts.cache() = cacheOpts;
    osgEarth::Map* map = new osgEarth::Map(mapOpts);
    //Map* map = new Map();
    _mapNode = new MapNode( map );
    //setBaseLayerTMS("http://readymap.org/readymap/tiles/1.0.0/7/");

	osgEarth::Drivers::QuadOptions tmsGg;
	//tmsGg.url() = "http://www.google.cn/maps/vt?lyrs=s&gl=cn";
	tmsGg.url() = "http://khm0.google.com/kh/v=815&hl=zh";
	osgEarth::ImageLayer* layerGg = new osgEarth::ImageLayer( "BASE", tmsGg );
	_mapNode->getMap()->addImageLayer( layerGg );

	_gles2ShaderGen->apply(*_mapNode);

    osgEarth::Drivers::TMSOptions elevation;
    elevation.url() = "http://readymap.org/readymap/tiles/1.0.0/9/";
    map->addElevationLayer( new ElevationLayer("ReadyMap elevation", elevation) );

    {
		jclass clazz = _genv->FindClass("cn/geobeans/earth/GEarth");
		if( !clazz )
		{
			OE_WARN << "[Demoscene tap]\tJNI getClass error." << std::endl;
			return ;
		}
		jmethodID method = _genv->GetStaticMethodID(clazz, "fireInit", "()V");
		if( !method )
		{
			OE_WARN << "[Demoscene tap]\tJNI getMethod error." << std::endl;
			return ;
		}

		_genv->CallStaticVoidMethod( _gjo, method); // @suppress("Invalid arguments")
	}

    //addDemoLine(map);
//    {
//    osgUtil::GLES2ShaderGenVisitor* gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();
// 	osg::Node* sw_model = osgDB::readNodeFile("/storage/emulated/0/tanmen/tanmen.osg");
//	Style st;
//	st.getOrCreate<ModelSymbol>()->setModel(sw_model);
	const SpatialReference* latLong = SpatialReference::get("wgs84");
//	osgEarth::Annotation::ModelNode* modelNode = new osgEarth::Annotation::ModelNode(_mapNode, st);
//	modelNode->setPosition(GeoPoint(latLong, 116.391854043, 39.906457831));
//	_mapNode->addChild(modelNode);
//
////	osgEarth::Util::ObjectPlacer* objPlacer = new osgEarth::Util::ObjectPlacer( _mapNode );
////	osg::Node* hzlOnEarth = objPlacer->placeNode( sw_model,39.906457831,116.390854043, 400.0 );//lat,lon表示模型在地球的纬度和经度,elevation是海拔
////	_mapNode->addChild( hzlOnEarth );
//
// 	osg::Node* jnb_model = osgDB::readNodeFile("/storage/emulated/0/jinianbe/jinianb.osg");
//	Style stJnb;
//	stJnb.getOrCreate<ModelSymbol>()->setModel(jnb_model);
//	osgEarth::Annotation::ModelNode* jnbNode = new osgEarth::Annotation::ModelNode(_mapNode, stJnb);
//	jnbNode->setPosition(GeoPoint(latLong, 116.391854043, 39.902957831));
//	_mapNode->addChild(jnbNode);
//    }


	_manip = dynamic_cast<osgEarth::Util::EarthManipulator*>(_viewer->getCameraManipulator());
    if ( _manip == 0L )
    {
        OSG_ALWAYS << "Helper used before installing an EarthManipulator" << std::endl;
    }

    _root = new osg::Group();
    _root->setThreadSafeRefUnref(true);

    osgUtil::Optimizer optimizer;

    EntityCache* entityCache = new EntityCache();
    _kml_options = new osgEarth::Drivers::KMLOptions();
    _kml_options->declutter() = true;
    _kml_options->entityCache = entityCache;

    _changesQueue = new SceneGraphChanges();
    SceneGraphChangeCallback* chcb = new SceneGraphChangeCallback();
    _mapNode->setUserData( _changesQueue );
    _mapNode->setUpdateCallback(chcb);

//    osg::ref_ptr<osg::Group> grp = new osg::Group;
//    grp->setName("place");
//    _mapNode->addChild(grp);
//    Decluttering::setEnabled( grp->getOrCreateStateSet(), true );

    //osgUtil::GLES2ShaderGenVisitor* gles2ShaderGen = new osgUtil::GLES2ShaderGenVisitor();

    osg::Light* light = new osg::Light( 0 );
    light->setPosition( osg::Vec4(0, -1, 0, 0 ) );
    light->setAmbient( osg::Vec4(0.4f, 0.4f, 0.4f ,1.0) );
    light->setDiffuse( osg::Vec4(1,1,1,1) );
    light->setSpecular( osg::Vec4(0,0,0,1) );
    _root->getOrCreateStateSet()->setAttribute(light);
    
    //have to add these
    osg::Material* material = new osg::Material();
    material->setAmbient(osg::Material::FRONT, osg::Vec4(0.4,0.4,0.4,1.0));
    material->setDiffuse(osg::Material::FRONT, osg::Vec4(0.9,0.9,0.9,1.0));
    material->setSpecular(osg::Material::FRONT, osg::Vec4(0.4,0.4,0.4,1.0));
    _root->getOrCreateStateSet()->setAttribute(material);
//    _root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
    
    double hours = 3.0f;
    float ambientBrightness = 0.4f;
    osgEarth::Util::SkyNode* sky = new osgEarth::Util::SkyNode( _mapNode->getMap() );
    sky->setAmbientBrightness( ambientBrightness );
    sky->setDateTime( 1984, 11, 8, hours );
    sky->attach( _viewer, 0 );
    sky->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

    _mapNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //_mapNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    //modelNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    //hzlOnEarth->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    _root->addChild( sky );
    _root->addChild( _mapNode );

    _editors = new osg::Group;
    _mapNode->addChild(_editors);
//    {
//	osg::Group* labelGroup = new osg::Group();
////	Decluttering::setEnabled(labelGroup->getOrCreateStateSet(), true);
//	_root->addChild(labelGroup);
//	Style pin;
//	pin.getOrCreate<IconSymbol>()->url()->setLiteral("/storage/emulated/0/placemark32.png");
////
//	// bunch of pins:
//	labelGroup->addChild(new PlaceNode(_mapNode, GeoPoint(latLong, 116.390854043, 39.908457831), "Beijing", pin));
//	labelGroup->addChild(new PlaceNode(_mapNode, GeoPoint(latLong, 113.40, 40.0), "Datong", pin));
//	labelGroup->addChild(new PlaceNode(_mapNode, GeoPoint(latLong, 115.49, 38.88), "Baoding", pin));
//	labelGroup->addChild(new PlaceNode(_mapNode, GeoPoint(latLong, 114.515, 38.046), "Shijiazhuang", pin));
////
//    }

	 // a box that follows lines of latitude (rhumb line interpolation, the default)
//	{
//		Geometry* geom = new Polygon();
//		geom->push_back( osg::Vec3d(0,   40, 0) );
//		geom->push_back( osg::Vec3d(-60, 40, 0) );
//		geom->push_back( osg::Vec3d(-60, 60, 0) );
//		geom->push_back( osg::Vec3d(0,   60, 0) );
//		Style geomStyle;
//		geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
//		geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 15.0f;
//		geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
//		FeatureNode* gnode = new FeatureNode(_mapNode, new Feature(geom, latLong, geomStyle));
//		//gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
//
//		_mapNode->addChild( gnode );
//	}
//    // A path using great-circle interpolation.
//    {
//        Geometry* path = new LineString();
//        path->push_back( osg::Vec3d(-74, 40.714, 0) );   // New York
//        path->push_back( osg::Vec3d(139.75, 35.68, 0) ); // Tokyo
//
//        Style pathStyle;
//        pathStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
//        pathStyle.getOrCreate<LineSymbol>()->stroke()->width() = 8.0f;
//        pathStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
//
//        Feature* pathFeature = new Feature(path, latLong, pathStyle);
//        pathFeature->geoInterp() = GEOINTERP_GREAT_CIRCLE;
//
//        //OE_INFO << "Path extent = " << pathFeature->getExtent().toString() << std::endl;
//
//        FeatureNode* pathNode = new FeatureNode(_mapNode, pathFeature);
//        _mapNode->addChild( pathNode );
//
//    }
//
//    Style ellipseStyle;
//       ellipseStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Orange, 1.0);
//       EllipseNode* ellipse = new EllipseNode(
//           _mapNode,
//           GeoPoint(latLong, 116.0,40.0,1500.0,ALTMODE_ABSOLUTE),
//           Linear(500, Units::MILES),
//           Linear(100, Units::MILES),
//           Angular(0, Units::DEGREES),
//           ellipseStyle,
//           true );
//       _mapNode->addChild(ellipse);

    osg::ref_ptr<PickHandler> picker = new PickHandler(_root, _genv, _gjo, _viewer.get());
    osgGA::GUIEventHandler* ptr = picker.get();

    //optimizer.optimize(_mapNode.get());
    //gles2ShaderGen->apply(*_root);

    _viewer->setSceneData( _root );

    _viewer->addEventHandler( ptr );

    //_manip->setHomeViewpoint(osgEarth::Viewpoint("Home", osg::Vec3d(-8.39753f, 41.56094f, 0.0 ), 0.0, 0.0, 50 ));
    _manip->setHomeViewpoint(osgEarth::Viewpoint("Home", osg::Vec3d(115.7, 39.95, 100.0 ), 0.0, -45.0, 100000.0 ));
    //addDemoLine(map);

    // build a track field schema.  produce tank battle!lijp
//     TrackNodeFieldSchema schema;
//     createFieldSchema( schema );
//
//     // create some track nodes.
//     osg::Group* tracks = new osg::Group();
//     createTrackNodes( _mapNode, tracks, schema, g_trackSims );
//     _root->addChild( tracks );
    // build a track field schema.  produce tank battle!lijp end


     // Set up the automatic decluttering. setEnabled() activates decluttering for
     // all drawables under that state set. We are also activating priority-based
     // sorting, which looks at the AnnotationData::priority field for each drawable.
     // (By default, objects are sorted by disatnce-to-camera.) Finally, we customize
     // a couple of the decluttering options to get the animation effects we want.
//     g_declutterStateSet = tracks->getOrCreateStateSet();
//     Decluttering::setEnabled( g_declutterStateSet, true );
//     g_dcOptions = Decluttering::getOptions();
//     g_dcOptions.inAnimationTime()  = 1.0f;
//     g_dcOptions.outAnimationTime() = 1.0f;
//     g_dcOptions.sortByPriority()   = true;
//     Decluttering::setOptions( g_dcOptions );

     // create LINES  start!
//        {
//    	    osg::Geode* geode = new osg::Geode();
//    	    _mapNode->addChild(geode);
//            // create Geometry object to store all the vertices and lines primitive.
//            osg::Geometry* linesGeom = new osg::Geometry();
//
//            osg::ref_ptr<osg::LineWidth> width = new osg::LineWidth;
//            osg::Vec3d startWorld;
//            osg::Vec3d endWorld;
//
//            GeoPoint startL(latLong,116.0,40.0,1500.0,ALTMODE_ABSOLUTE);
//            GeoPoint endL(latLong,118.0,40.0,1500.0,ALTMODE_ABSOLUTE);
//
//            startL.toWorld(startWorld);
//            endL.toWorld(endWorld);
//            // this time we'll preallocate the vertex array to the size we
//            // need and then simple set them as array elements, 8 points
//            // makes 4 line segments.
//            osg::Vec3Array* vertices = new osg::Vec3Array(2);
//            (*vertices)[0].set(startWorld);
//            (*vertices)[1].set(endWorld);
//
//            // pass the created vertex array to the points geometry object.
//            linesGeom->setVertexArray(vertices);
//
//            // set the colors as before, plus using the above
//            osg::Vec4Array* colors = new osg::Vec4Array;
//            colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
//            linesGeom->setColorArray(colors);
//            linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
//
//
//            // set the normal in the same way color.
//            osg::Vec3Array* normals = new osg::Vec3Array;
//            normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
//            linesGeom->setNormalArray(normals);
//            linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);
//
//            // This time we simply use primitive, and hardwire the number of coords to use
//            // since we know up front,
//            linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
//
//            width->setWidth(20.0);
//            linesGeom->getOrCreateStateSet()->setAttributeAndModes(width);
//            // add the points geometry to the geode.
//            geode->addDrawable(linesGeom);
//            //gles2ShaderGen->apply(*geode);
//        }
    // create LINES  end!!!

    addCompass();
     // attach the simulator to the viewer.
     _viewer->addUpdateOperation( new TrackSimUpdate(g_trackSims) );
     _viewer->setRunFrameScheme( _viewer->CONTINUOUS );

    _viewer->realize();

    //_viewer->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(_mapNode));
}

osg::MatrixTransform* GeoScene::createCompassPart(const std::string& image, float radius, float height)
{
	osg::Vec3 center(-radius, -radius, height);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(
		createTexturedQuadGeometry(center, osg::Vec3(radius*2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius*2.0f, 0.0f)));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(image));

	osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
	part->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	part->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	part->addChild(geode.get());
	_gles2ShaderGen->apply(*part);
	return part.release();
}

void GeoScene::addCompass()
{
	osg::ref_ptr<Compass> compass = new Compass;
	osg::Camera* cam = _viewer->getCamera();
	int h = cam->getViewport()->height();
	compass->setViewport(0.0, h-128, 128.0, 128.0);
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	std::string strCompass = "/storage/emulated/0/compass.png";
	compass->setPlate(createCompassPart(strCompass, 1.5f, -1.0f));
	compass->setMainCamera(cam);

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

    _mapNode->addChild(compass.get());
}
