#pragma once

#include "yukigram/options.h"
#include "base/options.h"

#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace Yukigram::Options {

template<typename T> struct Option {
	std::string_view key;
	Description<T> d;
	std::shared_ptr<rpl::variable<T>> v;
	std::optional<std::shared_ptr<base::options::option<T>>> _base;
	rpl::lifetime _lifetime;

	void init() {
		auto &inner = base::options::lookup<T>(std::string(key).c_str());
		if (d.mirrorExperimental) {
			d.defaultValue = inner.defaultValue();
		}
		const auto value = inner.value();
		if (const auto reason = d.checkValid(value); !reason.isEmpty()) {
			LOG(("Yukigram::Options::Option: option %1: value %2: %3. Resetting to default %4"
				).arg(QString::fromUtf8(key.data(), int(key.size()))).arg(value).arg(reason).arg(d.defaultValue));
			*v = d.defaultValue;
		} else {
			*v = value;
		}
	}
	void link() {
		auto &inner = base::options::lookup<T>(std::string(key).c_str());
		inner.set(v->current());
		v->value() | rpl::on_next([=, &inner, link = std::move(d.link)](T value) {
			inner.set(value);
			link(value);
		}, _lifetime);
	}
};

std::map<std::string_view, std::variant<Option<bool>, Option<int>, Option<QString>>> &Options();
std::map<std::string_view, std::vector<std::string_view>> &ByCategory();

}


