#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DynamicLight2.h"

class HelloWorld : public cocos2d::Scene {
private:
	DynamicLight2* dynLight;
	cocos2d::Point lightPosition;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(HelloWorld);
};

#endif
