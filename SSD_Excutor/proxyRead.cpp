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
    return "";
}
bool ProxyRead::bufferHit(unsigned int address) {
    return false;
}
