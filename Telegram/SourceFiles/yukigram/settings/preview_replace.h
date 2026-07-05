#include "rpl/rpl.h"
namespace Yukigram::Settings {
extern rpl::variable<bool> *PreviewReplace;
extern const char kOptionPreviewReplace[];
extern rpl::variable<QString> *PreviewReplacePatterns;
extern const char kOptionPreviewReplacePatterns[];
}

QString YukigramReplaceLink(QString link);
