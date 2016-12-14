#include "server/NetworkManage.h"
#include "server/SocketDataManage.h"
#include "mahjong/state/GameData.h"
#include "socket/GameSocketManage.hpp"

NetworkManage* NetworkManage::_instance = NULL;
std::string NetworkManage::allReciveInfo;


NetworkManage* NetworkManage::getInstance() {
    if (NULL == _instance) {
        _instance = new NetworkManage();
    }
    return _instance;
}

NetworkManage::NetworkManage() {
    connectServer();
}

void NetworkManage::connectServer() {
    GameSocketManage::getInstance()->socketConnect();
}

void NetworkManage::sendMsg(std::string code) {
   
}

void NetworkManage::heartbeat() {
    std::thread recvThread = std::thread(&NetworkManage::sendHeartBeat, this);
    recvThread.detach();
}

void NetworkManage::sendHeartBeat() {
    
}

void NetworkManage::receiveData() {
    
}

int NetworkManage::getMsgLength(std::string str) {
    return str.length();
}
