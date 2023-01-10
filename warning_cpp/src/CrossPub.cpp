// Copyright 2022, 2023 Czech Technical University in Prague
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file CrossingInfoPublisher.cpp
 *
 */

#include "common.hpp"
#include <argparse/argparse.hpp>
#include "CrossingInfoPublisher.hpp"


int main(int argc, char *argv[]) {

    argparse::ArgumentParser program("CrossPub");

    addCommonDdsArguments(program);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    CrossingInfoPublisher* publisher =
        new CrossingInfoPublisher(program.is_used("--server"),
                                  parseIP(program.get("--server")));

    std::cout << "x: crossing" << std::endl << "c: clear" << std::endl << "d: danger" << std::endl << "q: quit" << std::endl;
    char c;
    while (std::cin >> c) {
        if (c == 'C' || c == 'c') {
            publisher->publish(false, false);
        } else if (c == 'X' || c == 'x') {
            publisher->publish(false, true);
        } else if (c == 'D' || c == 'd') {
            publisher->publish(true, true);
        } else if (c == 'Q' || c == 'q') {
            break;
        }
    }

    delete publisher;
    return 0;

}
