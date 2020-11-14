//
// Created by Dmitry Khrykin on 03.08.2020.
//

#ifndef SPOTPL_ERRORS_H
#define SPOTPL_ERRORS_H


#include <iostream>
#include <string>
#include <exception>

namespace error {

    inline void print(const std::exception &e) {
        std::cerr << "\n" << e.what() << "\n\n";
    }

    class generic : public std::exception {
    public:
        explicit generic(std::string reason);

        [[nodiscard]] auto what() const noexcept -> const char * override;

    private:
        static auto make_message(std::string reason) -> std::string;

        std::string message;
    };

    class parse : public generic {
    public:
        explicit parse(std::string reason);
    };

}


#endif //SPOTPL_ERRORS_H
