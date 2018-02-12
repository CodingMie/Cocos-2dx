#include "Thunder.h"
#include <algorithm>
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = Thunder::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool Thunder::init() {
  if (!Layer::init()) {
    return false;
  }

  isMove = false;  //  «∑Òµ„ª˜∑…¥¨
  visibleSize = Director::getInstance()->getVisibleSize();

  // ¥¥Ω®±≥æ∞
  auto bgsprite = Sprite::create("bg.jpg");
  bgsprite->setPosition(visibleSize / 2);
  bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
                     visibleSize.height / bgsprite->getContentSize().height);
  this->addChild(bgsprite, 0);

  // ¥¥Ω®∑…¥¨
  player = Sprite::create("player.png");
  player->setAnchorPoint(Vec2(0.5, 0.5));
  player->setPosition(visibleSize.width / 2, player->getContentSize().height);
  player->setName("player");
  this->addChild(player, 1);

  // œ‘ æ‘… Ø∫Õ◊”µØ ˝¡ø
    enemysNum = Label::createWithTTF("enemys: 0", "arial.ttf", 20);
  enemysNum->setColor(Color3B(255, 255, 255));
  enemysNum->setPosition(50, 60);
  this->addChild(enemysNum, 3);
  bulletsNum = Label::createWithTTF("bullets: 0", "arial.ttf", 20);
  bulletsNum->setColor(Color3B(255, 255, 255));
  bulletsNum->setPosition(50, 30);
  this->addChild(bulletsNum, 3);

  addEnemy(5);        // ≥ı ºªØ‘… Ø
  preloadMusic();     // ‘§º”‘ÿ“Ù¿÷
  playBgm();          // ≤•∑≈±≥æ∞“Ù¿÷
  explosion();        // ¥¥Ω®±¨’®÷°∂Øª≠

  // ÃÌº”º‡Ã˝∆˜
  addTouchListener();
  addKeyboardListener();
  addCustomListener();

  // µ˜∂»∆˜
  schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

  return true;
}

//‘§º”‘ÿ“Ù¿÷Œƒº˛
void Thunder::preloadMusic() {
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("bgm.mp3");
    audio->preloadEffect("explore.wav");
    audio->preloadEffect("fire.wav");;
}

//≤•∑≈±≥æ∞“Ù¿÷
void Thunder::playBgm() {
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("bgm.mp3", true);
}

//≥ı ºªØ‘… Ø
void Thunder::addEnemy(int n) {
  enemys.clear();
  for (unsigned i = 0; i < 3; ++i) {
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", 3 - i);
    double width = visibleSize.width / (n + 1.0),
           height = visibleSize.height - (50 * (i + 1));
    for (int j = 0; j < n; ++j) {
      auto enemy = Sprite::create(enemyPath);
      enemy->setAnchorPoint(Vec2(0.5, 0.5));
      enemy->setScale(0.5, 0.5);
      enemy->setPosition(width * (j + 1), height);
      enemys.push_back(enemy);
      addChild(enemy, 1);
    }
  }
}

// ‘… ØœÚœ¬“∆∂Ø≤¢…˙≥…–¬µƒ“ª––(º”∑÷œÓ)
void Thunder::newEnemy() {
    for (Sprite* s : enemys) {
        if (s != NULL) {
            s->setPosition(s->getPosition() + Vec2(0, -50));
        }
    }
    static int i = 0;
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", i+1);
    i = (i+1)%3;
    double width = visibleSize.width / (5.0+1.0),
    height = visibleSize.height - 50;
    for (int j = 0; j < 5; ++j) {
        auto enemy = Sprite::create(enemyPath);
        enemy->setAnchorPoint(Vec2(0.5, 0.5));
        enemy->setScale(0.5, 0.5);
        enemy->setPosition(width * (j+0.5), height);
        enemys.push_back(enemy);
        addChild(enemy, 1);
    }
}

// “∆∂Ø∑…¥¨
void Thunder::movePlane(char c) {
    MoveBy* moveBy;
    if (c == 'A') {
        moveBy = MoveBy::create(0.5f, Vec2(-5, 0));
    } else if (c == 'D') {
        moveBy = MoveBy::create(0.5f, Vec2(5, 0));
    }
            player->runAction(moveBy);
}

//∑¢…‰◊”µØ
void Thunder::fire() {
    auto bullet = Sprite::create("bullet.png");
    bullet->setAnchorPoint(Vec2(0.5, 0.5));
    bullets.push_back(bullet);
    bullet->setPosition(player->getPosition());
    addChild(bullet, 1);
    SimpleAudioEngine::getInstance()->playEffect("fire.wav", true);
    auto seq = Sequence::create(MoveBy::create(1.0f, Vec2(0, visibleSize.height)),CallFunc::create([bullet]{
        bullet->removeFromParentAndCleanup(true);
    }), CallFunc::create(CC_CALLBACK_0(Thunder::bulletPop, this)), NULL);
    bullet->runAction(seq);
}
void Thunder::bulletPop() {
    bullets.pop_front();
}

