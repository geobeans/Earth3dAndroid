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
#ifndef OSGEARTH_DRIVER_QUAD_MAPLAYERFACTORY
#define OSGEARTH_DRIVER_QUAD_MAPLAYERFACTORY 1

#include <osgEarth/Common>
#include <osgEarth/TileSource>
#include <osgEarth/URI>

namespace osgEarth { namespace Drivers
{
    using namespace osgEarth;

    class QuadOptions : public TileSourceOptions // NO EXPORT; header only
    {
    public:
        optional<URI>& url() { return _url; }
        const optional<URI>& url() const { return _url; }

        optional<std::string>& tmsType() { return _tmsType; }
        const optional<std::string>& tmsType() const { return _tmsType; }

        optional<std::string>& format() { return _format; }
        const optional<std::string>& format() const { return _format; }

    public:
		QuadOptions( const TileSourceOptions& opt =TileSourceOptions() ) : TileSourceOptions( opt )
        {
            setDriver( "geobeans_quad" );
            fromConfig( _conf );
        }

		QuadOptions( const std::string& inUrl ) : TileSourceOptions()
        {
            setDriver( "geobeans_quad" );
            fromConfig( _conf );
            url() = inUrl;
        }

        /** dtor */
        virtual ~QuadOptions() { }

    public:
        Config getConfig() const {
            Config conf = TileSourceOptions::getConfig();
            conf.updateIfSet("url", _url);
            conf.updateIfSet("tms_type", _tmsType);
            conf.updateIfSet("format", _format);
            return conf;
        }

    protected:
        void mergeConfig( const Config& conf ) {
            TileSourceOptions::mergeConfig( conf );
            fromConfig( conf );
        }

    private:
        void fromConfig( const Config& conf ) {
            conf.getIfSet( "url", _url );
            conf.getIfSet( "format", _format );
            conf.getIfSet( "tms_type", _tmsType );
        }

        optional<URI>         _url;
        optional<std::string> _tmsType;
        optional<std::string> _format;
    };

} } // namespace osgEarth::Drivers

#endif // OSGEARTH_DRIVER_QUAD_MAPLAYERFACTORY

