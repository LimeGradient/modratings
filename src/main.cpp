#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

EventListener<web::WebTask> m_webListener;

$execute {
	new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) {
		m_webListener.bind([] (web::WebTask::Event* e) {
			if (web::WebResponse* res = e->getValue()) {
                if (res->ok()) {
					auto json = res->json();
				}
            } else if (e->isCancelled()) {
                log::info("The request was cancelled... So sad :(");
            }
		});

		auto req = web::WebRequest();
		m_webListener.setFilter(req.get("https://raw.githubusercontent.com/LimeGradient/modratings/master/ratings.json"));

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