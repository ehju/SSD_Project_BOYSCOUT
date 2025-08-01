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

    for (int i = commandBufferVector.size() - 1; i >= 0; --i) {
        if (commandBufferVector[i].commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_WRITE)) {
            if (commandBufferVector[i].commandInfo.lba == address) {
                return toHexString(commandBufferVector[i].commandInfo.value);
            }
        }
        else if (commandBufferVector[i].commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_ERASE)) {
            if (commandBufferVector[i].commandInfo.lba <= address && commandBufferVector[i].commandInfo.lba + commandBufferVector[i].commandInfo.value > address) {
                return "0x00000000";
            }
        }
    }
    return "";
}
bool ProxyRead::bufferHit(unsigned int address) {
    std::vector<DetailedCommandInfo>commandBufferVector = cbm->getCommandBufferList();
   // �ּҸ� ������ address�� �ش��ϴ� command�� �ִ��� �������� Ž��
    for (int i = commandBufferVector.size() - 1; i >= 0; --i) {
        if (commandBufferVector[i].commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_WRITE)) {
            if (commandBufferVector[i].commandInfo.lba == address) {
                return true;
            }
        }
        else if (commandBufferVector[i].commandInfo.command == static_cast<int>(SSDCommand::SSDCommand_ERASE)) {
            if (commandBufferVector[i].commandInfo.lba <= address && commandBufferVector[i].commandInfo.lba + commandBufferVector[i].commandInfo.value > address) {
                return true;
            }
        }
    }
    return false;
}

std::string ProxyRead::toHexString(unsigned int value) {
    std::stringstream ss;
    ss << "0x"
        << std::uppercase            // �빮��
        << std::setfill('0')        // �� �ڸ��� 0���� ä��
        << std::setw(8)             // �׻� 8�ڸ��� ����
        << std::hex                 // 16������ ���
        << value;
    return ss.str();
}