<b>Code</b>
<pre>
	//set backround
	auto back = Sprite::create("resources/back.png");
	back->retain();
	back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(back);
	//set shadowcater
	auto shadowCaster = Sprite::create("resources/casters1.png");
	shadowCaster->retain();
	shadowCaster->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	//set light circle
	auto lightCircle = Sprite::create("resources/tex0.png");
	lightCircle->retain();

	dynLightManager = DynamicLightManager3::create();
	dynLightManager->retain();
	dynLightManager->setShadowCaster(shadowCaster);
	dynLightManager->setAmbientColor({ 0.0, 0.0, 0.0, 0.01 });
	dynLightManager->addLightSource({ 450, 200 }, 384, lightCircle, "Light1");
	this->addChild(dynLightManager);//important!
</pre>
