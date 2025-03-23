#pragma once

#include <vector>
#include <fmt/core.h>
#include <string>
#include "./Utils/TypeDefinitions.h"
#include <algorithm>

struct List
{
	struct ListElement
	{
		scalar_t Value = 0;
		scalar_t Weight = 1;
	};

	std::vector<ListElement> Elements;

	scalar_t Max() const
	{
		scalar_t max = Elements[0].Value;

		for (const auto& element : Elements)
			if (element.Value > max)
				max = element.Value;

		return max;
	}

	scalar_t Min() const
	{
		scalar_t min = Elements[0].Value;

		for (const auto& element : Elements)
			if (element.Value < min)
				min = element.Value;

		return min;
	}

	scalar_t Average() const
	{
		scalar_t sum = 0;
		scalar_t weightSum = 0;

		for (const auto& element : Elements)
		{
			sum += element.Value * element.Weight;
			weightSum += element.Weight;
		}

		return sum / weightSum;
	}

	scalar_t Sum() const
	{
		scalar_t sum = 0;

		for (const auto& element : Elements)
			sum += element.Value;

		return sum;
	}

	scalar_t Variance() const
	{
		scalar_t average = Average();
		scalar_t sum = 0;
		scalar_t weightSum = 0;

		for (const auto& element : Elements)
		{
			sum += element.Weight * (element.Value - average) * (element.Value - average);
			weightSum += element.Weight;
		}

		return sum / weightSum;
	}

	scalar_t Count() const
	{
		size_t count = Elements.size();
		return scalar_t(count);
	}

	List Distinct() const
	{
		List result;

		for (const auto& element : Elements)
		{
			if (std::find_if(result.Elements.begin(), result.Elements.end(), [&element](const ListElement& e) { return e.Value == element.Value; }) == result.Elements.end())
				result.Elements.push_back(element);
		}

		return result;
	}



	std::string ToString() const
	{
		std::string result = "{";

		for (int i = 0; i < Elements.size(); i++)
		{
			result += fmt::format("{}|{}", Elements[i].Value, Elements[i].Weight);

			if (i != Elements.size() - 1)
				result += "; ";
		}

		result += "}";

		return result;
	}
	
};
