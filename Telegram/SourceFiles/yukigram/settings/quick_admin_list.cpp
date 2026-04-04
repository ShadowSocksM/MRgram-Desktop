#include "yukigram/settings/quick_admin_list.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *QuickAdminList = Yukigram::Options::make<bool>(kOptionQuickAdminList, {
	.keywords = { u"yukigram"_q, u"admin"_q, u"list"_q },
	.category = "chat",
});

const char kOptionQuickAdminList[] = "quick-admin-list";

}
