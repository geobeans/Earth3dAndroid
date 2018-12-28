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
#ifndef OSGEARTH_DRIVERS_QUAD_TMSTILESOURCE
#define OSGEARTH_DRIVERS_QUAD_TMSTILESOURCE 1

#include "QuadOptions.h"

#include <osgEarth/TileSource>
#include <osgEarth/Registry>
#include <osgEarthUtil/TMS>

#include <osg/Notify>

using namespace osgEarth;
using namespace osgEarth::Util;


namespace osgEarth { namespace Drivers { namespace TileMapService
{
    class QuadTileSource : public TileSource
    {
    public:
		QuadTileSource(const TileSourceOptions& options);

    public: // TileSource

        // one-time initialization (per source)
        Status initialize(const osgDB::Options* dbOptions);

        // TileSource timestamp is the time of the time map metadata.
        //TimeStamp getLastModifiedTime() const;

        // reflect a default cache policy based on whether this TMS repo is
        // local or remote.
        //CachePolicy getCachePolicyHint(const Profile* targetProfile) const;

        // creates an image from the TMS repo.
        osg::Image* createImage(
            const             TileKey& key, 
            ProgressCallback* progress);

        int getPixelsPerTile() const;

        virtual std::string getExtension() const;

    private:
		std::string getURL(const osgEarth::TileKey& tilekey, bool invertY);
        osg::ref_ptr<TMS::TileMap>        _tileMap;
        bool                              _invertY;
        const QuadOptions                  _options;
        osg::ref_ptr<osgDB::Options>      _dbOptions;
        osg::ref_ptr<osgDB::ReaderWriter> _writer;
        bool                              _forceRGB;

        bool resolveWriter();
    };

} } } // namespace osgEarth::Drivers::TileMapService

#endif // OSGEARTH_DRIVERS_QUAD_TMSTILESOURCE
