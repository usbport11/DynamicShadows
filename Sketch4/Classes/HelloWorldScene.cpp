#include "HelloWorldScene.h"
//#include "DynamicLight2.h"

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
	//dynLight = nullptr;
	dynLightManager = nullptr;
	//testLight = nullptr;
	lightPosition = { 450, 200 };
	memset(keybKeys, 0, 1024);
}

HelloWorld::~HelloWorld() {
	//CC_SAFE_RELEASE(dynLight);
	CC_SAFE_RELEASE(dynLightManager);
	//CC_SAFE_RELEASE(testLight);
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	if(!Scene::init())  {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//set backround
	auto back = Sprite::create("resources/back.png");
	back->retain();
	back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(back);
	//set shadowcater
	auto shadowCaster = Sprite::create("resources/casters1.png");
	shadowCaster->retain();
	shadowCaster->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//this->addChild(shadowCaster);
	auto lightCircle = Sprite::create("resources/tex0.png");
	lightCircle->retain();
	lightCircle->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//this->addChild(lightCircle);

	dynLightManager = DynamicLightManager3::create();
	dynLightManager->retain();
	dynLightManager->setShadowCaster(shadowCaster);
	dynLightManager->setAmbientColor({ 0.0, 0.0, 0.0, 0.01 });
	dynLightManager->addLightSource({ 450, 200 }, 384, lightCircle, "Light1");
	this->addChild(dynLightManager);//important!

	//enable update function
	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float delta) {
	if (dynLightManager) {
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

		DynamicLight3* dls = dynLightManager->getLightSource("Light1");
		if (dls) {
			dls->setPosition(lightPosition);
		}
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