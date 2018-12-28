/* -*-c++-*- */
/* osgEarth - Dynamic map generation toolkit for OpenSceneGraph
* Copyright 2008-2014 Pelican Mapping
* http://osgearth.org
*
* osgEarth is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "QuadTileSource.h"
#include <osgEarth/ImageUtils>
#include <osgEarth/FileUtils>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Drivers::TileMapService;

#define LC "[QuadTileSource] "


bool
isPathToArchivedFile(const std::string& path)
{
    osgDB::Registry::ArchiveExtensionList list = osgDB::Registry::instance()->getArchiveExtensions();
    for( osgDB::Registry::ArchiveExtensionList::const_iterator i = list.begin(); i != list.end(); ++i )
    {
        if (path.find("."+*i+"/")  != std::string::npos ||
            path.find("."+*i+"\\") != std::string::npos)
        {
            return true;
        }
    }
    return false;
}

QuadTileSource::QuadTileSource(const TileSourceOptions& options) :
TileSource(options),
_options  (options),
_forceRGB (false)
{
    _invertY = true;
}

TileSource::Status
QuadTileSource::initialize(const osgDB::Options* dbOptions)
{
    // local copy of options we can modify if necessary.
    _dbOptions = Registry::instance()->cloneOrCreateOptions(dbOptions);

    // see if the use passed in a profile
    //const Profile* profile = getProfile();
	//geobeans
	//const Profile* profile = Profile::create(
	//	SpatialReference::get("wgs84"),
	//	-256, -256, 256, 256,
	//	2, 2);

	//google test
	const Profile* profile = Profile::create(
		SpatialReference::get("spherical-mercator"),
		MERC_MINX, MERC_MINY, MERC_MAXX, MERC_MAXY,
		2, 2);

	setProfile(profile);

    URI tmsURI = _options.url().value();
    if ( tmsURI.empty() )
    {
        return Status::Error( "Fail: TMS driver requires a valid \"url\" property" );
    }

    // Take the override profile if one is given
    if ( profile )
    {
        OE_INFO << LC 
            << "Using override profile \"" << getProfile()->toString()
            << "\" for URI \"" << tmsURI.base() << "\"" 
            << std::endl;

        _tileMap = TMS::TileMap::create( 
            _options.url()->full(),
            profile,
            _options.format().value(),
            _options.tileSize().value(), 
            _options.tileSize().value() );
    }

    else
    {
        // Attempt to read the tile map parameters from a TMS TileMap XML tile on the server:
        _tileMap = TMS::TileMapReaderWriter::read( tmsURI.full(), _dbOptions.get() );
        if (!_tileMap.valid())
        {
            return Status::Error( Stringify() << "Failed to read tilemap from " << tmsURI.full() );
        }

        profile = _tileMap->createProfile();
        if ( profile )
            setProfile( profile );
    }

    // Make sure we've established a profile by this point:
    if ( !profile )
    {
        return Status::Error( Stringify() << "Failed to establish a profile for " << tmsURI.full() );
    }

    // TileMap and profile are valid at this point. Build the tile sets.
    // Automatically set the min and max level of the TileMap
    if ( _tileMap->getTileSets().size() > 0 )
    {
        OE_DEBUG << LC << "TileMap min/max " << _tileMap->getMinLevel() << ", " << _tileMap->getMaxLevel() << std::endl;
        if (_tileMap->getDataExtents().size() > 0)
        {
            for (DataExtentList::iterator itr = _tileMap->getDataExtents().begin(); itr != _tileMap->getDataExtents().end(); ++itr)
            {
                this->getDataExtents().push_back(*itr);
            }
        }
        else
        {
            //Push back a single area that encompasses the whole profile going up to the max level
            this->getDataExtents().push_back(DataExtent(profile->getExtent(), 0, _tileMap->getMaxLevel()));
        }
    }

    // set up the IO options so that we do not cache TMS tiles:
    CachePolicy::NO_CACHE.apply( _dbOptions.get() );

    return STATUS_OK;
}


//TimeStamp
//QuadTileSource::getLastModifiedTime() const
//{
//    if ( _tileMap.valid() )
//        return _tileMap->getTimeStamp();
//    else
//        return TileSource::getLastModifiedTime();
//}

std::string
QuadTileSource::getURL(const osgEarth::TileKey& tilekey, bool invertY)
{
	if (!_tileMap->intersectsKey(tilekey))
	{
		//OE_NOTICE << LC << "No key intersection for tile key " << tilekey.str() << std::endl;
		return "";
	}

	int zoom = tilekey.getLevelOfDetail();

	unsigned int tile_x, tile_y;
	tilekey.getTileXY(tile_x, tile_y);

	//0,0 at the bottom left,so inverting Y will make 0,0 in the uper left
	if (!_invertY)
	{
		unsigned int numRows, numCols;
		tilekey.getProfile()->getNumTiles(tilekey.getLevelOfDetail(), numCols, numRows);
		tile_y = numRows - tile_y - 1;
	}
	
	std::stringstream ss;
	int _zoom = zoom+1;
	unsigned int x = tile_x;
	//unsigned int y = tile_y + std::pow(2.0, zoom);
	unsigned int y =  tile_y;

	//GeoBeans
	//ss << _options.url()->full() << "&level=" << _zoom << "&col=" << x << "&row=" << y << "&rowModel=d&errorType=icon";
	
	//google test
	ss << _options.url()->full() << "&x=" << x << "&y=" << y << "&z=" << _zoom ;

	std::string ssStr;
	ssStr = ss.str();
	return ssStr;
}

osg::Image*
QuadTileSource::createImage(const TileKey&    key,
                           ProgressCallback* progress)
{
    //if (_tileMap.valid() && key.getLevelOfDetail() <= _tileMap->getMaxLevel() )
	if (_tileMap.valid() && key.getLevelOfDetail() <= 18)
    {
        std::string image_url = getURL( key, _invertY );

        osg::ref_ptr<osg::Image> image;
        if (!image_url.empty())
        {
            image = URI(image_url).readImage( _dbOptions.get(), progress ).getImage();
        }

        if (!image.valid())
        {
			return 0;
            if (image_url.empty() || !_tileMap->intersectsKey(key))
            {
                //We couldn't read the image from the URL or the cache, so check to see if the given key is less than the max level
                //of the tilemap and create a transparent image.
                if (key.getLevelOfDetail() <= _tileMap->getMaxLevel())
                {
                    OE_DEBUG << LC << "Returning empty image " << std::endl;
                    return ImageUtils::createEmptyImage();
                }
            }
        }
		//else
		//{
		//	if (ImageUtils::isSingleColorImage(image)) {
		//		return 0;
		//	}
		//}
        return image.release();
    }
    return 0;
}

int
QuadTileSource::getPixelsPerTile() const
{
    return _tileMap->getFormat().getWidth();
}

std::string
QuadTileSource::getExtension() const
{
    return _tileMap->getFormat().getExtension();
}

bool
QuadTileSource::resolveWriter()
{
    _writer = osgDB::Registry::instance()->getReaderWriterForMimeType(
        _tileMap->getFormat().getMimeType());

    if ( !_writer.valid() )
    {
        _writer = osgDB::Registry::instance()->getReaderWriterForExtension(
            _tileMap->getFormat().getExtension());

        if ( !_writer.valid() )
        {
            _writer = osgDB::Registry::instance()->getReaderWriterForExtension(
                _options.format().value() );
        }
    }

    // The OSG JPEG writer does not accept RGBA images, so force conversion.
    _forceRGB =
        _writer.valid() &&
        (_writer->acceptsExtension("jpeg") || _writer->acceptsExtension("jpg") );

    if ( _forceRGB )
    {
        OE_INFO << LC << "Note: images will be stored as RGB" << std::endl;
    }

    return _writer.valid();
}
