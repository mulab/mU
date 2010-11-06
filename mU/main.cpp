#include <mU/Kernel.h>
#include <mU/Parser.h>
#include <mU/Interface.h>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#pragma comment(lib,"Parser.lib")
#pragma comment(lib,"Interface.lib")
#endif

using namespace mU;

inline void prompt() {
    wcout << (wchar)956 << _W("> ");
}
inline void newline() {
    wcout << _W("  > ");
}
int main(int argc, char *argv[]) {
    Kernel k;
    Parser p(k);
    if (!cinstall(k, "system")) {
    	wcerr << _W("system library not found, quit...") << endl;
        return -1;
    }
    wifstream f;
    setlocale(LC_ALL, "");
    //wcerr.rdbuf(0);
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            f.open(argv[i]);
            if (f) {
                p.start(f);
                p.eval();
                f.close();
            }
        }
        return 0;
    }
    f.open("mU.ini");
    if (f) {
        p.start(f);
        p.eval();
        f.close();
    }
    wcin.imbue(std::locale(""));
    wcout.imbue(std::locale(""));
    wcout <<
    std::setw(45) << _W("*         *    \n") <<
    std::setw(45) << _W("*         *    \n") <<
    std::setw(45) << _W("*         *    \n") <<
    std::setw(45) << _W("*         *    \n") <<
    std::setw(45) << _W("*         *    \n") <<
    std::setw(45) << _W("*         *   *\n") <<
    std::setw(45) << _W("* *     *  * * \n") <<
    std::setw(45) << _W("*   * *        \n") <<
    std::setw(45) << _W("*              \n");
    prompt();
    var r;
    wstring buf, line;
    while (std::getline(wcin, line)) {
        if (line.empty()) {
            buf.clear();
            prompt();
            continue;
        }
		if (line == _W("Exit"))
			break;
        buf += line;
        try {
            wistringstream stream(buf);
            p.start(stream);
            p.gen(p.parse());
        } catch (std::exception& e) {
            if (!strcmp(e.what(), "more")) {
                newline();
                buf += _W('\n');
                continue;
            } else if (!strcmp(e.what(), "error"))
                wcout << std::setw(p.column + 4) << _W("^\n");
            buf.clear();
            prompt();
            continue;
        }
        buf.clear();
        try {
            r = k.eval(p.code());
            if (r) {
                println(k, r);
                r = null;
            }
        } catch (std::exception& e) {
            wcerr
            << _W("Eval:")
            << e.what()
            << endl;
			k.start();
        }
        prompt();
    }
    return 0;
}
