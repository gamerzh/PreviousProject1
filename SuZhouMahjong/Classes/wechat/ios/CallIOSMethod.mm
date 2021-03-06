//
//  CallIOSMethod.cpp
//  Mahjong
//
//  Created by qiuzhong on 16/9/19.
//
//
#import "json/document.h"
#import "json/rapidjson.h"
#import "wechat/ios/CallIOSMethod.h"
#import "wechat/ios/StatusBarTool.h"
#import "wechat/ios/RechargeVC.h"
#import "wechat/ios/LoginByWechat.h"
#include "mahjong/GameConfig.h"
#include "userdata/UserData.h"

#define RETURN_IF(cond)           if((cond)) return

CallIOSMethod* CallIOSMethod::_instance = 0;

CallIOSMethod::CallIOSMethod(){
    this->init();
}

void CallIOSMethod::init(){
    //TODO
}

CallIOSMethod* CallIOSMethod::getInstance(){
    if (_instance == 0){
        _instance = new CallIOSMethod();
    }
    return _instance;
}

void CallIOSMethod::doPayEvent(std::string poxiaoId,int payId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //获取商品的编号
    if(UserData::getInstance()->isWeixinPayOpen() && UserData::getInstance()->getUserPayWay() == "1"){
        //微信支付
                NSString* pyid = [[NSString alloc] initWithFormat:@"%s",poxiaoId.c_str()];
                NSString *poid=  [[NSString alloc] initWithFormat:@"%d",payId];
                LoginByWechat* loginByWechat = [LoginByWechat sharedManager] ;
                [loginByWechat payWeChat:pyid PayPoint:poid];
    }else{
        //IAP支付
        getProductId(poxiaoId,StringUtils::format("%d",payId));
    }
#endif
}


void CallIOSMethod::doWechatLogin(){
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager] ;
    [loginByWechat sendAuthRequestScope];
}

bool CallIOSMethod::isWenxinInstalled(){
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager] ;
    bool result = [loginByWechat isWenxinInstalled];
    return result;
}

std::string CallIOSMethod::getBatteryPersent(){
    UIApplication *app = [UIApplication sharedApplication];
    NSArray *infoArray = nil;
    CGSize size = [UIScreen mainScreen].bounds.size;
    CGFloat maxHeight = MAX(size.width, size.height);
    if (maxHeight == 812) {
        infoArray = [[[[app valueForKey:@"statusBar"] valueForKey:@"statusBar"] valueForKey:@"foregroundView"] subviews];
    } else {
        infoArray = [[[app valueForKey:@"statusBar"] valueForKey:@"foregroundView"] subviews];
    }
    
    for (id info in infoArray)
    {
        if ([info isKindOfClass:NSClassFromString(@"UIStatusBarBatteryPercentItemView")])
        {
            NSString *percentString = [info valueForKeyPath:@"percentString"];
            NSLog(@"电量为：%@",percentString);
            return std::string([percentString UTF8String]).c_str();
        }
    }
    return "100";
}

void CallIOSMethod::doWechatShareWeb(std::string url,std::string title,std::string content,int scene){
    NSString* wxUrl = [[NSString alloc] initWithFormat:@"%s",url.c_str()];
    NSString *wxTitle= [[NSString alloc] initWithCString:title.c_str() encoding:NSUTF8StringEncoding];
    NSString* wxContent = [[NSString alloc] initWithCString:content.c_str() encoding:NSUTF8StringEncoding];
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager] ;
    [loginByWechat wechatShareWeb:wxUrl ContentTile:wxTitle ContentDescription:wxContent Scene:scene];
    
}


void CallIOSMethod::doWechatShareApp(std::string title,std::string content){
    NSString* wxTitle = [[NSString alloc] initWithFormat:@"%s",title.c_str()];
    NSString* wxContent = [[NSString alloc] initWithFormat:@"%s",content.c_str()];
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager];
    [loginByWechat wechatShareApp:wxTitle ContentDescription:wxContent];
}

void CallIOSMethod::doWechatShareImg(std::string filepath,int scene){
    NSString *filePath=[NSString stringWithFormat:@"%s",filepath.c_str()];
    NSData *imageData = [NSData dataWithContentsOfFile:filePath];
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager];
    [loginByWechat wechatShareImg:imageData Scene:scene];
}

void CallIOSMethod::copyToPasteboard(std::string str){
    //把string类型转换成为char*
    
    char*p=(char*)str.data();
    
    //把char*转换成OC的NSString
    
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //获得iOS的剪切板
    
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //改变剪切板的内容
    
    pasteboard.string = nsMessage;
}

void CallIOSMethod::updateClientAppVersion(){
    LoginByWechat* loginByWechat = [LoginByWechat sharedManager];
    [loginByWechat updateClientAppVersion];
}


void CallIOSMethod::getProductId(std::string poxiaoId,std::string payId){
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(StringUtils::format("%s?pay_point=%s&tbu_id=%s&poxiao_id=%s",APPLE_STORE_PAY_LIST,payId.c_str(),TBU_ID,poxiaoId.c_str()).c_str());
    request->setResponseCallback(CC_CALLBACK_2(CallIOSMethod::onHttpRequestCompleted, this));
    request->setTag("Get Product ID");
    HttpClient::getInstance()->send(request);
    request->release();
}

void CallIOSMethod::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response){
    std::vector<char> *buffer;
    while (true)
    {
        if (!response)
        {
            return;
        }
        if (!response->isSucceed())
        {
            return;
        }
        buffer = response->getResponseData();
        break;
    }
    std::string msg(buffer->begin(),buffer->end());;
    log("onHttpRequestCompleted response = %s",msg.c_str());
    rapidjson::Document _mDoc;
    RETURN_IF(NULL == msg.c_str() || !msg.compare(""));
    _mDoc.Parse<0>(msg.c_str());
    RETURN_IF(_mDoc.HasParseError() || !_mDoc.IsObject());
    const rapidjson::Value &result = _mDoc["result"];
    if(result.GetInt() == 0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        const rapidjson::Value &ios = _mDoc["ios"];
        const rapidjson::Value &myOrderId = _mDoc["orderId"];
        const rapidjson::Value &mypoxiaoId = _mDoc["poxiao_id"];
        //iOS代码
        NSString* pxProductId = [[NSString alloc] initWithFormat:@"%s",ios.GetString()];
        NSString* pxOrderId = [[NSString alloc] initWithFormat:@"%s",myOrderId.GetString()];
        NSString* poXiaoId = [[NSString alloc] initWithFormat:@"%s",mypoxiaoId.GetString()];
        RechargeVC* rechargeVC = [RechargeVC shareInstance] ;
        [rechargeVC buy:pxOrderId productId:pxProductId poxiaoId:poXiaoId];
#endif
    }else{
        UIAlertView *alerView2 = [[UIAlertView alloc] initWithTitle:@"提示" message:@"你今天购买次数过多" delegate:nil cancelButtonTitle:NSLocalizedString(@"关闭",nil) otherButtonTitles:nil];
        [alerView2 show];
    }
}



