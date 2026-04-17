#include "yukigram/settings/hide_wall_paper.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideWallPaper = Yukigram::Options::make<bool>(kOptionHideWallPaper, {
	.keywords = { u"yukigram"_q, u"wallpaper"_q },
	.category = "style",
});

const char kOptionHideWallPaper[] = "hide-wall-paper";

}
