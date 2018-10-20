#ifndef ARITHMETICUINODES_HPP
#define ARITHMETICUINODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class BinaryOperationNode : public BasicUINode,
							public ValueCombinationFeature
{
	SERIALIZABLE;

public:
	BinaryOperationNode ();
	BinaryOperationNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~BinaryOperationNode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;

	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual double				DoOperation (double a, double b) const = 0;
};

class AdditionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (AdditionNode);

public:
	AdditionNode ();
	AdditionNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~AdditionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class SubtractionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (SubtractionNode);

public:
	SubtractionNode ();
	SubtractionNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~SubtractionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class MultiplicationNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (MultiplicationNode);

public:
	MultiplicationNode ();
	MultiplicationNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~MultiplicationNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class DivisionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (DivisionNode);

public:
	DivisionNode ();
	DivisionNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~DivisionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

}

#endif
