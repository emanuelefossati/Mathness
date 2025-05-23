#include "DotNode.h"

EvaluationResult DotNode::GetResult() const
{
	auto left = _Left->GetResult();
	auto right = _Right->GetResult();

	if (left.IsError())
		return left;

	if (right.IsError())
		return right;

	if (!left.IsMatrix() || !right.IsMatrix())
		return Error("cannot perform dot operation on non-vector types");

	Matrix leftMatrix = left.ToMatrix();
	Matrix rightMatrix = right.ToMatrix();

	if (!leftMatrix.IsVector() || !rightMatrix.IsVector())
		return Error("cannot perform dot operation on non-vector types");

	if (leftMatrix.Elements.size() != rightMatrix.Elements.size())
		return Error("cannot perform dot operation on vectors of different sizes");

	return leftMatrix.Dot(rightMatrix);
}