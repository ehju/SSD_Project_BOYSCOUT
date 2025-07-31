#pragma once
#include <vector>
#include <string>

class CommandBufferManger
{
public:
	std::vector<std::string> getCommandBufferList();

	std::vector<std::string> commandBufferList;
};