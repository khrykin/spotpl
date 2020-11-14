//
// Created by Dmitry Khrykin on 03.08.2020.
//

#include "errors.h"

error::generic::generic(std::string reason)
        : message(make_message(std::move(reason))) {}

auto error::generic::what() const noexcept -> const char * {
    return message.c_str();
}

auto error::generic::make_message(std::string reason) -> std::string {
    return "Error: " + std::move(reason);
}

error::parse::parse(std::string reason)
        : generic(std::move(reason) + ".\n\nRun spotpl -h for more info.") {}
