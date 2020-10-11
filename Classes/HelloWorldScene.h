#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DynamicLight2.h"

class HelloWorld : public cocos2d::Scene {
private:
	DynamicLight2* dynLight;
	cocos2d::Point lightPosition;
	bool keybKeys[1024];
public:
	HelloWorld();
	~HelloWorld();
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float delta);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	CREATE_FUNC(HelloWorld);
};

#endif
