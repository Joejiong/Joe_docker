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


#include <sys/wait.h>        // waitpid
#include <sys/mount.h>       // mount 
#include <fcntl.h>           // open
#include <unistd.h>          // execv, sethostname, chroot, fchdir
#include <sched.h>           // clone

#include <iostream>
#include <string>

// C standard library
#include <cstring>

#define STACK_SIZE (1024 * 1024) // define the size of child proc space as 1 M

namespace joe_docker {
    typedef struct docker_info {
        std::string hostname;
        std::string root_dir;
    } docker_info;

    typedef int process_pid;
    
    class jocker {
    private:
        docker_info info;                           // container info
        char child_stack[STACK_SIZE];               // child proc stack size

        void set_hostname() {                       // set hostname
            sethostname(
                this->info.hostname.c_str(),
                this->info.hostname.size()
            );
        }
        void set_rootdir() {                        // set root_directory
            chdir(this->info.root_dir.c_str());
            chroot(".");
        }
        void set_procsys() {                        // mount proc fs system
            mount("none", "/proc", "proc", 0, nullptr);
            mount("none", "/sys", "sysfs", 0, nullptr);
        }
        void start_bash() {
            std::string bash = "/bin/bash";
            char* c_bash = new char[bash.size()+1];

            strcpy(c_bash, bash.c_str());
            char* const child_args[] = {c_bash, NULL};
            execv(child_args[0], child_args);
            delete []c_bash;
            c_bash = nullptr;
        }
    public:
        jocker(docker_info &d_info):info(d_info){}  //constructor
        
        static int set_info(void *args) {
            auto j_docker  = static_cast<jocker*>(args);
            j_docker->set_hostname();
            j_docker->set_rootdir();
            j_docker->set_procsys();
            j_docker->start_bash();

            return 1;
        }
        void start() {
            std::cout<<"welcome to jocker world new docker!!! ok ok!!!"<<std::endl;
            process_pid child_proc = clone(set_info,                
                                           child_stack+STACK_SIZE,  // each container is a process
                                           CLONE_NEWUTS |           // uts field virtualization
                                           CLONE_NEWNS  |           // Mounting namespace
                                           CLONE_NEWIPC |           // proc communication 
                                           CLONE_NEWPID |
                                           SIGCHLD,                 // send a sig to the parent proc when child_proc exit
                                           this);                   // trans this ptr to the static func
            waitpid(child_proc, nullptr, 0);                        // wait for child proc exit avoid from Z-proc
            printf("exit from jocker\n");
        }

    };

} // end of namespace jocker