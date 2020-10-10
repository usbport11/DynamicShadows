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

	cocos2d::Color4B color;
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
	
	cocos2d::RenderTexture* darkAreaMap;
	cocos2d::Sprite* darkAreaMapSprite;
	
	void initOcclusionMap();
	void initShadowMap1D();
	void initFinalShadowMap();
	void initDarkAreaMap();
	
	void updateUniforms();
	
	void createOcclusionMap();
	void create1DShadowMap();
	void createFinalShadowMap();
	void createDarkAreaMap();
public:
	CREATE_FUNC(DynamicLight2);
	
	virtual bool init();
	cocos2d::backend::Program* loadShader(const std::string& vert, const std::string& frag);
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
	
	DynamicLight2();
	~DynamicLight2();
	
	void setPosition(const cocos2d::Point& position) override;
	void setSoftShadows(bool shadows);
	void setLightSize(int size);
	void setColor(cocos2d::Color4B c);
	void setShadowCaster(cocos2d::Sprite* caster);

	cocos2d::Sprite* getDarkAreaMapSprite();
};

#endif
