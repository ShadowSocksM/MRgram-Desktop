#include "yukigram/settings/quick_recent_actions.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *QuickRecentActions = Yukigram::Options::make<bool>(kOptionQuickRecentActions, {
	.keywords = { u"yukigram"_q, u"recent"_q, u"actions"_q },
	.category = "chat",
});

const char kOptionQuickRecentActions[] = "quick-recent-actions";

}
