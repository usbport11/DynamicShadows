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

void DynamicLightManager3::initLightAreaMap() {
	CC_SAFE_RELEASE(lightAreaMap);
	CC_SAFE_RELEASE(lightAreaMapSprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	lightAreaMap = RenderTexture::create(visibleSize.width, visibleSize.height);
	lightAreaMap->retain();
	lightAreaMapSprite = Sprite::createWithTexture(lightAreaMap->getSprite()->getTexture());
	lightAreaMapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	lightAreaMapSprite->setFlippedY(true);//important!
	lightAreaMapSprite->retain();
}

void DynamicLightManager3::initFinalAreaMap() {
	CC_SAFE_RELEASE(finalAreaMap);
	CC_SAFE_RELEASE(finalAreaMapSprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	finalAreaMap = RenderTexture::create(visibleSize.width, visibleSize.height);
	finalAreaMap->retain();
	finalAreaMapSprite = Sprite::createWithTexture(lightAreaMap->getSprite()->getTexture());
	finalAreaMapSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	finalAreaMapSprite->setProgramState(lightRenderShaderState);
	finalAreaMapSprite->retain();
}

void DynamicLightManager3::updateUniforms() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::backend::UniformLocation resolution = lightRenderShaderState->getUniformLocation("resolution");
	lightRenderShaderState->setUniform(resolution, &Vec2(visibleSize.width, visibleSize.height), sizeof(Vec2));
	cocos2d::backend::UniformLocation ambColor = lightRenderShaderState->getUniformLocation("ambientColor");
	lightRenderShaderState->setUniform(ambColor, &Vec4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a), sizeof(Vec4));
}

void DynamicLightManager3::createDarkAreaMap() {
	darkAreaMap->beginWithClear(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
	for (auto&& light : lightSources) {
		light.second->createLight();
		light.second->getFinalShadowMapSprite()->setPosition(light.second->getLightPosition());
		light.second->getFinalShadowMapSprite()->visit();
	}
	darkAreaMap->end();
}

void DynamicLightManager3::createLightAreaMap() {
	lightAreaMap->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	cocos2d::Sprite* lightSprite;
	for (auto&& light : lightSources) {
		lightSprite = light.second->getLightCircle();
		if (lightSprite) {
			lightSprite->setPosition(light.second->getLightPosition());
			lightSprite->visit();
		}
	}
	lightAreaMap->end();
}

void DynamicLightManager3::createFinalAreaMap() {
	cocos2d::backend::UniformLocation u_texture1 = lightRenderShaderState->getUniformLocation("u_texture1");
	lightRenderShaderState->setTexture(u_texture1, 0, shadowCaster->getTexture()->getBackendTexture());//zero important
	cocos2d::backend::UniformLocation u_texture2 = lightRenderShaderState->getUniformLocation("u_texture2");
	lightRenderShaderState->setTexture(u_texture2, 1, lightAreaMapSprite->getTexture()->getBackendTexture());
	finalAreaMap->beginWithClear(0.0f, 0.0f, 0.0f, 1.0f);
	finalAreaMapSprite->visit();
	finalAreaMap->end();
}
	
DynamicLightManager3::DynamicLightManager3() {
	shadowCaster = nullptr;
	darkAreaMap = nullptr;
	darkAreaMapSprite = nullptr;

	lightRenderShader = nullptr;
	lightRenderShaderState = nullptr;
	lightAreaMap = nullptr;
	lightAreaMapSprite = nullptr;
	finalAreaMap = nullptr;
	finalAreaMapSprite = nullptr;

	ambientColor = { 0.0, 0.0, 0.0, 0.05 };
}

DynamicLightManager3::~DynamicLightManager3() {
	CC_SAFE_RELEASE(shadowCaster);
	CC_SAFE_RELEASE(darkAreaMap);
	CC_SAFE_RELEASE(darkAreaMapSprite);

	CC_SAFE_RELEASE(lightRenderShader);
	CC_SAFE_RELEASE(lightRenderShaderState);
	CC_SAFE_RELEASE(lightAreaMap);
	CC_SAFE_RELEASE(lightAreaMapSprite);
	CC_SAFE_RELEASE(finalAreaMap);
	CC_SAFE_RELEASE(finalAreaMapSprite);

	lightSources.clear();
}

bool DynamicLightManager3::init() {
	if (!Node::init()) {
		return false;
	}

	lightRenderShader = loadShader("shaders/vert.glsl", "shaders/fragLightMap.glsl");
	if (!lightRenderShader) {
		return false;
	}
	lightRenderShaderState = new backend::ProgramState(lightRenderShader);
	if (!lightRenderShader) {
		return false;
	}

	initDarkAreaMap();
	initLightAreaMap();
	initFinalAreaMap();

	return true;
}

cocos2d::backend::Program* DynamicLightManager3::loadShader(const std::string& vert, const std::string& frag) {
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

void DynamicLightManager3::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	updateUniforms();

	createDarkAreaMap();
	createLightAreaMap();
	createFinalAreaMap();

	darkAreaMapSprite->visit();
	finalAreaMapSprite->visit();
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

void DynamicLightManager3::addLightSource(const cocos2d::Point& position, int size, cocos2d::Sprite* lightCircle, std::string name) {
	if (!shadowCaster) {
		return;
	}

	DynamicLight3* dynLight = DynamicLight3::create();
	dynLight->retain();
	dynLight->setShadowCaster(shadowCaster);
	dynLight->setLightSize(size);
	dynLight->setPosition(position);
	dynLight->setLightCircle(lightCircle);

	addLightSource(dynLight, name);
}

void DynamicLightManager3::removeLightSource(DynamicLight3* lightSource) {
	lightSources.erase(lightSource->getName());
}

void DynamicLightManager3::removeLightSource(const std::string& name) {
	lightSources.erase(name);
}

DynamicLight3* DynamicLightManager3::getLightSource(const std::string& name) {
	auto ret = lightSources.find(name);
	if (ret == lightSources.end()) {
		return nullptr;
	}
	return ret->second;
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