#include "HelloWorldScene.h"
#include "DynamicLight.h"

USING_NS_CC;

Scene* HelloWorld::createScene() {
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename) {
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

HelloWorld::HelloWorld() {
	dynLight = nullptr;
	lightPosition = { 500, 200 };
}

HelloWorld::~HelloWorld() {
	CC_SAFE_RELEASE(dynLight);
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	if(!Scene::init())  {
		return false;
	}
	
	lightPosition = {300, 200};

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto shadowCaster = Sprite::create("resources/cat4.png");
	shadowCaster->retain();
	shadowCaster->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(shadowCaster, 8);
	auto back = Sprite::create("resources/back.png");
	back->retain();
	back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(back);

	dynLight = DynamicLight2::create();
	dynLight->retain();
	dynLight->setShadowCaster(shadowCaster);
	dynLight->setLightSize(256);
	dynLight->setPosition(lightPosition);
	this->addChild(dynLight, 9);

	//enable update function
	this->scheduleUpdate();
	
	return true;
}

void HelloWorld::update(float delta) {
	if (dynLight) {
		if (keybKeys[(int)EventKeyboard::KeyCode::KEY_UP_ARROW]) {
			lightPosition.y += 1;
		}
		if (keybKeys[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW]) {
			lightPosition.y -= 1;
		}
		if (keybKeys[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW]) {
			lightPosition.x -= 1;
		}
		if (keybKeys[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW]) {
			lightPosition.x += 1;
		}
		dynLight->setPosition(lightPosition);
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	keybKeys[(int)keyCode] = 1;
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	keybKeys[(int)keyCode] = 0;
}
