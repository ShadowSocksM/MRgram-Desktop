#include "yukigram/settings/no_shout_buttons.h"
#include "yukigram/options.h"

namespace Ui {
extern bool Yukigram_NoShoutButtons;
}

namespace Yukigram::Settings {

rpl::variable<bool> *NoShoutButtons = Yukigram::Options::make<bool>(kOptionNoShoutButtons, {
	.keywords = { u"yukigram"_q, u"shout"_q, u"caps"_q, u"button"_q },
	.category = "interface",
	.link = [](bool value) { Ui::Yukigram_NoShoutButtons = value; },
});

const char kOptionNoShoutButtons[] = "no-shout-buttons";

}
