#include "DynamicLightManager2.h"

USING_NS_CC;

void DynamicLightManager2::initDarkAreaMap() {
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

void DynamicLightManager2::createDarkAreaMap() {
	for (auto&& light : lightSources) {
		light.second->drawLight();
	}
	darkAreaMap->beginWithClear(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
	for (auto&& light : lightSources) {
		light.second->setPosition(getPositionX(), getPositionY());
		light.second->finalShadowMapSprite->visit();
	}
	darkAreaMap->end();
}
	
DynamicLightManager2::DynamicLightManager2() {
	shadowMapShaderState = nullptr;
	shadowRenderShaderState = nullptr;
	shadowMapShader = nullptr;
	shadowRenderShader = nullptr;
	darkAreaMap = nullptr;
	darkAreaMapSprite = nullptr;
}

DynamicLightManager2::~DynamicLightManager2() {
	CC_SAFE_RELEASE(shadowRenderShaderState);
	CC_SAFE_RELEASE(shadowMapShaderState);
	CC_SAFE_RELEASE(shadowRenderShader);
	CC_SAFE_RELEASE(shadowMapShader);
	CC_SAFE_RELEASE(shadowCaster);
	CC_SAFE_RELEASE(darkAreaMap);
	CC_SAFE_RELEASE(darkAreaMapSprite);
	
	lightSources.clear();
}

bool DynamicLightManager2::init() {
	if (!Node::init()) {
		return false;
	}
	lightSources.reserve(MAX_LIGHT_NUM);
	
	//load shaders
	shadowMapShader = loadShader("shaders/vert.glsl", "shaders/fragShadowMap.glsl");
	shadowRenderShader = loadShader("shaders/vert.glsl", "shaders/fragShadowRender.glsl");
	if (!shadowMapShader || !shadowRenderShader) {
		return false;
	}
	shadowMapShaderState = new backend::ProgramState(shadowMapShader);
	shadowRenderShaderState = new backend::ProgramState(shadowRenderShader);
	if (!shadowMapShaderState || !shadowRenderShaderState) {
		return false;
	}
	
	initDarkAreaMap();
}

cocos2d::backend::Program* DynamicLightManager2::DynamicLightManager2::loadShader(const std::string& vert, const std::string& frag) {
	if (vert.empty() || frag.empty()) {
		return nullptr;
	}
	auto fileUtiles = FileUtils::getInstance();
	auto vertexFilePath = fileUtiles->fullPathForFilename(vert);
	auto vertSource = fileUtiles->getStringFromFile(vertexFilePath);
	auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);
	if (vertSource.empty() || fragSource.empty()) {
		return nullptr;
	}
	auto device = backend::Device::getInstance();
	cocos2d::backend::Program* program = device->newProgram(vertSource.c_str(), fragSource.c_str());
	return program;
}

void DynamicLightManager2::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	if(!bakedMapIsValid) {
		bakedMapIsValid = true;
		createDarkAreaMap();
	}
}

void DynamicLightManager2::addLightSource(DynamicLight2* lightSource, std::string name) {
	if (name == "") {
		return;
	}
	if(lightSources.find(name) != lightSources.end()) {
		return;
	}
	lightSource->setName(name);
	lightSources.insert(name, lightSource);
}

void DynamicLightManager2::addLightSource(const cocos2d::Point& position, int size, std::string name) {
	if(!shadowCaster) {
		return;
	}
	DynamicLight2* dynLight = DynamicLight2::create();
	dynLight->retain();
	dynLight->setShadowCaster(shadowCaster);
	dynLight->setLightSize(size);
	dynLight->setPosition(position);
	addLightSource(dynLight, name);
}

void DynamicLightManager2::removeLightSource(DynamicLight* lightSource) {
	lightSources.erase(lightSource->getName());
}

void DynamicLightManager2::removeLightSource(const std::string& name) {
	lightSources.erase(name);
}

//sets
void DynamicLightManager2::setAmbientColor(cocos2d::Color4F c) {
	if (this->ambientColor != c) {
		this->ambientColor = c;
		bakedMapIsValid = false;
	}
}

void DynamicLightManager2::setShadowCaster(cocos2d::Sprite* caster) {
	CC_SAFE_RELEASE(shadowCaster);

    bakedMapIsValid = false;

	shadowCaster = Sprite::createWithTexture(caster->getTexture());
	shadowCaster->setAnchorPoint(caster->getAnchorPoint());
	shadowCaster->setPosition(caster->getPosition());
	shadowCaster->retain();
}

//gets
cocos2d::Sprite* DynamicLightManager2::getDarkAreaMapSprite(); {
	return darkAreaMapSprite;
}

cocos2d::Sprite* DynamicLightManager2::getShadowCaster() {
	return shadowCaster;
}

cocos2d::backend::ProgramState* DynamicLightManager2::getShadowMapShaderState() {
	return shadowMapShaderState;
}

cocos2d::backend::ProgramState* DynamicLightManager2::getShadowRenderShaderState() {
	return shadowRenderShaderState;
}

bool DynamicLightManager2::isBackedMapValid() {
	return backedMapisValid;
}