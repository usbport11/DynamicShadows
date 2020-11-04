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
	cocos2d::Sprite* lightCircle;

	cocos2d::backend::Program* shadowMapShader;
	cocos2d::backend::ProgramState* shadowMapShaderState;
	cocos2d::backend::Program* shadowRenderShader;
	cocos2d::backend::ProgramState* shadowRenderShaderState;
	
	cocos2d::RenderTexture* occlusionMap;
	cocos2d::Sprite* occlusionMapSprite;
	cocos2d::RenderTexture* shadowMap1D;
	cocos2d::Sprite* shadowMap1DSprite;
	cocos2d::RenderTexture* finalShadowMap;
	cocos2d::Sprite* finalShadowMapSprite;
	
	void initOcclusionMap();
	void initShadowMap1D();
	void initFinalShadowMap();
	
	void updateUniforms();
	
	void createOcclusionMap();
	void create1DShadowMap();
	void createFinalShadowMap();

	cocos2d::backend::Program* loadShader(const std::string& vert, const std::string& frag);
public:
	CREATE_FUNC(DynamicLight3);
	virtual bool init();
	bool createLight();
	
	DynamicLight3();
	~DynamicLight3();
	
	void setSoftShadows(bool shadows);
	void setPosition(const cocos2d::Point& position) override;
	void setLightSize(int size);
	void setShadowCaster(cocos2d::Sprite* caster);

	cocos2d::Sprite* getFinalShadowMapSprite();
	int getLightSize();
	cocos2d::Vec2 getLightPosition();

	cocos2d::Sprite* getLightCircle();
	void setLightCircle(cocos2d::Sprite* circle);
};

#endif