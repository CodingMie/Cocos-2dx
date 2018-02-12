#include "HelloWorldScene.h"

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object

    
    std::string toggleStr_1 = "Pause";
    auto font_1 = MenuItemFont::create(toggleStr_1.c_str());
    std::string toggleStr_2 = "Resume";
    auto font_2 = MenuItemFont::create(toggleStr_2.c_str());
    auto menuItem_2 = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorld::menuLabelCallback,this), font_1, font_2, NULL);
    menuItem_2->setPosition(Point(visibleSize.width/2, visibleSize.height-200));
    
    auto menu = Menu::create(closeItem, menuItem_2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCDictionary* pDict = CCDictionary::createWithContentsOfFile("UserInfo.xml");
    const char *name = ((CCString*)pDict->valueForKey("name"))->_string.c_str();
    
    auto name_label = Label::createWithTTF(name, "fonts/Hanzipen.ttc", 24);
    
    // position the label on the center of the screen
    name_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - name_label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(name_label, 1);
    const char *num = ((CCString*)pDict->valueForKey("stuNum"))->_string.c_str();
    auto num_label = Label::createWithTTF(num, "fonts/Marker Felt.ttf", 24);
    
    // 设置位置
    num_label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - name_label->getContentSize().height
                                 - num_label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(num_label, 1);
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("mypic.jpeg");
    sprite->setScale(0.5, 0.5);
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - name_label->getContentSize().height - num_label->getContentSize().height));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::menuLabelCallback(Ref* pSender)
{
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume();
    } else {
        Director::getInstance()->pause();
    }
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
