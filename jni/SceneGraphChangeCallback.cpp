#include "SceneGraphChangeCallback.hpp"
#include <osgEarthDrivers/wms/SceneGraphChanges>
#include <osgEarthDrivers/wms/GeoNode>

#include <osg/Group>

SceneGraphChangeCallback::SceneGraphChangeCallback(){}

osg::Group* SceneGraphChangeCallback::getorcreateChild(osg::Group* node,std::string strGrp)
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

void SceneGraphChangeCallback::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	osg::ref_ptr<SceneGraphChanges> changesQueue = dynamic_cast<SceneGraphChanges*> (node->getUserData() );
	osg::Group* nodeAsGroup = node->asGroup();

	if( changesQueue && nodeAsGroup )
	{
		std::map< std::string, std::list< GeoNode* > > queue = changesQueue->getMap();

		for( std::map< std::string, std::list< GeoNode* > >::iterator it = queue.begin() ; it != queue.end(); ++it )
		{
			std::string strLyr = it->first;
			osg::ref_ptr<osg::Group> grp = getorcreateChild(nodeAsGroup,strLyr);
			for( std::list< GeoNode* >::iterator itl = it->second.begin(); itl != it->second.end(); ++itl )
			{
				if( (*itl)->toRemove() && (*itl)->inGraph() )
				{
					//nodeAsGroup->removeChild( (*itl)->getNode() );
					grp->removeChild( (*itl)->getNode() );
					(*itl)->setInGraph(false);
					changesQueue->checkRemove();
				}
				else if( !(*itl)->toRemove() && !(*itl)->inGraph() )
				{
					//nodeAsGroup->addChild( (*itl)->getNode() );
					grp->addChild( (*itl)->getNode() );
					(*itl)->setInGraph(true);
				}
		    }
		}
	}
	traverse(node, nv);
}

SceneGraphChangeCallback::~SceneGraphChangeCallback(){}
