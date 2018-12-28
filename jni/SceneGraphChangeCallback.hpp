#ifndef SCENEGRAPHCHANGECALLBACK_CLASS_H
#define SCENEGRAPHCHANGECALLBACK_CLASS_H

#include <osg/Node>
#include <osg/NodeVisitor>

class SceneGraphChangeCallback : public osg::NodeCallback{
	public:
		SceneGraphChangeCallback();
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

		~SceneGraphChangeCallback();
	protected:
		osg::Group* getorcreateChild(osg::Group* node,std::string strGrp);
};

#endif
