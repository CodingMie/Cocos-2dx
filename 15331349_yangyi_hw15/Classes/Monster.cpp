#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame(){
	auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
	monsterDead.reserve(4);
	for (int i = 0; i < 4; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
		monsterDead.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
	monster.pushBack(mons);
	return mons;
}

void Factory::removeMonster(Sprite* sp) {
    Animation* animation = Animation::createWithSpriteFrames(monsterDead, 0.1f);
    Animate* animate = Animate::create(animation);
    Sequence* seq = Sequence::create(DelayTime::create(1),animate, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)),NULL);
    sp->runAction(seq);
    monster.eraseObject(sp);
}
void Factory::moveMonster(Vec2 playerPos,float time){
    for (auto i = monster.begin(); i != monster.end(); i++) {
        Vec2 pos = (*i)->getPosition();
        Vec2 direction = playerPos-pos;
        direction.normalize();
        (*i)->runAction(MoveBy::create(time, direction*30));
    }
}

Sprite* Factory::collider(Rect rect) {
    for (auto i = monster.begin(); i != monster.end(); i++) {
        if (rect.containsPoint((*i)->getPosition())) {
            return *i;
        }
    }
    return NULL;
}