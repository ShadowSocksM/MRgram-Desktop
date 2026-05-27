#include "yukigram/settings/old_style_spoilers.h"
#include "yukigram/options.h"

namespace Ui {
extern bool Yukigram_OldStyleSpoilers;
}

namespace Yukigram::Settings {

rpl::variable<bool> *OldStyleSpoilers = Yukigram::Options::make<bool>(kOptionOldStyleSpoilers, {
	.keywords = { u"yukigram"_q, u"spoilers"_q, u"mess"_q },
	.category = "style",
	.link = [](bool value) { Ui::Yukigram_OldStyleSpoilers = value; },
});

const char kOptionOldStyleSpoilers[] = "old-style-spoilers";

}