// «–∏Ó±¨’®∂Øª≠÷°
void Thunder::explosion() {
    auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
    explore.reserve(8);
    for (int i = 0; i < 5; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8*i,212.5,188.8,212.5)));
        explore.pushBack(frame);
    }
    for (int i = 0; i < 3; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(188.8*i,0,188.8,212.5)));
        explore.pushBack(frame);
    }
    auto animation = Animation::createWithSpriteFrames(explore, 0.1f);
   // animation->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(animation, "explosion");
    
}

void Thunder::update(float f) {
  //  µ ±∏¸–¬“≥√Êƒ⁄‘… Ø∫Õ◊”µØ ˝¡ø(≤ªµ√…æ≥˝)
  // “™«Û ˝¡øœ‘ æ’˝»∑(º”∑÷œÓ)
  char str[15];
  sprintf(str, "enemys: %d", enemys.size());
  enemysNum->setString(str);
  sprintf(str, "bullets: %d", bullets.size());
  bulletsNum->setString(str);

  // ∑…¥¨“∆∂Ø
  if (isMove)
    this->movePlane(movekey);

  static int ct = 0;
  static int dir = 4;
  ++ct;
  if (ct == 120)
    ct = 40, dir = -dir;
  else if (ct == 80) {
    dir = -dir;
    newEnemy();  // ‘… ØœÚœ¬“∆∂Ø≤¢…˙≥…–¬µƒ“ª––(º”∑÷œÓ)
  }
  else if (ct == 20)
    ct = 40, dir = -dir;

  //‘… Ø◊Û”““∆∂Ø
  for (Sprite* s : enemys) {
    if (s != NULL) {
      s->setPosition(s->getPosition() + Vec2(dir, 0));
    }
  }

  // ∑÷∑¢◊‘∂®“Â ¬º˛
  EventCustom e("meet");
  _eventDispatcher->dispatchEvent(&e);
}

// ◊‘∂®“Â≈ˆ◊≤ ¬º˛
void Thunder::meet(EventCustom * event) {
    auto explosion = Animate::create(AnimationCache::getInstance()->getAnimation("explosion"));
    for (auto it1 = bullets.begin(); it1 != bullets.end(); it1++) {
        for (auto it2 = enemys.begin(); it2 != enemys.end(); it2++) {
            if (*it1 == NULL || *it2 == NULL) {
                continue;
            } else if ((*it1)->getPosition().getDistance((*it2)->getPosition())<25) {
                auto temp1 = it1;
                it1 = bullets.erase(it1);
                (*temp1)->removeFromParentAndCleanup(true);
                auto temp = (*it2);
                (*it2)->runAction(Sequence::create(explosion, CallFunc::create([temp] {
                    temp->removeFromParentAndCleanup(true);
                }), NULL));
                it2 = enemys.erase(it2);
            }
        }
    }
    for (auto it = enemys.begin(); it != enemys.end(); it++) {
        if (player->getPosition().getDistance((*it)->getPosition())<25) {
            _eventDispatcher->removeAllEventListeners();
            player->runAction(Sequence::create(explosion, CallFunc::create([&] {
                player->removeFromParentAndCleanup(true);
            }), NULL));
            auto temp = *it;
            (*it)->runAction(Sequence::create(explosion, CallFunc::create([temp] {
                temp->removeFromParentAndCleanup(true);
            }), NULL));
            it = enemys.erase(it);
            break;
        }
    }
}
void Thunder::addCustomListener() {
    auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
    _eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
}

// ÃÌº”º¸≈Ã ¬º˛º‡Ã˝∆˜
void Thunder::addKeyboardListener() {
    auto KeyboardListener = EventListenerKeyboard::create();
    KeyboardListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
    KeyboardListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(KeyboardListener, 2);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_A:
    movekey = 'A';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
  case EventKeyboard::KeyCode::KEY_D:
    movekey = 'D';
    isMove = true;
    break;
  case EventKeyboard::KeyCode::KEY_SPACE:
    fire();
    break;
  }
}

void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case EventKeyboard::KeyCode::KEY_A:
  case EventKeyboard::KeyCode::KEY_CAPITAL_A:
  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case EventKeyboard::KeyCode::KEY_D:
  case EventKeyboard::KeyCode::KEY_CAPITAL_D:
    isMove = false;
    break;
  }
}

// ÃÌº”¥•√˛ ¬º˛º‡Ã˝∆˜
void Thunder::addTouchListener() {
    auto TouchListener = EventListenerTouchOneByOne::create();
    TouchListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
    TouchListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
    TouchListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListener, player);
}

//  Û±Íµ„ª˜∑¢…‰≈⁄µØ
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
    fire();
  return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
  isClick = false;
}

// µ± Û±Í∞¥◊°∑…¥¨∫Ûø…øÿ÷∆∑…¥¨“∆∂Ø (º”∑÷œÓ)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
    Vec2 delta = touch->getDelta();
    auto moveBy = MoveBy::create(1, Vec2(delta.x, 0));
    player->runAction(moveBy);
}
