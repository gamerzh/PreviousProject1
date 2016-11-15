//
//  LoginByWechat.cpp
//  Mahjong
//
//  Created by qiuzhong on 16/9/26.
//
//

#import "payment/ios/LoginByWechat.h"
#include "payment/ios/WxLoginHandler.hpp"
#include "userdata/UserData.h"

@implementation LoginByWechat


static NSString *kAuthScope = @"snsapi_message,snsapi_userinfo,snsapi_friend,snsapi_contact";
static NSString *kAuthState = @"heisdoubi";

static NSString *kAppContentExInfo = @"<xml>extend info</xml>";
static NSString *kAppContnetExURL = @"http://weixin.qq.com";
static NSString *kAppMessageExt = @"这是第三方带的测试字段";
static NSString *kAppMessageAction = @"<action>dotaliTest</action>";

static NSString * poxiaoOrderId = nil;


#pragma mark - LifeCycle

+(instancetype)sharedManager {
    static dispatch_once_t onceToken;
    static LoginByWechat *instance;
    dispatch_once(&onceToken, ^{
        instance = [[LoginByWechat alloc] init];
        [WXApi registerApp:AppID withDescription:@"上海麻将"];
    });
    return instance;
}

- (void)dealloc {
    [super dealloc];
}

#pragma mark - View Lifecycle

- (BOOL)sendAuthRequestScope{
    if (UserData::getInstance()->getWxOpenId() == "unknow") {
        SendAuthReq* req    =[[SendAuthReq alloc]init];
        req.scope           = kAuthScope;
        req.state           = kAuthState;
        //第三方向微信终端发送一个SendAuthReq消息结构
        return [WXApi sendReq:req];
        
    }else{
        BOOL result = [self checkTokenOutTime];
        if(result){
            WxLoginHandler::getInstance()->doGameLogin(UserData::getInstance()->getWxOpenId(),UserData::getInstance()->getPicture(),StringUtils::format("%d",UserData::getInstance()->getGender()) ,UserData::getInstance()->getNickName());
            return true;
        }else{
            SendAuthReq* req    =[[SendAuthReq alloc]init];
            req.scope           = kAuthScope;
            req.state           = kAuthState;
            //第三方向微信终端发送一个SendAuthReq消息结构
            return [WXApi sendReq:req];
        }
    }
}

- (void)sendLoginMsg2Server:(NSString*) code{
    
    //第一步，创建URL
    NSString *urlstring = [NSString stringWithFormat:@"http://183.129.206.54:1111/pay!getWxAccessToken.action?appid=%@&code=%@",AppID,code];
    NSURL *url = [NSURL URLWithString:urlstring];
    //第二步，创建请求
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
    //第三步，连接服务器
    NSData *received = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *result = [[NSString alloc]initWithData:received encoding:NSUTF8StringEncoding];
    NSLog(@"received = %@",result);
    //解析返回的json数据
    NSData *returnedData = [result dataUsingEncoding:NSUTF8StringEncoding];;
    if(NSClassFromString(@"NSJSONSerialization"))
    {
        NSError *error = nil;
        id object = [NSJSONSerialization
                     JSONObjectWithData:returnedData
                     options:0
                     error:&error];
        
        if(error) {
            UserData::getInstance()->setWxOpenId("unknow");
            NSLog(@"json 格式有错误1");
        }
        if([object isKindOfClass:[NSDictionary class]])
        {
            NSDictionary *results = object;
            NSObject *myresult = [results objectForKey:@"result"];
            NSString *resultstr = [NSString stringWithFormat:@"%@", myresult];
            if(strcmp(std::string([resultstr UTF8String]).c_str(),"0") == 0){
                NSObject *openid = [results objectForKey:@"openid"];
                NSString *openidstr = [NSString stringWithFormat:@"%@", openid];
                NSObject *headimgurl = [results objectForKey:@"headimgurl"];
                NSString *headimgurlstr = [NSString stringWithFormat:@"%@", headimgurl];
                NSObject *sex = [results objectForKey:@"sex"];
                NSString *sexStr = [NSString stringWithFormat:@"%@", sex];
                NSObject *nickname = [results objectForKey:@"nickname"];
                NSString *nicknameStr = [NSString stringWithFormat:@"%@", sex];
                NSString *man = @"0";
                if(sexStr == man){
                    UserData::getInstance()->setGender(1);
                }else{
                    UserData::getInstance()->setGender(0);
                }
                UserData::getInstance()->setWxOpenId(std::string([openidstr UTF8String]));
                UserData::getInstance()->setPicture(std::string([headimgurlstr UTF8String]));
                UserData::getInstance()->setGender(atoi(std::string([sexStr UTF8String]).c_str()));
                UserData::getInstance()->setNickName(std::string([nickname UTF8String]));
                WxLoginHandler::getInstance()->doGameLogin(std::string([openidstr UTF8String]), std::string([headimgurlstr UTF8String]),std::string([sexStr UTF8String]),std::string([nickname UTF8String]));
            }else{
                UserData::getInstance()->setWxOpenId("unknow");
                [self sendAuthRequestScope];
            }
            
        }
        else
        {
            UserData::getInstance()->setWxOpenId("unknow");
            NSLog(@"json 格式有错误2");
        }
    }
    else
    {
        UserData::getInstance()->setWxOpenId("unknow");
        NSLog(@"系统版本过低");
    }
}


