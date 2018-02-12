#include "HelloWorldScene.h"
#include "Monster.h"
#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    killed = 0;
    database->setIntegerForKey("killed", killed);
    log("%s", FileUtils::getInstance()->getWritablePath().c_str());
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
    tmx->setPosition(visibleSize.width/2, visibleSize.height/2);
    tmx->setAnchorPoint(Vec2(0.5, 0.5));
    tmx->setScale(Director::getInstance()->getContentScaleFactor());
    this->addChild(tmx, 0);
    
    //  添加MenuItemLabel
    TTFConfig ttfConfig;
    ttfConfig.fontFilePath = "arial.ttf";
    ttfConfig.fontSize = 36;
    ttfConfig.outlineSize = 2;
    //auto X = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "X"), CC_CALLBACK_1(HelloWorld::DeadMenuCallback, this));
    auto Y = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "Y"), CC_CALLBACK_1(HelloWorld::AttackMenuCallback, this));
    auto W = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "W"), CC_CALLBACK_1(HelloWorld::WMoveMenuCallback, this));
    auto A = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "A"), CC_CALLBACK_1(HelloWorld::AMoveMenuCallback, this));
    auto S = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "S"), CC_CALLBACK_1(HelloWorld::SMoveMenuCallback, this));
    auto D = MenuItemLabel::create(Label::createWithTTF(ttfConfig, "D"), CC_CALLBACK_1(HelloWorld::DMoveMenuCallback, this));
   /* X->setPosition(Vec2(origin.x + visibleSize.width-X->getContentSize().width,
                        origin.y+X->getContentSize().height+50));*/
    Y->setPosition(Vec2(origin.x + visibleSize.width-Y->getContentSize().width-50,
                   origin.y+Y->getContentSize().height));
    W->setPosition(Vec2(origin.x + Y->getContentSize().width+50,
                        origin.y+Y->getContentSize().height+50));
    A->setPosition(Vec2(origin.x + Y->getContentSize().width,
                        origin.y+Y->getContentSize().height));
    S->setPosition(Vec2(origin.x + Y->getContentSize().width+50,
                        origin.y+Y->getContentSize().height));
    D->setPosition(Vec2(origin.x + Y->getContentSize().width+100,
                        origin.y+Y->getContentSize().height));
    auto menu = Menu::create(Y, W, A, S, D, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);
    
    //  ----------------------------------------------------------------
    
    //  增加hp血条
    Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
    Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
    
    pT = ProgressTimer::create(sp);
    pT->setScaleX(90);
    pT->setAnchorPoint(Vec2(0, 0));
    pT->setType(ProgressTimerType::BAR);
    pT->setBarChangeRate(Point(1, 0));
    pT->setMidpoint(Point(0, 1));
    pT->setPercentage(100);
    percent = 100;
    pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
    addChild(pT,1);
    sp0->setAnchorPoint(Vec2(0, 0));
    sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
    addChild(sp0,0);
    
    //  ----------------------------------------------------------------
    
    //  attack
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
    attack.reserve(17);
    for (int i = 0; i < 17; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
        attack.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
    animation->setRestoreOriginalFrame(true);  //  回到第一帧
    AnimationCache::getInstance()->addAnimation(animation, "attack");

    //  ----------------------------------------------------------------
    
    //  dead
    auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
    dead.reserve(22);
    for (int i = 0; i < 22; i++) {
        auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79*i,0,90,90)));
        dead.pushBack(frame);
    }
    animation = Animation::createWithSpriteFrames(dead, 0.1f);
    animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "dead");
    
    //  ----------------------------------------------------------------
    
    //  move
    
    auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
    run.reserve(8);
    for (int i = 0; i < 8; i++) {
        auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68*i,0,68,101)));
        run.pushBack(frame);
    }
    animation = Animation::createWithSpriteFrames(run, 0.05f);
    AnimationCache::getInstance()->addAnimation(animation, "move");

    //  ----------------------------------------------------------------
    
    //idle
    
	idle.reserve(1);
	idle.pushBack(frame0);
    
    animation = Animation::createWithSpriteFrames(idle, 0.2f);
    AnimationCache::getInstance()->addAnimation(animation, "idle");
    
    player = Sprite::createWithSpriteFrame(frame0);
    player->setPosition(Vec2(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height/2));
    addChild(player, 3);
    
    String* temp = String::createWithFormat("%d", 0);
    time = Label::createWithTTF(ttfConfig, temp->_string);
    time->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height-time->getContentSize().height));
    addChild(time, 1);
    schedule(schedule_selector(HelloWorld::update), 2.0f, kRepeatForever, 0);
    schedule(schedule_selector(HelloWorld::HitByMonster), 0.2f, kRepeatForever, 0);
    return true;
}
void HelloWorld::AttackMenuCallback(Ref* pSender) {
    if (!canAct()) return;
    Animate* _attack = Animate::create(AnimationCache::getInstance()->getAnimation("attack"));
    player->runAction(_attack);
    Rect playerRect = player->getBoundingBox();
    Rect attackRect = Rect(playerRect.getMinX()-40, playerRect.getMinY(),playerRect.getMaxX()-playerRect.getMinX()+80, playerRect.getMaxY()-playerRect.getMinY());
    auto fac = Factory::getInstance();
    auto collision = fac->collider(attackRect);
    if (collision != NULL) {
        fac->removeMonster(collision);
        if(percent < 100) {
            pT->runAction(CCProgressTo::create(2, percent+20));
            percent += 20;
        }
        killed++;
        String* temp = String::createWithFormat("%d", killed);
        time->setString(temp->_string);
        database->setIntegerForKey("value", killed);
    }
}

