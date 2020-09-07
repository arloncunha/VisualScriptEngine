#ifndef NUIE_NODEEDITORINFO_HPP
#define NUIE_NODEEDITORINFO_HPP

#include "NE_SlotId.hpp"
#include "NE_NodeId.hpp"
#include "NUIE_Geometry.hpp"

#include <vector>
#include <string>

namespace NUIE
{

struct SlotInfo
{
	SlotInfo ();

	NE::SlotId		id;
	std::wstring	name;
	Rect			modelRect;
	Rect			screenRect;
};

struct NodeInfo
{
	NodeInfo ();

	NE::NodeId				id;
	std::wstring			name;
	
	Rect					modelRect;
	Rect					screenRect;

	std::vector<SlotInfo>	inputSlots;
	std::vector<SlotInfo>	outputSlots;
};

struct ConnectionInfo
{
	ConnectionInfo ();

	NE::NodeId				fromNodeId;
	NE::SlotId				fromSlotId;
	NE::NodeId				toNodeId;
	NE::SlotId				toSlotId;
};

struct NodeEditorInfo
{
	NodeEditorInfo ();

	std::vector<NodeInfo>			nodes;
	std::vector<ConnectionInfo>		connections;
};

}

#endif
