#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t count = 0;
volatile sig_atomic_t quitflag = 0;

void signalINTreg(int sig) {
    (void)sig;
    write(1, "\a", 1);
    count++;
}

void signalOUTreg(int sig) {
    (void)sig;
    quitflag = 1;
}

int main(void) {
    sigset(SIGINT, signalINTreg);
    sigset(SIGQUIT, signalOUTreg);

    while (!quitflag)
        sleep(1);

    printf("\nNumber of beeps: %d\n", (int)count);
    return 0;
}