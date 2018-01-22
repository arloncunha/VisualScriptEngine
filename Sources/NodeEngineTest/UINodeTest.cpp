#include "SimpleTest.hpp"
#include "NodeManager.hpp"
#include "Node.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

#include "NodeUIManager.hpp"
#include "UINodeParameters.hpp"

using namespace NE;
using namespace NUIE;

namespace UINodeTest
{

class TestNode : public UINode
{
public:
	TestNode (const std::wstring& name, const Point& position) :
		UINode (name, position)
	{
		
	}

	virtual void RegisterSlots () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), L"First Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), L"Second Input", NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValuePtr a = EvaluateSingleInputSlot (SlotId ("in1"), env);
		ValuePtr b = EvaluateSingleInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In1DefaultValueParameter : public NodeParameter
		{
		public:
			In1DefaultValueParameter () :
				NodeParameter ("in1", L"In1")
			{
			
			}

			virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
			{
				return uiNode->GetInputSlot (SlotId ("in1"))->GetDefaultValue ();
			}

			virtual bool IsApplicableTo (const UINodePtr& uiNode) const override
			{
				return std::dynamic_pointer_cast<TestNode> (uiNode) != nullptr;
			}

			virtual bool CanSetValue (const UINodePtr&, NE::ValuePtr& value) const override
			{
				return NE::Value::IsType<NE::IntValue> (value);
			}

			virtual bool SetValue (NodeUIManager&, NE::EvaluationEnv&, UINodePtr& uiNode, NE::ValuePtr& value) override
			{
				uiNode->GetInputSlot (SlotId ("in1"))->SetDefaultValue (value);
				return true;
			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In1DefaultValueParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}

	virtual void CalculationPostProcess (const ValuePtr&, NE::EvaluationEnv&) const override
	{
	
	}
};

class TestNode2 : public TestNode
{
public:
	TestNode2 (const std::wstring& name, const Point& position) :
		TestNode (name, position)
	{
		
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In1DefaultValueParameter : public NodeParameter
		{
		public:
			In1DefaultValueParameter () :
				NodeParameter ("in2", L"In2")
			{
			
			}

			virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
			{
				return uiNode->GetInputSlot (SlotId ("in2"))->GetDefaultValue ();
			}

			virtual bool IsApplicableTo (const UINodePtr& uiNode) const override
			{
				return std::dynamic_pointer_cast<TestNode2> (uiNode) != nullptr;
			}

			virtual bool CanSetValue (const UINodePtr&, NE::ValuePtr& value) const override
			{
				return NE::Value::IsType<NE::IntValue> (value);
			}

			virtual bool SetValue (NodeUIManager&, NE::EvaluationEnv&, UINodePtr& uiNode, NE::ValuePtr& value) override
			{
				uiNode->GetInputSlot (SlotId ("in2"))->SetDefaultValue (value);
				return true;
			}
		};

		TestNode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In1DefaultValueParameter ()));
	}
};

TEST (NodeParametersTest)
{
	NodeUIManager uiManager;

	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	NodeParameterList paramList;
	node->RegisterParameters (paramList);
	ValuePtr nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 3);

	ASSERT (paramList.GetParameterCount () == 2);
	
	ASSERT (node->GetNodeName () == L"TestNode");
	NodeParameterPtr nameParam = paramList.GetParameter (0);
	ASSERT (nameParam->GetId () == "NodeNameParameter");
	ASSERT (nameParam->GetName () == L"Name");
	ValuePtr newNameValue (new StringValue (L"NewNodeName"));
	nameParam->SetValue (uiManager, NE::EmptyEvaluationEnv, node, newNameValue);
	ASSERT (node->GetNodeName () == L"NewNodeName");

	NodeParameterPtr in1DefParam = paramList.GetParameter (1);
	ASSERT (in1DefParam->GetId () == "in1");
	ASSERT (in1DefParam->GetName () == L"In1");
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 1);
	ValuePtr newIn1Value (new IntValue (5));
	in1DefParam->SetValue (uiManager, NE::EmptyEvaluationEnv, node, newIn1Value);
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 5);
	nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 7);
}

TEST (NodeParametersTest2)
{
	NodeUIManager uiManager;

	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection (), paramList);
		ASSERT (paramList.GetParameterCount () == 0);
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection (node->GetId ()), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		paramList.GetParameter (0)->GetId () == "NodeNameParameter";
		paramList.GetParameter (1)->GetId () == "in1";
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection (node2->GetId ()), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		paramList.GetParameter (0)->GetId () == "NodeNameParameter";
		paramList.GetParameter (1)->GetId () == "in2";
	}

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		paramList.GetParameter (0)->GetId () == "NodeNameParameter";
		paramList.GetParameter (1)->GetId () == "in1";
	}
}

TEST (NodeParametersTest3)
{
	NodeUIManager uiManager;

	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		paramList.GetParameter (0)->GetId () == "NodeNameParameter";
		paramList.GetParameter (1)->GetId () == "in1";
		ValuePtr newName (new StringValue (L"NewName"));
		ApplyCommonParameter (uiManager, nodeCollection, paramList.GetParameter (0), EmptyEvaluationEnv, newName);
		ASSERT (node->GetNodeName () == L"NewName");
		ASSERT (node2->GetNodeName () == L"NewName");
	}
}

}
