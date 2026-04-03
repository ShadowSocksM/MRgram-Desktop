#include "yukigram/settings/force_mobile_layout.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *ForceMobileLayout = Yukigram::Options::make<bool>(kOptionForceMobileLayout, {
	.keywords = { u"yukigram"_q, u"layout"_q, u"small"_q, u"phone"_q, u"mobile"_q },
	.category = "interface",
});

const char kOptionForceMobileLayout[] = "force-mobile-layout";

}
