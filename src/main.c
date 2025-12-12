//
// Created by dingjing on 2025/12/10.
//

#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "wm/wm.h"
#include "global.h"
#include "common/log.h"


static void setup (void);
static void cleanup (void);
static void print_help (void);
static void print_version (void);

int main (int argc, char *argv[])
{
    int c = 0;

    while ((c = getopt(argc, argv, "c:vh")) != -1) {
        if ('c' == c) {
            gConfigFilePath = optarg;
        }
        else if ('h' == c) {
            print_help();
            exit(0);
        }
        else if ('v' == c) {
            print_version();
            exit(0);
        }
        else {
            print_help();
            exit(-1);
        }
    }

    if (!getenv("XDG_RUNTIME_DIR")) {
        DDIE("XDG_RUNTIME_DIR must be set");
    }

    init_global_config();

    setup();
    cleanup();

    return 0;
}

static void print_help (void)
{
    printf("Usage: graceful [option]\n"
           "options:\n"
           "    -h                  Print this help information\n"
           "    -v                  Print version information\n"
           "    -c <file path>      Use the configuration file of settings\n");
}

static void print_version (void)
{
    printf("graceful Version: %s\n", PACKAGE_VERSION);
}

void sig_handler(int sigNo)
{
    if (sigNo == SIGCHLD) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
    }
    else if (sigNo == SIGINT || sigNo == SIGTERM) {
        wm_quit();
    }
}

static void setup (void)
{
    int i, sig[] = {SIGCHLD, SIGINT, SIGTERM, SIGTSTP};
    struct sigaction sa = {.sa_flags = SA_RESTART, .sa_handler = sig_handler};

    setenv("XCURSOR_SIZE", "24", 1);
    setenv("XDG_CURRENT_DESKTOP", "graceful", 1);

    sigemptyset(&sa.sa_mask);
    for (i = 0; i < (int) C_N_ELEMENTS(sig); i++) {
        sigaction(sig[i], &sa, NULL);
    }

    // 初始化日志
    wm_log_init();



}

static void cleanup (void)
{

}