#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <iostream>

#include "../common/socket.h"
#include "State.h"
#include "StateManager.h"

static unsigned short PORT = 8888;

std::vector<std::string> splitMessage(const std::string& message)
{
    std::vector<std::string> parts;

    std::string prefix;
    for (size_t i = 0; i < message.size(); i++)
    {
        if (message[i] == ' ')
        {
            parts.push_back(prefix);
            prefix.clear();
        }
        else prefix += message[i];
    }

    parts.push_back(prefix);

    return parts;
}

int run_child(int argc, char** argv, unsigned short port)
{
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);

    std::vector<std::string> args, envp;

    args.push_back(std::string(argv[1]));
    envp.push_back("SE_PORT=" + std::to_string(port));

    if (argc > 3)
    {
        envp.push_back("LD_PRELOAD=" + std::string(argv[2]));
        envp.push_back("LD_LIBRARY_PATH=$LD_LIBRARY_PATH:" + std::string(argv[3]));
    }

    char* argsArray[16];
    for (size_t i = 0; i < args.size(); i++)
    {
        argsArray[i] = const_cast<char*>(args[i].c_str());
    }
    argsArray[args.size()] = nullptr;

    char* envpArray[16];
    for (size_t i = 0; i < envp.size(); i++)
    {
        envpArray[i] = const_cast<char*>(envp[i].c_str());
    }
    envpArray[envp.size()] = nullptr;

    return execvpe(argv[1], argsArray, envpArray);
}

int main(int argc, char** argv)
{
    pid_t pid = fork();
    if (!pid) // child
    {
        exit(run_child(argc, argv, PORT));
    }

    int status;
    StateManager stateManager;

    Socket server;
    server.bind(PORT);

    // initial SIGTRAP
    waitpid(pid, &status, 0);
    ptrace(PTRACE_CONT, pid, nullptr, nullptr);

    std::cerr << "Rewinder: child started" << std::endl;

    std::unique_ptr<Socket> client = server.accept();
    std::cerr << "Rewinder: child connected" << std::endl;

    std::string mmapArea = client->readline();
    auto parts = splitMessage(mmapArea);
    size_t start = std::stoul(parts[1], nullptr, 10);
    size_t length = std::stoul(parts[2], nullptr, 10);
    stateManager.setGuardedArea(start, length);

    std::cerr << "Rewinder: child mmap area: " << (void*) start << ", " << length << " bytes" << std::endl;

    std::unique_ptr<State> state;

    // second stop
    while (true)
    {
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGUSR1)
        {
            std::cerr << "Rewinder: saving child" << std::endl;
            state = stateManager.save(pid);
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        }
        else if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGUSR2)
        {
            std::cerr << "Rewinder: restoring child" << std::endl;
            stateManager.restore(pid, state.get());
            ptrace(PTRACE_CONT, pid, nullptr, nullptr);
        }
        else if (WIFEXITED(status))
        {
            std::cerr << "Rewinder: child exited with status " << WEXITSTATUS(status) << std::endl;
            break;
        }
        else
        {
            std::cerr << "Rewinder: child stopped with signal " << WSTOPSIG(status) << std::endl;
        }
    }

    return 0;
}
