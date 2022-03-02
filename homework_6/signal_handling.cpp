#include <iostream>
#include <signal.h>
#include <ucontext.h>
#include <string.h>
#include <unistd.h>

// signal handler
void signal_handler(int sig, siginfo_t * info, void *ucontext)
{
    if (sig != SIGUSR1)
    {
        return;
    }
    std::cout << "PID: " << info->si_pid << std::endl;
    std::cout << "UID: " << info->si_uid << std::endl;
    
    // cast void* to ucontext_t*
    ucontext_t* context = (ucontext_t*)ucontext;
    
    // registers
    std::cout << "EIP: " << context->uc_mcontext.gregs[REG_RIP] << std::endl;
    std::cout << "EAX: " << context->uc_mcontext.gregs[REG_RAX] << std::endl;
    std::cout << "EBX: " << context->uc_mcontext.gregs[REG_RBX] << std::endl;
}

int main()
{
    // create sigaction object
    struct sigaction action;

    // fill struct memory with 0
    memset(&action, 0, sizeof(action));

    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = signal_handler;
    while (true)
    {
        sleep(10);
    }
    return 0;
}