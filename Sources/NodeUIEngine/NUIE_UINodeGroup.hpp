#ifndef NUIE_UINODEGROUP_HPP
#define NUIE_UINODEGROUP_HPP

#include "NE_NodeGroup.hpp"
#include "NUIE_UINode.hpp"

namespace NUIE
{

class GroupDrawingImage : public DrawingImage
{
public:
	GroupDrawingImage ();
	virtual ~GroupDrawingImage ();

	void			Reset ();
	
	const Rect&		GetRect () const;
	void			SetRect (const Rect& newRect);

private:
	Rect rect;
};

class NodeRectGetter
{
public:
	NodeRectGetter ();
	virtual ~NodeRectGetter ();

	virtual Rect GetNodeRect (const NE::NodeId& nodeId) const = 0;
};

class UINodeGroup : public NE::NodeGroup
{
	DYNAMIC_SERIALIZABLE (UINodeGroup);

public:
	UINodeGroup ();
	UINodeGroup (const std::wstring& name);
	~UINodeGroup ();

	const std::wstring&			GetName () const;
	void						SetName (const std::wstring& newName);

	Rect						GetRect (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const;
	void						Draw (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const;
	void						InvalidateGroupDrawing () const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	const GroupDrawingImage&	GetDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const;
	void						UpdateDrawingImage (NodeUIDrawingEnvironment& env, const NodeRectGetter& rectGetter, const NE::NodeCollection& nodes) const;

	std::wstring				name;
	mutable GroupDrawingImage	drawingImage;
};

using UINodeGroupPtr = std::shared_ptr<UINodeGroup>;
using UINodeGroupConstPtr = std::shared_ptr<const UINodeGroup>;

}

#endif
