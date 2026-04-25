#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) {
            // Count spaces to estimate number of arguments (plus one for the command itself)
            int space_count = 0;
            for (char c : cmd) {
                if (c == ' ') space_count++;
            }
            
            // Allocate memory for argv array
            _argc = space_count + 1;
            _argv = new char*[_argc];
            
            // Parse the command string
            int arg_index = 0;
            int start = 0;
            int len = cmd.length();
            
            for (int i = 0; i <= len; i++) {
                if (i == len || cmd[i] == ' ') {
                    // Found end of an argument
                    if (i > start) {  // Make sure we have a non-empty argument
                        int arg_len = i - start;
                        _argv[arg_index] = new char[arg_len + 1];
                        strncpy(_argv[arg_index], cmd.c_str() + start, arg_len);
                        _argv[arg_index][arg_len] = '\0';
                        arg_index++;
                    }
                    start = i + 1;  // Skip the space
                }
            }
            
            // In case there are extra slots due to consecutive spaces
            _argc = arg_index;
        }
        ~arguments() {
            // Free memory for each string
            for (int i = 0; i < _argc; i++) {
                delete[] _argv[i];
            }
            // Free the array of pointers
            delete[] _argv;
        }
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}