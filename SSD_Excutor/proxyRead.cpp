#include "proxyRead.h"

ProxyRead::ProxyRead() {
	cbm = &(CommandBufferManager::getInstance());
}

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
	std::vector<DetailedCommandInfo>commandBufferVector = cbm->getCommandBufferList();

	int vectorSize = static_cast<int>(commandBufferVector.size());
	for (int i = vectorSize - 1; i >= 0; --i) {
		if (isBufferHitWriteCommand(commandBufferVector[i], address)) {
			return toHexString(commandBufferVector[i].commandInfo.value);
		}
		else if (isBufferHitEraseCommand(commandBufferVector[i], address)) {
			return "0x00000000";
		}
	}
	return "";
}

bool ProxyRead::isBufferHitWriteCommand(const DetailedCommandInfo& detailedCommandInfo, unsigned int address) {
	if (detailedCommandInfo.commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_WRITE)) {
		if (detailedCommandInfo.commandInfo.lba == address) return true;
	}
	return false;
}

bool ProxyRead::isBufferHitEraseCommand(const DetailedCommandInfo& detailedCommandInfo, unsigned int address) {
	if (detailedCommandInfo.commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_ERASE)) {
		if (detailedCommandInfo.commandInfo.lba <= address && detailedCommandInfo.commandInfo.lba + detailedCommandInfo.commandInfo.value > address)
			return true;
	}
	return false;
}

bool ProxyRead::bufferHit(unsigned int address) {
	std::vector<DetailedCommandInfo>commandBufferVector = cbm->getCommandBufferList();

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