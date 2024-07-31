#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

$execute {
	new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) {
        if (event->getModID() == "geode.loader") {
            // Remember: no assumptions, even trivial ones!
            if (auto rating = CCSprite::createWithSpriteFrameName("GJ_epicCoin3_001.png")) {
                rating->setScaleX(1.75f);
                rating->setScaleY(1.75f);
				rating->setAnchorPoint({0.5f, 0.6f});
				rating->setZOrder(event->getSprite()->getZOrder() - 1);
                // `event->getSprite()` is guaranteed to not be `nullptr` though
                event->getSprite()->addChildAtPosition(rating, Anchor::Center);
            }
        }
        // You should always propagate Geode UI events
        return ListenerResult::Propagate;
    });
}