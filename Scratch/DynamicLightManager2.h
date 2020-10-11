#ifndef DYNAMICLIGHTMANAGER2_H
#define DYNAMICLIGHTMANAGER2_H

#include "cocos2d.h"
#include "renderer/backend/Device.h"
#include "DynamicLight2.h"

#define MAX_LIGHT_NUM 20

class DynamicLightManager2 : public cocos2d::Node {
private:
	cocos2d::Map<std::string, DynamicLight2*> lightSources;
	
	cocos2d::Color4F ambientColor;
	cocos2d::Sprite* shadowCaster;
	
	cocos2d::backend::ProgramState* shadowMapShaderState;
	cocos2d::backend::ProgramState* shadowRenderShaderState;
	cocos2d::backend::Program* shadowMapShader;
	cocos2d::backend::Program* shadowRenderShader;
	
	cocos2d::RenderTexture* darkAreaMap;
	cocos2d::Sprite* darkAreaMapSprite;

	void initDarkAreaMap();
	void createDarkAreaMap();
	
	cocos2d::backend::Program* loadShader(const std::string& vert, const std::string& frag);
public:
	CREATE_FUNC(DynamicLightManager2);
	
	DynamicLightManager2();
	~DynamicLightManager2();
	
	virtual bool init();
	void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
	
	//manage light sources
	void addLightSource(DynamicLight2* lightSource, std::string name = "");
	void addLightSource(const cocos2d::Point& position, int size);
	void removeLightSource(DynamicLight* lightSource);
	void removeLightSource(const std::string& name);
	DynamicLight* getLightSource(const std::string& name);
	
	//sets
	void setAmbientColor(cocos2d::Color4F c);
	void setShadowCaster(cocos2d::Sprite* caster);

	//gets
	cocos2d::Sprite* getDarkAreaMapSprite();
	cocos2d::Sprite* getShadowCaster();
	cocos2d::backend::ProgramState* getShadowMapShaderState();
	cocos2d::backend::ProgramState* getShadowRenderShaderState();
	bool isBackedMapValid();
};

#endif