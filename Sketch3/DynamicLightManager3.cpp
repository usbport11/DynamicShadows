#include "DynamicLightManager3.h"

USING_NS_CC;

void DynamicLightManager3::initDarkAreaMap() {
	CC_SAFE_RELEASE(darkAreaMap);
	CC_SAFE_RELEASE(darkAreaMapSprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	darkAreaMap = RenderTexture::create(visibleSize.width, visibleSize.height);
	darkAreaMap->retain();
	darkAreaMapSprite = Sprite::createWithTexture(darkAreaMap->getSprite()->getTexture());
	darkAreaMapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	darkAreaMapSprite->setBlendFunc({ backend::BlendFactor::SRC_COLOR , backend::BlendFactor::SRC_ALPHA });
	darkAreaMapSprite->setFlippedY(true);//important!
	darkAreaMapSprite->retain();
}

void DynamicLightManager3::createDarkAreaMap() {
	darkAreaMap->beginWithClear(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
	for (auto&& light : lightSources) {
		light.second->createLight();
		light.second->getFinalShadowMapSprite()->setPosition(light.second->getPositionX(), light.second->getPositionY());
		light.second->getFinalShadowMapSprite()->visit();
	}
	darkAreaMap->end();
}
	
DynamicLightManager3::DynamicLightManager3() {
	shadowCaster = nullptr;
	darkAreaMap = nullptr;
	darkAreaMapSprite = nullptr;

	ambientColor = { 0.0, 0.0, 0.0, 0.05 };
}

DynamicLightManager3::~DynamicLightManager3() {
	CC_SAFE_RELEASE(shadowCaster);
	CC_SAFE_RELEASE(darkAreaMap);
	CC_SAFE_RELEASE(darkAreaMapSprite);
	
	lightSources.clear();
}

bool DynamicLightManager3::init() {
	if (!Node::init()) {
		return false;
	}

	initDarkAreaMap();

	return true;
}

void DynamicLightManager3::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	createDarkAreaMap();
}

void DynamicLightManager3::addLightSource(DynamicLight3* lightSource, std::string name) {
	if (name == "") {
		return;
	}
	if(lightSources.find(name) != lightSources.end()) {
		return;
	}
	lightSource->setName(name);
	lightSources.insert(name, lightSource);
}

void DynamicLightManager3::addLightSource(const cocos2d::Point& position, int size, std::string name) {
	if(!shadowCaster) {
		return;
	}
	
	DynamicLight3* dynLight = DynamicLight3::create();
	dynLight->retain();
	dynLight->setShadowCaster(shadowCaster);
	dynLight->setLightSize(size);
	dynLight->setPosition(position);
	
	addLightSource(dynLight, name);
}

void DynamicLightManager3::removeLightSource(DynamicLight3* lightSource) {
	lightSources.erase(lightSource->getName());
}

void DynamicLightManager3::removeLightSource(const std::string& name) {
	lightSources.erase(name);
}

DynamicLight3* DynamicLightManager3::getLightSource(const std::string& name) {
	return lightSources.find(name)->second;
}

//sets
void DynamicLightManager3::setAmbientColor(cocos2d::Color4F c) {
	if (this->ambientColor != c) {
		this->ambientColor = c;
	}
}

void DynamicLightManager3::setShadowCaster(cocos2d::Sprite* caster) {
	CC_SAFE_RELEASE(shadowCaster);

	shadowCaster = Sprite::createWithTexture(caster->getTexture());
	shadowCaster->setAnchorPoint(caster->getAnchorPoint());
	shadowCaster->setPosition(caster->getPosition());
	shadowCaster->retain();
}

//gets
cocos2d::Sprite* DynamicLightManager3::getDarkAreaMapSprite() {
	return darkAreaMapSprite;
}
