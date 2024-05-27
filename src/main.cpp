#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;

CCNode* keystrokes;

class $modify(PlayerObject) {
	$override
	void pushButton(PlayerButton p0) {
		PlayerObject::pushButton(p0);	
		
		if(!keystrokes) return;
		if(!m_isPlatformer && p0 != PlayerButton::Jump) return;
		if(!PlayLayer::get()) return;

		int expectedCount = 1;
		if(m_isPlatformer) expectedCount *= 3;
		if(PlayLayer::get()->m_level->m_twoPlayerMode) expectedCount *= 2;
		if(keystrokes->getChildrenCount() < expectedCount) return;

		std::string player = (PlayLayer::get()->m_level->m_twoPlayerMode && this == PlayLayer::get()->m_player2) ? "p2-" : "p1-";

		switch(p0) {
			case PlayerButton::Left:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "left"))->setColor({255, 255, 255});
				break;
			case PlayerButton::Jump:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "jump"))->setColor({255, 255, 255});
				break;
			case PlayerButton::Right:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "right"))->setColor({255, 255, 255});
				break;
		}
	}

	$override
	void releaseButton(PlayerButton p0) {
		PlayerObject::releaseButton(p0);

		if(!keystrokes) return;
		if(!m_isPlatformer && p0 != PlayerButton::Jump) return;
		if(!PlayLayer::get()) return;

		int expectedCount = 1;
		if(m_isPlatformer) expectedCount *= 3;
		if(PlayLayer::get()->m_level->m_twoPlayerMode) expectedCount *= 2;
		if(keystrokes->getChildrenCount() < expectedCount) return;

		std::string player = (PlayLayer::get()->m_level->m_twoPlayerMode && this == PlayLayer::get()->m_player2) ? "p2-" : "p1-";
		switch(p0) {
			case PlayerButton::Left:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "left"))->setColor({127, 127, 127});
				break;
			case PlayerButton::Jump:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "jump"))->setColor({127, 127, 127});
				break;
			case PlayerButton::Right:
				static_cast<CCSprite*>(keystrokes->getChildByID(player + "right"))->setColor({127, 127, 127});
				break;
		}
	}

	$override
	void playerDestroyed(bool p0) {
		releaseButton(PlayerButton::Jump);
		PlayerObject::playerDestroyed(p0);
	}
};

class $modify(KeystrokesLayer, PlayLayer) {
	$override
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if(!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

		keystrokes = CCNode::create();
		keystrokes->setID("keystrokes"_spr);
		keystrokes->setScale(Mod::get()->getSettingValue<double>("scale"));

		if(level->isPlatformer()) {
			keystrokes->setContentWidth(185.5f);

			auto p1Left = CCSprite::createWithSpriteFrameName("PBtn_Move_001.png");
			p1Left->setID("p1-left");
			p1Left->setColor({127, 127, 127});
			keystrokes->addChild(p1Left);

			auto p1Jump = CCSprite::createWithSpriteFrameName("PBtn_Jump_001.png");
			p1Jump->setID("p1-jump");
			p1Jump->setColor({127, 127, 127});
			keystrokes->addChild(p1Jump);

			auto p1Right = CCSprite::createWithSpriteFrameName("PBtn_Move_001.png");
			p1Right->setID("p1-right");
			p1Right->setColor({127, 127, 127});
			p1Right->setFlipX(true);
			keystrokes->addChild(p1Right);

			if(level->m_twoPlayerMode) {
				auto p2Left = CCSprite::createWithSpriteFrameName("PBtn_Move_001.png");
				p2Left->setID("p2-left");
				p2Left->setColor({127, 127, 127});
				keystrokes->addChild(p2Left);

				auto p2Jump = CCSprite::createWithSpriteFrameName("PBtn_Jump_001.png");
				p2Jump->setID("p2-jump");
				p2Jump->setColor({127, 127, 127});
				keystrokes->addChild(p2Jump);

				auto p2Right = CCSprite::createWithSpriteFrameName("PBtn_Move_001.png");
				p2Right->setID("p2-right");
				p2Right->setColor({127, 127, 127});
				p2Right->setFlipX(true);
				keystrokes->addChild(p2Right);
			}
		} else {
			keystrokes->setContentWidth(55.f);

			auto p1Jump = CCSprite::createWithSpriteFrameName("PBtn_Jump_001.png");
			p1Jump->setID("p1-jump");
			p1Jump->setColor({127, 127, 127});
			keystrokes->addChild(p1Jump);

			if(level->m_twoPlayerMode) {
				auto p2Jump = CCSprite::createWithSpriteFrameName("PBtn_Jump_001.png");
				p2Jump->setID("p2-jump");
				p2Jump->setColor({127, 127, 127});
				keystrokes->addChild(p2Jump);
			}
		}

		keystrokes->setLayout(AxisLayout::create()->setGrowCrossAxis(true));
		keystrokes->updateLayout(false);

		keystrokes->setPosition(ccp(
			CCDirector::sharedDirector()->getWinSize().width - (keystrokes->getContentWidth() * Mod::get()->getSettingValue<double>("scale")) - 10,
			10
		));
		this->addChild(keystrokes, 50);

		return true;
	}
};