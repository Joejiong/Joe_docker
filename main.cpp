/* Copyright (c) 2020 the Author joejiong. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "jocker.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "start jocker yeah~" << std::endl;
    joe_docker::docker_info jocker_info;
    jocker_info.hostname = "MYDOCKER";
    jocker_info.root_dir = "./joejiong";
    joe_docker::jocker my_jocker(jocker_info);
    my_jocker.start();
    std::cout<< "stop jocker..." << std::endl;
    return 0;
}