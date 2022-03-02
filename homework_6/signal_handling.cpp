#include <iostream>
#include <signal.h>
#include <ucontext.h>

void signal_handler(int sig, siginfo_t * info, void *ucontext)
{
    std::cout << "PID: " << info->si_pid << std::endl;
    std::cout << "UID: " << info->si_uid << std::endl;
    ucontext_t* context = (ucontext_t*)ucontext;
    std::cout << "EIP: " << context->uc_mcontext.gregs[REG_RIP] << std::endl;
    std::cout << "EAX: " << context->uc_mcontext.gregs[REG_RAX] << std::endl;
    std::cout << "EBX: " << context->uc_mcontext.gregs[REG_RBX] << std::endl;
}

int main()
{
    while (true)
    {
        
    }
}