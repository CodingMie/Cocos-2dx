#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "sqlite3.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"
using namespace cocos2d;
using namespace cocos2d::network;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    //virtual void DeadMenuCallback(Ref* pSender);
    virtual void HitByMonster(float dt);
    virtual void loginCallback(Ref* pSender);
    virtual void rankCallback(Ref* pSender);
    virtual void submitCallback(Ref* pSender);
    virtual void AttackMenuCallback(Ref* pSender);
    virtual void WMoveMenuCallback(Ref* pSender);
    virtual void AMoveMenuCallback(Ref* pSender);
    virtual void SMoveMenuCallback(Ref* pSender);
    virtual void DMoveMenuCallback(Ref* pSender);
    virtual void Move(Vec2 loc);
    virtual bool canAct();
	void update(float dt) override;
    void onHttpComplete_login(HttpClient *sender, HttpResponse *response);
    void onHttpComplete_rank(HttpClient *sender, HttpResponse *response);
    void onHttpComplete_submit(HttpClient *sender, HttpResponse *response);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	int killed;
    String highscore;
    TextFieldTTF * tf;
    int percent;
    bool isLogin;
    std::string Gamesessionid;
    Label* myscore;
    Label* rank;
	cocos2d::ProgressTimer* pT;
};

#endif // __HELLOWORLD_SCENE_H__