void HelloWorld::WMoveMenuCallback(Ref* pSender) {
    auto loc = player->getPosition()+Vec2(0, 25);
    if (!canAct())return;
    Move(loc);
}
void HelloWorld::AMoveMenuCallback(Ref* pSender) {
    auto loc = player->getPosition()+Vec2(-25, 0);
    player->setFlippedX(true);
    Move(loc);
}
void HelloWorld::SMoveMenuCallback(Ref* pSender) {
    auto loc = player->getPosition()+Vec2(0, -25);
    Move(loc);
}
void HelloWorld::DMoveMenuCallback(Ref* pSender) {
    auto loc = player->getPosition()+Vec2(25, 0);
    player->setFlippedX(false);
    Move(loc);
}
void HelloWorld::Move(Vec2 loc) {
    if (loc.x < origin.x || loc.x > origin.x+visibleSize.width||
        loc.y < origin.y ||loc.y > origin.x+visibleSize.height)
            return;
    player->runAction(MoveTo::create(0.4, loc));
    Animate* _move = Animate::create(AnimationCache::getInstance()->getAnimation("move"));
    player->runAction(_move);
}
bool HelloWorld::canAct() {
    return (getActionManager()->getNumberOfRunningActionsInTarget(player) < 1);
}

void HelloWorld::update(float dt) {
    auto fac = Factory::getInstance();
    for (int i = 0; i < 1; i++) {
        auto m = fac->createMonster();
        float x = random(origin.x, visibleSize.width);
        float y = random(origin.y, visibleSize.height);
        m->setPosition(x, y);
        addChild(m, 3);
    }
    fac->moveMonster(player->getPosition(), dt);
}

void HelloWorld::HitByMonster(float dt) {
    if (!canAct()) return;
    auto fac = Factory::getInstance();
    auto collision = fac->collider(player->getBoundingBox());
    if (collision != NULL) {
        fac->removeMonster(collision);
        if(percent >= 20) {
            pT->runAction(CCProgressTo::create(2, percent-20));
            percent -= 20;
        }
        Animate* _dead = Animate::create(AnimationCache::getInstance()->getAnimation("dead"));
        player->runAction(_dead);
        if (percent == 0) {
            Director::getInstance()->pause();
        }
    }
}