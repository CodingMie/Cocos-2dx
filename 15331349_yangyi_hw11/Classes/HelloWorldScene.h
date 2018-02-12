#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "sqlite3.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    //virtual void DeadMenuCallback(Ref* pSender);
    virtual void HitByMonster(float dt);
    virtual void AttackMenuCallback(Ref* pSender);
    virtual void WMoveMenuCallback(Ref* pSender);
    virtual void AMoveMenuCallback(Ref* pSender);
    virtual void SMoveMenuCallback(Ref* pSender);
    virtual void DMoveMenuCallback(Ref* pSender);
    virtual void Move(Vec2 loc);
    virtual bool canAct();
	void update(float dt) override;
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
    int percent;
	cocos2d::ProgressTimer* pT;
};

#endif // __HELLOWORLD_SCENE_H__
