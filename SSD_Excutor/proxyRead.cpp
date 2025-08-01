#include "proxyRead.h"

ProxyRead::ProxyRead(CommandBufferManager* ptr) : cbm{ ptr } {}

void ProxyRead::execute(CommandInfo commandInfo) {
	if (true == bufferHit(commandInfo.lba)) {
		FileUtil::deletePrevOutputFile();
		std::string matchedValue = getHexValueFromBuffer(commandInfo.lba);
		FileUtil::writeOutputFile(matchedValue);
	}
	else realRead.execute(commandInfo);
	return;
}
std::string ProxyRead::getHexValueFromBuffer(unsigned int address)
{
	std::string ret = getValueIfHitBuffer(address);
	return ret;
}

std::string ProxyRead::getValueIfHitBuffer(unsigned int address) {
	std::vector<CommandInfo>commandBufferVector = cbm->getCommandBufferList();
	int vectorSize = static_cast<int>(commandBufferVector.size());
	const std::string ERASED_VALUE = "0x00000000";

	for (int i = vectorSize - 1; i >= 0; --i) {
		if (isBufferHitWriteCommand(commandBufferVector[i], address)) {
			return toHexString(commandBufferVector[i].value);
		}
		else if (isBufferHitEraseCommand(commandBufferVector[i], address)) {
			return ERASED_VALUE;
		}
	}
	return "";
}

bool ProxyRead::isBufferHitWriteCommand(const CommandInfo& commandInfo, unsigned int address) {
	if (commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_WRITE)) {
		if (commandInfo.lba == address) return true;
	}
	return false;
}

bool ProxyRead::isBufferHitEraseCommand(const CommandInfo& commandInfo, unsigned int address) {
	if (commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_ERASE)) {
		if (commandInfo.lba <= address && commandInfo.lba + commandInfo.value > address)
			return true;
	}
	return false;
}

bool ProxyRead::bufferHit(unsigned int address) {
	std::vector<CommandInfo>commandBufferVector = cbm->getCommandBufferList();

	int vectorSize = static_cast<int>(commandBufferVector.size());
	for (int i = vectorSize - 1; i >= 0; --i) {
		if (isBufferHitWriteCommand(commandBufferVector[i], address)) {
				return true;
			}
		else if (isBufferHitEraseCommand(commandBufferVector[i], address)) {
				return true;
			}
	}
	return false;
}

std::string ProxyRead::toHexString(unsigned int value) {
	std::stringstream ss;
	ss << "0x"
		<< std::uppercase
		<< std::setfill('0')
		<< std::setw(8)
		<< std::hex
		<< value;
	return ss.str();
}