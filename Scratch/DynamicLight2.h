#ifndef DYNAMICLIGHT2_H
#define DYNAMICLIGHT2_H

#include "cocos2d.h"
#include "renderer/backend/Device.h"

class DynamicLight2 : public cocos2d::Node {
private:
	bool softShadows;
	int lightSize;
	float upScale;
	bool bakedMapIsValid;
	float finalSize;
	float accuracy;
	bool additive;

	cocos2d::Sprite* shadowCaster;
	
	cocos2d::RenderTexture* occlusionMap;
	cocos2d::Sprite* occlusionMapSprite;
	cocos2d::RenderTexture* shadowMap1D;
	cocos2d::Sprite* shadowMap1DSprite;
	cocos2d::RenderTexture* finalShadowMap;
	cocos2d::Sprite* finalShadowMapSprite;
	
	cocos2d::backend::ProgramState* shadowMapShaderState;
	cocos2d::backend::ProgramState* shadowRenderShaderState;
	cocos2d::backend::Program* shadowMapShader;
	cocos2d::backend::Program* shadowRenderShader;
	
	void initOcclusionMap();
	void initShadowMap1D();
	void initFinalShadowMap();
	
	void updateUniforms();
	
	void createOcclusionMap();
	void create1DShadowMap();
	void createFinalShadowMap();
public:
	CREATE_FUNC(DynamicLight2);
	
	virtual bool init();
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
	
	DynamicLight2();
	~DynamicLight2();
	
	void setSoftShadows(bool shadows);
	void setPosition(const cocos2d::Point& position) override;
	void setLightSize(int size);
	void setShadowCaster(cocos2d::Sprite* caster);
};

#endif