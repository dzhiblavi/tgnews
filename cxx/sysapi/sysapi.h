#ifndef TGNEWS_SYSAPI
#define TGNEWS_SYSAPI

#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <functional>
#include <csignal>
#include <cerrno>
#include <filesystem>
#include <sys/stat.h>
#include <cstring>


namespace sysapi {
struct error : std::runtime_error {
    using std::runtime_error::runtime_error;
};

void kill(pid_t pid, int sig = SIGKILL);

int wait(int pid);

void chdir(std::filesystem::path const& path);

template <typename F>
int wait(int pid, F&& pred) {
    int status;

    for (;;) {
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            if (pred()) {
                kill(pid);
                break;
            }
        } else if (result == -1) {
            throw error(std::strerror(errno));
        } else {
            break;
        }
    }

    return WEXITSTATUS(status);
}

template <typename... Args>
void executer(Args&&... args) {
    if (0 > execl(std::forward<Args>(args).c_str()..., nullptr)) {
        throw sysapi::error(std::strerror(errno));
    }
}

template <typename... Args>
int execute(Args&&... args) {
    int pid = fork();

    if (!pid) {
        executer(std::forward<Args>(args)...);
        return 0;
    } else {
        return pid;
    }
}

template<typename... Args>
int executew(Args&&... args) {
    return sysapi::wait(execute(std::forward<Args>(args)...));
}

template<typename B, typename... Args>
int executewp(B &&b, Args&&... args) {
    return sysapi::wait(execute(std::forward<Args>(args)...), std::forward<B>(b));
}
} // namespace sysapi


#endif // TGNEWS_SYSAPI