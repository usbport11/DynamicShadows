# DynamicShadows
Cocos2d-x 4.0 Dynamic shadows

<b>Source articles</b><br>
https://github.com/mattdesl/lwjgl-basics/wiki/2D-Pixel-Perfect-Shadows<br>
https://github.com/namkazt/DynamicLight<br>
https://github.com/zerodarkzone/Cocos2d-x-lights<br>
https://github.com/JochenHeizmann/dynLight<br>
<br>
It is really work.<br><br>
<b>Code</b>
<pre>
  auto back = Sprite::create("resources/back.png");
  back->retain();
  back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
  this->addChild(back);

  auto shadowCaster = Sprite::create("resources/cat4.png");
  shadowCaster->retain();
  shadowCaster->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
  this->addChild(shadowCaster);

  dynLight = DynamicLight2::create();
  dynLight->retain();
  dynLight->setShadowCaster(shadowCaster);
  dynLight->setLightSize(512);
  dynLight->setPosition(lightPosition);
  this->addChild(dynLight);//important!

  auto darkArea = dynLight->getDarkAreaMapSprite();
  this->addChild(darkArea);//must be on top
</pre>
<br>
<b>Steps</b><br>
<ul>
  <li>
    Prepare shadow caster and background sprites<br>
    <img src='Explain/step1.png' width='320'><br><br>
  </li>
  <li>
    Create occlusion map<br>
    <img src='Explain/step2.png' width='320'><br><br>
  </li>
  <li>
    Create 1D shadow map (in code 16px)<br>
    <img src='Explain/step3.png' width='320'><br><br>
  </li>
  <li>
    Create final shadow map<br>
    <img src='Explain/step4.png' width='320'><br><br>
  </li>
  <li>
    Create dark area map (blending)<br>
    <img src='Explain/step5.png' width='320'><br><br>
  </li>
<ul>
<br>