- (BOOL)checkTokenOutTime{
    //第一步，创建URL
    NSString *openidstr = [NSString stringWithFormat:@"%s", UserData::getInstance()->getWxOpenId().c_str()];
    NSString *urlstring = [NSString stringWithFormat:@"http://183.129.206.54:1111/pay!getIfExpire.action?openId=%@",openidstr];
    NSURL *url = [NSURL URLWithString:urlstring];
    //第二步，创建请求
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc]initWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10];
    //第三步，连接服务器
    NSData *received = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *result = [[NSString alloc]initWithData:received encoding:NSUTF8StringEncoding];
    NSLog(@"received = %@",result);
    //解析返回的json数据
    NSData *returnedData = [result dataUsingEncoding:NSUTF8StringEncoding];;
    if(NSClassFromString(@"NSJSONSerialization"))
    {
        NSError *error = nil;
        id object = [NSJSONSerialization
                     JSONObjectWithData:returnedData
                     options:0
                     error:&error];
        
        if(error) {
            NSLog(@"json 格式有错误1");
        }
        if([object isKindOfClass:[NSDictionary class]])
        {
            NSDictionary *results = object;
            NSObject *myresult = [results objectForKey:@"result"];
            NSString *resultstr = [NSString stringWithFormat:@"%@", myresult];
            if(strcmp(std::string([resultstr UTF8String]).c_str(),"0") == 0){
                NSObject *head = [results objectForKey:@"head"];
                NSString *headstr = [NSString stringWithFormat:@"%@", head];
                UserData::getInstance()->setPicture(std::string([headstr UTF8String]));
                NSObject *sex = [results objectForKey:@"sex"];
                NSString *sexStr = [NSString stringWithFormat:@"%@", sex];
                NSString *man = @"0";
                if(sexStr == man){
                    UserData::getInstance()->setGender(1);
                }else{
                    UserData::getInstance()->setGender(0);
                }
                return true;
            }else{
                UserData::getInstance()->setWxOpenId("unknow");
                [self sendAuthRequestScope];
            }
            
        }
        else
        {
            NSLog(@"json 格式有错误2");
        }
    }
    else
    {
        NSLog(@"系统版本过低");
    }
    return false;
}

//微信分享
- (void) wechatShareApp:(NSString*) contentTile ContentDescription:(NSString*) contentDescription{
    
    Byte* pBuffer = (Byte *)malloc(1024 * 100);
    memset(pBuffer, 0, 1024 * 100);
    NSData* data = [NSData dataWithBytes:pBuffer length:1024 * 100];
    free(pBuffer);
    WXAppExtendObject *ext = [WXAppExtendObject object];
    ext.extInfo = kAppContentExInfo;
    ext.url = kAppContnetExURL;
    ext.fileData = data;
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = contentTile;
    message.description = contentDescription;
    message.mediaObject = ext;
    message.messageExt = kAppMessageExt;
    message.messageAction = kAppMessageAction;
    message.mediaTagName = nil;
    [message setThumbImage:nil];
    SendMessageToWXReq *req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.message = message;
    [WXApi sendReq:req];
}

- (void) wechatShareWeb:(NSString*) url ContentTile:(NSString*) contentTile ContentDescription:(NSString*) contentDescription Scene:(int) inScene{
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = url;
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = contentTile;
    message.description = contentDescription;
    message.mediaObject = ext;
    message.messageExt = kAppMessageExt;
    message.messageAction = kAppMessageAction;
    message.mediaTagName = nil;
    UIImage *thumbImage = [UIImage imageNamed:@"Icon-180.png"];
    [message setThumbImage:thumbImage];
    SendMessageToWXReq *req = [[[SendMessageToWXReq alloc] init] autorelease];
    req.message = message;
    req.scene = inScene;
    [WXApi sendReq:req];
}

