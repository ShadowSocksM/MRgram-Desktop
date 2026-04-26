#pragma once

#include "rpl/rpl.h"

#include <QStringList>
#include <string_view>

namespace Yukigram::Options {

template<typename T> using CheckValid = std::function<QString(T)>;

namespace Valid {
template<typename T> QString Always(T) {
	return {};
}
rpl::producer<QString> AlwaysHint();
CheckValid<int> Bounded(int lower, int upper);
std::function<rpl::producer<QString>()> BoundedHint(int lower, int upper);
} // namespace Valid

template<typename T> struct Description {
	QStringList keywords;
	std::string_view category;
	bool restartRequired = false;
	bool mirrorExperimental = false;
	T defaultValue;
	std::function<void(T)> link = [](T) {};
	CheckValid<T> checkValid = Valid::Always<T>;
	std::function<rpl::producer<QString>()> validHint = Valid::AlwaysHint;
};

template<typename T> rpl::variable<T> *make(const char *key, Description<T> &&d) {
	if constexpr (std::is_same_v<T, bool>) {
		return make_bool(key, std::move(d));
	} else if constexpr (std::is_same_v<T, int>) {
		return make_int(key, std::move(d));
	} else if constexpr (std::is_same_v<T, QString>) {
		return make_qs(key, std::move(d));
	}
}
rpl::variable<bool> *make_bool(const char *key, Description<bool> &&d);
rpl::variable<int> *make_int(const char *key, Description<int> &&d);
rpl::variable<QString> *make_qs(const char *key, Description<QString> &&d);

} // namepace Yukigram::Options
