#include "yukigram/settings/prevent_delete.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *PreventDelete = Yukigram::Options::make<bool>(kOptionPreventDelete, {
	.keywords = { u"yukigram"_q, u"delete"_q, u"history"_q, u"chat"_q, u"prevent"_q },
	.category = "interface",
	.restartRequired = true,
});

const char kOptionPreventDelete[] = "prevent-delete";

}