-(void) payWeChat:(NSString*) poxiaoId PayPoint:(NSString*) payPoint{
    NSString* urlString= [NSString stringWithFormat:@"http://183.129.206.54:1111/pay!generateOrd.action?charge_type=1&tbu_id=201617&pay_platform=apple&game_version=1&hsman=ios&hstype=ios&imei=123456789&imsi=46000&channel_id=apple&request_pay_amount=1&poxiao_id=%@&pay_point=%@",poxiaoId,payPoint];
    NSLog(@"url:%@",urlString);
    //解析服务端返回json数据
    NSError *error;
    //加载一个NSURL对象
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlString]];
    //第三步，连接服务器
    NSData *received = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *result = [[NSString alloc]initWithData:received encoding:NSUTF8StringEncoding];
    NSLog(@"received = %@",result);
    //解析返回的json数据
    NSData *returnedData = [result dataUsingEncoding:NSUTF8StringEncoding];;
    if(NSClassFromString(@"NSJSONSerialization"))
    {
        NSError *error = nil;
        id object = [NSJSONSerialization
                     JSONObjectWithData:returnedData
                     options:0
                     error:&error];
        
        if(error) {
            NSLog(@"json 格式有错误");
        }
        if([object isKindOfClass:[NSDictionary class]])
        {
            
            NSDictionary *results = object;
            NSObject *myresult = [results objectForKey:@"result"];
            NSString *resultstr = [NSString stringWithFormat:@"%@", myresult];
            if(strcmp(std::string([resultstr UTF8String]).c_str(),"0") == 0){
                NSObject *px_order_id = [results objectForKey:@"px_order_id"];
                poxiaoOrderId = [NSString stringWithFormat:@"%@", px_order_id];
                //                NSObject *wx_order_id = [results objectForKey:@"wx_order_id"];
                //                NSString *wxOrderId = [NSString stringWithFormat:@"%@", wx_order_id];
                NSObject *wx_nonce_str = [results objectForKey:@"wx_nonce_str"];
                NSString *wxNonceStr = [NSString stringWithFormat:@"%@", wx_nonce_str];
                NSObject *wx_prepayid = [results objectForKey:@"wx_prepayid"];
                NSString *wxPrepayid = [NSString stringWithFormat:@"%@", wx_prepayid];
                NSObject *wx_sign = [results objectForKey:@"wx_sign"];
                NSString *wxSign = [NSString stringWithFormat:@"%@", wx_sign];
                NSObject *wx_timestamp = [results objectForKey:@"wx_timestamp"];
                int wxTimestamp = [(NSNumber*)wx_timestamp intValue];
                //调起微信支付
                PayReq* req             = [[[PayReq alloc] init]autorelease];
                req.partnerId           = @"1380518102";
                req.prepayId            = wxPrepayid;
                req.nonceStr            = wxNonceStr;
                req.timeStamp           = wxTimestamp;
                req.package             = @"Sign=WXPay";
                req.sign                = wxSign;
                [WXApi sendReq:req];
            }
        }
        else
        {
            NSLog(@"数据格式有错误");
        }
    }
    else
    {
        NSLog(@"系统版本过低");
    }
}


- (BOOL) queryPayResult{
    NSString* urlString= [NSString stringWithFormat:@"http://183.129.206.54:1111/pay!findOrd.action?order_id=%@",poxiaoOrderId];
    NSLog(@"url:%@",urlString);
    //解析服务端返回json数据
    NSError *error;
    //加载一个NSURL对象
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlString]];
    //第三步，连接服务器
    NSData *received = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *result = [[NSString alloc]initWithData:received encoding:NSUTF8StringEncoding];
    NSLog(@"received = %@",result);
    //解析返回的json数据
    NSData *returnedData = [result dataUsingEncoding:NSUTF8StringEncoding];;
    if(NSClassFromString(@"NSJSONSerialization"))
    {
        NSError *error = nil;
        id object = [NSJSONSerialization
                     JSONObjectWithData:returnedData
                     options:0
                     error:&error];
        
        if(error) {
            NSLog(@"json 格式有错误");
        }
        if([object isKindOfClass:[NSDictionary class]])
        {
            
            NSDictionary *results = object;
            NSObject *myresult = [results objectForKey:@"result"];
            NSString *resultstr = [NSString stringWithFormat:@"%@", myresult];
            if(strcmp(std::string([resultstr UTF8String]).c_str(),"0") == 1){
                return true;
            }else{
                return false;
            }
        }
        else
        {
            NSLog(@"数据格式有错误");
        }
    }
    else
    {
        NSLog(@"系统版本过低");
    }
}

#pragma mark - WXApiDelegate
- (void)onResp:(BaseResp *)resp {
    if ([resp isKindOfClass:[SendAuthResp class]]) {
        SendAuthResp *authResp = (SendAuthResp *)resp;
        //发送请求到服务端
        [self sendLoginMsg2Server:authResp.code];
    }
    if ([resp isKindOfClass:[PayResp class]]){
        PayResp*response=(PayResp*)resp;
        switch(response.errCode){
            case WXSuccess:
                //服务器端查询支付通知或查询API返回的结果再提示成功
            {
                NSLog(@"支付成功");
                LoginByWechat* loginByWechat = [LoginByWechat alloc] ;
                BOOL result = [loginByWechat queryPayResult];
                if(result){
                    WxLoginHandler::getInstance()->updatePlayerInfo();
                }
                break;
            }
            default:
                NSLog(@"支付失败，retcode=%d",resp.errCode);
                break;
        }
    }
}

- (void)onReq:(BaseReq *)req {
    
}

@end
