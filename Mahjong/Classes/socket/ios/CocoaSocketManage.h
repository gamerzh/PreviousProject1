//
//  CocoaSocketManage.hpp
//  Mahjong
//
//  Created by qiuzhong on 2016/12/13.
//
//

#ifndef CocoaSocketManage_hpp
#define CocoaSocketManage_hpp

class CocoaSocketManage{
public:
    static CocoaSocketManage* getInstance();
    bool connectSocket(std::string host, int port);
    void startScoketBeat(std::string msg);
    void sendScoketData(std::string msg);
    void receiveScoketData(std::string msg);
    void resetBeatCount();
    void disConnectSocket();
    void disConnectSelf();
private:
    CocoaSocketManage();
    static CocoaSocketManage* _instance;
};

#endif /* CocoaSocketManage_hpp */
