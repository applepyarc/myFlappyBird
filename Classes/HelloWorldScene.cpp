#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    screenSize = Director::getInstance()->getVisibleSize();
    initWorld();
    addBird();
    addBarContainer();
    addGround();

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::startGame), 3);
    return true;
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    bird->getB2Body()->SetLinearVelocity(b2Vec2(0, 5));
}

void HelloWorld::initWorld()
{
    world = new b2World(b2Vec2(0, -10));
    world->SetContactListener(this);
}

void HelloWorld::addBird()
{
    bird = PhysicsSprite::create("bird.png");
    Size size = bird->getContentSize();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(screenSize.width / 2 / RATIO, screenSize.height/2/RATIO);
    b2Body * birdBody = world->CreateBody(&bodyDef);

    b2PolygonShape birdShape;
    birdShape.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);

    b2FixtureDef birdFixtureDef;
    birdFixtureDef.shape = &birdShape;
    birdBody->CreateFixture(&birdFixtureDef);

    bird->setPTMRatio(RATIO);
    bird->setB2Body(birdBody);
    this->addChild(bird);
}

void HelloWorld::addGround(){

    PhysicsSprite *ground = PhysicsSprite::create("ground.png");
    Size size = ground->getContentSize();

    b2BodyDef bDef;
    bDef.type = b2_staticBody;
    bDef.position = b2Vec2(size.width / 2 / RATIO, size.height / 2 / RATIO);
    b2Body *groundBody = world->CreateBody(&bDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(size.width / 2 / RATIO, size.height / 2 / RATIO);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundBody->CreateFixture(&groundFixtureDef);

    ground->setB2Body(groundBody);
    ground->setPTMRatio(RATIO);
    addChild(ground);
}

void HelloWorld::addBarContainer()
{
    barContainer = Sprite::create();
    addChild(barContainer);
}

void HelloWorld::addBar(float dt)
{
    float offset = -rand() % 5;

    //down bar
    PhysicsSprite * down_bar = PhysicsSprite::create("down_bar.png");
    Size down_bar_size = down_bar->getContentSize();

    b2BodyDef down_bar_body_def;
    down_bar_body_def.type = b2_kinematicBody;
    down_bar_body_def.position = b2Vec2(screenSize.width / RATIO + 10, down_bar_size.height / RATIO / 2 + offset);
    down_bar_body_def.linearVelocity = b2Vec2(-5, 0);
    b2Body * down_bar_body = world->CreateBody(&down_bar_body_def);

    b2PolygonShape down_bar_shape;
    down_bar_shape.SetAsBox(down_bar_size.width / 2 / RATIO, down_bar_size.height / 2 / RATIO);
    b2FixtureDef down_bar_fixture_def;
    down_bar_fixture_def.shape = &down_bar_shape;
    down_bar_body->CreateFixture(&down_bar_fixture_def);

    down_bar->setB2Body(down_bar_body);
    down_bar->setPTMRatio(RATIO);
    barContainer->addChild(down_bar);

    //up bar
    PhysicsSprite * up_bar = PhysicsSprite::create("up_bar.png");
    Size up_bar_size = up_bar->getContentSize();

    b2BodyDef up_bar_body_def;
    up_bar_body_def.type = b2_kinematicBody;
    up_bar_body_def.position = b2Vec2(screenSize.width / RATIO + 10, down_bar_size.height / RATIO + offset + 2 + up_bar_size.height / 2 / RATIO);
    up_bar_body_def.linearVelocity = b2Vec2(-5, 0);
    b2Body * up_bar_body = world->CreateBody(&up_bar_body_def);

    b2PolygonShape up_bar_shape;
    up_bar_shape.SetAsBox(up_bar_size.width / 2 / RATIO, up_bar_size.height / 2 / RATIO);
    b2FixtureDef up_bar_fixture_def;
    up_bar_fixture_def.shape = &up_bar_shape;
    up_bar_body->CreateFixture(&up_bar_fixture_def);

    up_bar->setB2Body(up_bar_body);
    up_bar->setPTMRatio(RATIO);
    barContainer->addChild(up_bar);
}

void HelloWorld::update(float dt){
    world->Step(dt, 8, 3);
    Sprite *s;

    for (b2Body *b = world->GetBodyList(); b != NULL; b = b->GetNext())
    {
        if (b->GetPosition().x<-3)
        {
            s = (Sprite*)b->GetUserData();
            if (s != NULL)
            {
                s->removeFromParent();
            }
            world->DestroyBody(b);
        }
    }
}

void HelloWorld::BeginContact(b2Contact *contact)
{
    if (contact->GetFixtureA()->GetBody() == bird->getB2Body() ||
        contact->GetFixtureB()->GetBody() == bird->getB2Body()) 
    {
        stopGame();
        MessageBox("GameOver", "Game Over");
    }
}

void HelloWorld::startGame(float dt)
{
    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addBar), 1);
}

void HelloWorld::stopGame()
{
    unscheduleUpdate();
    unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::addBar));
}
