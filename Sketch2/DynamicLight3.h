#ifndef DynamicLight3_H
#define DynamicLight3_H

#include "cocos2d.h"
#include "renderer/backend/Device.h"

class DynamicLight3 : public cocos2d::Node {
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
	
	void initOcclusionMap();
	void initShadowMap1D();
	void initFinalShadowMap();
	
	void updateUniforms();
	
	void createOcclusionMap();
	void create1DShadowMap();
	void createFinalShadowMap();
public:
	static DynamicLight3* create(cocos2d::backend::ProgramState* shadowMap, cocos2d::backend::ProgramState* shadowRender);
	bool init(cocos2d::backend::ProgramState* shadowMap, cocos2d::backend::ProgramState* shadowRender);
	void drawLight();
	
	DynamicLight3();
	~DynamicLight3();
	
	void setSoftShadows(bool shadows);
	void setPosition(const cocos2d::Point& position) override;
	void setLightSize(int size);
	void setShadowCaster(cocos2d::Sprite* caster);

	cocos2d::Sprite* getFinalShadowMapSprite();
};

#endif