#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    // 获取屏幕宽度和高度
    _screenWidth = visibleSize.width;
    _screenHeight = visibleSize.height;
    
    _tileMap = TMXTiledMap::create("RedAlertMap.tmx");
    _tileMap->setAnchorPoint(Vec2::ZERO);
    _tileMap->setPosition(Vec2::ZERO);
    this->addChild(_tileMap,-1);

    
    auto objects = _tileMap->getObjectGroup("MiningYard");
    ValueMap map = objects->getObject("_MiningYard");
    int x = map.at("x").asInt();
    int y = map.at("y").asInt();
    _player = Sprite::create("player.png");
    _player->setPosition(x,y);
    _tileMap->addChild(_player);
    
    camera = Sprite::create("camera.png");
    camera->setPosition(Vec2(_screenWidth/2,_screenHeight/2));
    this->addChild(camera);
    
    
    
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = true;
    };
    //listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    this->scheduleUpdate();
    
    return true;
}



void HelloWorld::setViewpointCenter(Vec2 position)
{
    /*
    int x = MAX(position.x, _screenWidth/2);
    int y = MAX(position.y, _screenHeight/2);
    x = MIN(x, _tileMap->getMapSize().width * _tileMap->getTileSize().width - _screenWidth/2);
    x = MIN(y, _tileMap->getMapSize().height * _tileMap->getTileSize().height - _screenHeight/2);
    
    */
    Point actualPosition = Vec2(position.x,position.y);
    Point centerofView = Vec2(_screenWidth/2,_screenHeight/2);
    Point viewPoint = centerofView - actualPosition;
    this->setPosition(viewPoint);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void HelloWorld::update(float delta)
{
    Node::update(delta);
    auto leftArrow = EventKeyboard::KeyCode::KEY_A, rightArrow = EventKeyboard::KeyCode::KEY_D,upArrow = EventKeyboard::KeyCode::KEY_W,downArrow = EventKeyboard::KeyCode::KEY_S;
    if(isKeyPressed(leftArrow))
    {
        keyPressedDuration(leftArrow);
    }
    else if(isKeyPressed(rightArrow))
    {
        keyPressedDuration(rightArrow);
    }
    else if(isKeyPressed(upArrow))
    {
        keyPressedDuration(upArrow);
    }
    else if(isKeyPressed(downArrow))
    {
        keyPressedDuration(downArrow);
    }
    this->setViewpointCenter(camera->getPosition());
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
    if(keys[keyCode]) {
        return true;
    } else {
        return false;
    }
}
void HelloWorld::keyPressedDuration(EventKeyboard::KeyCode code)
{
    int offsetX = 0, offsetY = 0;
    switch (code) {
        case EventKeyboard::KeyCode::KEY_A:
            if ((int)camera->getPositionX() > _screenWidth/2)
            {
                offsetX = -10;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if ((int)camera->getPositionX() < _tileMap->getMapSize().width * _tileMap->getTileSize().width - _screenWidth/2 - 10)
            {
                offsetX = 10;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if ((int)camera->getPositionY() < _tileMap->getMapSize().height * _tileMap->getTileSize().height - _screenHeight/2 - 10)
            {
                offsetY = 10;
            }
            else
            {
                offsetY = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if ((int)camera->getPositionY() > _screenHeight/2)
            {
                offsetY = -10;
            }
            else
            {
                offsetY = 0;
            }
            break;
        default:
            offsetY = offsetX = 0;
            break;
    }
    // 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
    
    auto moveTo = MoveTo::create(0, Vec2(camera->getPositionX() + offsetX, camera->getPositionY() + offsetY));
    camera->runAction(moveTo);
}

