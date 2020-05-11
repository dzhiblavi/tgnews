#include "sysapi.h"

namespace sysapi {
void kill(pid_t pid) {
    ::kill(pid, SIGKILL);
}

void chdir(std::filesystem::path const& path) {
    if (0 != ::chdir(path.c_str())) {
        throw error(std::strerror(errno));
    }
}

int wait() {
    int status;

    if (::wait(&status) == -1) {
        throw error(std::strerror(errno));
    }

    return WEXITSTATUS(status);
}

int wait(int pid) {
    int status;

    for (;;) {
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            // wait
        } else if (result == -1) {
            throw error(std::strerror(errno));
        } else {
            break;
        }
    }

    return WEXITSTATUS(status);
}
}
