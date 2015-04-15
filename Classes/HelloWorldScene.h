#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CCPhysicsSprite.h"
//#include "B2Sprite.h"
//#include "physics-nodes\CCPhysicsSprite.h"
//#include "cocos2d/extensions/cocos-ext.h"

#define RATIO 48.0f

class HelloWorld : public cocos2d::Layer, public b2ContactListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void initWorld();
    void addBird();
    void addBarContainer();
    void addGround();
    void addBar(float dt);
    void startGame(float dt);
    void stopGame();

    virtual void update(float dt);
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void BeginContact(b2Contact* contact);

private:
    b2World *world;
    PhysicsSprite *bird;
    Size screenSize;
    Sprite *barContainer;
};

#endif // __HELLOWORLD_SCENE_H__
