#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>

using namespace geode::prelude;

class ModRating {
public:
	std::string modID;
	int rating;

	ModRating(std::string id, int rate) {
		this->modID = id;
		this->rating = rate;
	}
	static std::string getRatingFromEnum(int rating) {
		switch (rating) {
			case 1:
				return "GJ_epicCoin1_001.png";
			case 2:
				return "GJ_epicCoin2_001.png";
			case 3:
				return "GJ_epicCoin3_001.png";
		}
	}
};

EventListener<web::WebTask> m_webListener;
std::vector<ModRating*> modsList;

$execute {
	m_webListener.bind([] (web::WebTask::Event* e) {
		if (web::WebResponse* res = e->getValue()) {
			if (res->ok()) {
				auto json = res->json().unwrap();
				auto mods = json["mods"].as_array();
				for (auto mod : mods) {
					ModRating* modRating = new ModRating(mod["id"].as_string(), mod["rating"].as_int());
					log::info("found mod. id: {}, rating: {}", modRating->modID, mod["rating"].as_int());
					modsList.push_back(modRating);
				}
			}
		} else if (e->isCancelled()) {
			log::info("The request was cancelled... So sad :(");
		}
	});

	auto req = web::WebRequest();
	m_webListener.setFilter(req.get("https://raw.githubusercontent.com/LimeGradient/modratings/master/ratings.json"));

	new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) {
		for (auto mod : modsList) {
			if (event->getModID() == mod->modID) {
				if (auto rating = CCSprite::createWithSpriteFrameName(ModRating::getRatingFromEnum(mod->rating).c_str())) {
					rating->setScaleX(1.75f);
					rating->setScaleY(1.75f);
					rating->setAnchorPoint({0.5f, 0.6f});
					rating->setZOrder(event->getSprite()->getZOrder() - 1);
					// `event->getSprite()` is guaranteed to not be `nullptr` though
					event->getSprite()->addChildAtPosition(rating, Anchor::Center);
				}
			}
		}
        // You should always propagate Geode UI events
        return ListenerResult::Propagate;
    });
	new EventListener<EventFilter<ModPopupUIEvent>>(+[](ModPopupUIEvent* event) {
		if (event->getPopup())
	})
}