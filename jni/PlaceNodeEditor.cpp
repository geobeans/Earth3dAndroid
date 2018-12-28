#include "PlaceNodeEditor.h"
/**********************************************************************/

class DraggerCallback : public osgEarth::Dragger::PositionChangedCallback
{
public:
	DraggerCallback(osgEarth::Annotation::PositionedAnnotationNode* node, PlaceNodeEditor* editor) :
		_node(node),
		_editor(editor)
	{
	}

	virtual void onPositionChanged(const osgEarth::Dragger* sender, const osgEarth::GeoPoint& position)
	{
		_node->setPosition(position);
		_editor->updateDraggers();
	}

	osgEarth::Annotation::PositionedAnnotationNode* _node;
	PlaceNodeEditor* _editor;
};

/**********************************************************************/
PlaceNodeEditor::PlaceNodeEditor(osgEarth::Annotation::PositionedAnnotationNode* node) :
	_node(node)
{
	_dragger = new osgEarth::SphereDragger(_node->getMapNode());
	_dragger->addPositionChangedCallback(new DraggerCallback(_node, this));
	addChild(_dragger);
	updateDraggers();
}

PlaceNodeEditor::~PlaceNodeEditor()
{
}

void
PlaceNodeEditor::updateDraggers()
{
	osgEarth::GeoPoint pos = _node->getPosition();
	_dragger->setPosition(pos, false);
}

void
PlaceNodeEditor::setPosition(const osgEarth::GeoPoint& pos)
{
	_node->setPosition(pos);
	updateDraggers();
}
