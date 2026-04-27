#include "yukigram/settings/icon_theme_symbolic.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *IconThemeSymbolic = Yukigram::Options::make<bool>(kOptionIconThemeSymbolic, {
	.keywords = { u"yukigram"_q, u"tray"_q, u"icon"_q, u"monochrome"_q, u"symbolic"_q },
	.category = "system",
});

const char kOptionIconThemeSymbolic[] = "icon-theme-symbolic";

}
