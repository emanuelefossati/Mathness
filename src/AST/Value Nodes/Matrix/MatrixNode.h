#pragma once

#include "../../Node.h"
#include "./Results/Matrix/Matrix.h"


class MatrixNode : public INode
{

public:
	MatrixNode();

	std::vector<std::shared_ptr<INode>>& GetElements() { return _Elements; }
	void SetElements(std::vector<std::shared_ptr<INode>> elements) { _Elements = elements; }

	size_t GetRows() const { return _Rows; }
	void SetRows(size_t rows) { _Rows = rows; }

	size_t GetColumns() const { return _Columns; }
	void SetColumns(size_t columns) { _Columns = columns; }

	id_t GetId() const { return _Id; }

	Result GetResult() const override;

private:
	std::vector<std::shared_ptr<INode>> _Elements;
	
	static size_t _Count;

	id_t _Id;

	size_t _Rows;
	size_t _Columns;
};