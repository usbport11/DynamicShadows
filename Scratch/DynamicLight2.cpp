#include "DynamicLight2.h"

USING_NS_CC;

DynamicLight2::DynamicLight2() {
	softShadows = true;
	lightSize = 256;
	upScale = 1.0f;
	accuracy = 1.0f;
	bakedMapIsValid = false;
	finalSize = 256;
	additive = false;

	shadowCaster = nullptr;
	
	occlusionMap = nullptr;
	occlusionMapSprite = nullptr;
	shadowMap1D = nullptr;
	shadowMap1DSprite = nullptr;
	finalShadowMap = nullptr;
	finalShadowMapSprite = nullptr;
}

DynamicLight2::~DynamicLight2() {
	CC_SAFE_RELEASE(occlusionMap);
	CC_SAFE_RELEASE(shadowMap1D);
	CC_SAFE_RELEASE(finalShadowMap);
	CC_SAFE_RELEASE(finalShadowMapSprite);
}

void DynamicLight2::initOcclusionMap() {
	CC_SAFE_RELEASE(occlusionMap);
	CC_SAFE_RELEASE(occlusionMapSprite);

	occlusionMap = RenderTexture::create(lightSize, lightSize);
	occlusionMap->retain();
	occlusionMapSprite = Sprite::createWithTexture(occlusionMap->getSprite()->getTexture());
	occlusionMapSprite->setFlippedY(true);
	occlusionMapSprite->setProgramState(shadowMapShaderState);//link program to sprite
	occlusionMapSprite->retain();
}

void DynamicLight2::initShadowMap1D() {
	CC_SAFE_RELEASE(shadowMap1D);
	CC_SAFE_RELEASE(shadowMap1DSprite);

	shadowMap1D = RenderTexture::create(lightSize, 16);//16
	shadowMap1D->retain();
	shadowMap1DSprite = Sprite::createWithTexture(shadowMap1D->getSprite()->getTexture());
	shadowMap1DSprite->retain();
}

void DynamicLight2::initFinalShadowMap() {
	CC_SAFE_RELEASE(finalShadowMap);
	CC_SAFE_RELEASE(finalShadowMapSprite);

	finalSize = lightSize * upScale;

	finalShadowMap = RenderTexture::create(finalSize, finalSize);
	finalShadowMap->retain();
	finalShadowMapSprite = Sprite::createWithTexture(finalShadowMap->getSprite()->getTexture());
	finalShadowMapSprite->setProgramState(shadowRenderShaderState);//link program to sprite
	finalShadowMapSprite->retain();
}

void DynamicLight2::updateUniforms() {
	cocos2d::backend::UniformLocation resolutionMS_ = shadowMapShaderState->getUniformLocation("resolution");
	cocos2d::backend::UniformLocation upScale_ = shadowMapShaderState->getUniformLocation("upScale");
	cocos2d::backend::UniformLocation accuracy_ = shadowMapShaderState->getUniformLocation("accuracy");
	shadowMapShaderState->setUniform(resolutionMS_, &Vec2(lightSize, lightSize), sizeof(Vec2));
	shadowMapShaderState->setUniform(upScale_, &upScale, sizeof(float));
	shadowMapShaderState->setUniform(accuracy_, &accuracy, sizeof(float));

	float softShadowsSize = 0.0f;
	if (softShadows) softShadowsSize = 1.0f;
	cocos2d::backend::UniformLocation resolutionRS_ = shadowRenderShaderState->getUniformLocation("resolution");
	cocos2d::backend::UniformLocation softShadows_ = shadowRenderShaderState->getUniformLocation("softShadows");
	shadowRenderShaderState->setUniform(resolutionRS_, &Vec2(lightSize, lightSize), sizeof(Vec2));
	shadowRenderShaderState->setUniform(softShadows_, &softShadowsSize, sizeof(float));
}

void DynamicLight2::createOcclusionMap() {
	if (!shadowCaster) {
		occlusionMap->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
		occlusionMap->end();
		return;
	}

	Point p1 = shadowCaster->getAnchorPoint();
	Point p2 = shadowCaster->getPosition();
	auto x = shadowCaster->getPositionX() - (getPositionX() - (lightSize / 2));
	auto y = shadowCaster->getPositionY() - (getPositionY() - (lightSize / 2));

	occlusionMap->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
		shadowCaster->setPosition(x, y);
		shadowCaster->visit();
	occlusionMap->end();
	shadowCaster->setAnchorPoint(p1);
	shadowCaster->setPosition(p2);
}

void DynamicLight2::create1DShadowMap() {
	//for debug draw only
	occlusionMapSprite->setAnchorPoint({0, 0});
	//create 1d bw shadow map
	shadowMap1D->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
		occlusionMapSprite->visit();
	shadowMap1D->end();
}

void DynamicLight2::createFinalShadowMap() {
	//difficult to say that this function create final shadow map
	cocos2d::backend::UniformLocation u_texture2 = shadowRenderShaderState->getUniformLocation("u_texture2");
	shadowRenderShaderState->setTexture(u_texture2, 1, shadowMap1DSprite->getTexture()->getBackendTexture());
}

bool DynamicLight2::init() {
	if (!Node::init()) {
        return false;
    }

	if (!shadowMapShader || !shadowRenderShader) {
		return false;
	}
	if (!shadowMapShaderState || !shadowRenderShaderState) {
		return false;
	}

	//create empty sprites and textures
	initOcclusionMap();
	initShadowMap1D();
	initFinalShadowMap();
	bakedMapIsValid = false;
	
	return true;
}

void DynamicLight2::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
	if (!bakedMapIsValid) {
		bakedMapIsValid = true;

		updateUniforms();

		createOcclusionMap();
		create1DShadowMap();
		createFinalShadowMap();
	}
}

void DynamicLight2::setPosition(const cocos2d::Point& position) {
	if (position.x == getPosition().x && position.y == getPosition().y) {
		return;
	}
	Node::setPosition(position);//important!
	bakedMapIsValid = false;
}

void DynamicLight2::setSoftShadows(bool shadows) {
	softShadows = shadows;
}

void DynamicLight2::setLightSize(int size) {
	if (this->lightSize != size) {
		if (size < 0) lightSize = 0;
		if (size > 1200) {
			this->lightSize = 1200;
		}
		else {
			this->lightSize = size;
		}
		initOcclusionMap();
		initShadowMap1D();
		initFinalShadowMap();
		bakedMapIsValid = false;
	}
}

void DynamicLight2::setShadowCaster(cocos2d::Sprite* caster) {
	CC_SAFE_RELEASE(shadowCaster);
	
	bakedMapIsValid = false;

	shadowCaster = Sprite::createWithTexture(caster->getTexture());
	shadowCaster->setAnchorPoint(caster->getAnchorPoint());
	shadowCaster->setPosition(caster->getPosition());
	shadowCaster->retain();
}
