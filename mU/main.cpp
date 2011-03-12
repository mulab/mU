#include <mU/Interface.h>
#include <mU/Parser.h>
#include <mU/utils.h>
#include <cstring>
#ifdef _MSC_VER
#pragma comment(lib,"Kernel.lib")
#pragma comment(lib,"Parser.lib")
#endif

using namespace mU;

inline void prompt() {
    wcout <<
#ifdef _WIN32
#ifdef _MSC_VER
    (wchar)956 <<
#else
    _W("mU") <<
#endif
#else
    _W("mu") <<
#endif
    _W("> ");
}
inline void newline() {
    wcout << _W("  > ");
}
inline void open(Parser& p, wifstream& f, const char* s) {
	f.open(s);
	if (!f)
		return;
	try {
		p.start(f);
		p.eval();
	} catch (std::exception& e) {
		wcerr << _W("Error occurred while eval ") << s << _W(", ")
			<< e.what() << _W("...") << endl;
	}
	f.close();
}
int main(int argc, char *argv[]) {
	// FIXME: .u脚本中文路径名处理有问题
#ifdef _MSC_VER
	try {
		setlocale(LC_ALL, "");
		wcin.imbue(std::locale(""));
		wcout.imbue(std::locale(""));
	} catch (std::exception& e) {
		wcerr << _W("Error occurred while set locale, ") << e.what() << _W("...") << endl;
	}
#endif
	// wcerr.rdbuf(0);

	Kernel k;
	if (!cinstall(k, "system")) {
    	wcerr << _W("System library not found, quit...") << endl;
        return -1;
    }

	Parser p(k);
	wifstream f;
	open(p, f, wcs2mbs(path() + _W("mU.ini")).c_str());
    open(p, f, "mU.ini");
    if (argc > 1) {
        for (int i = 1; i < argc; ++i)
            open(p, f, argv[i]);
        return 0;
    }
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
        try {
            r = k.eval(p.code());
            if (r) {
                println(k, r);
                r = null;
            }
        } catch (std::exception& e) {
            wcerr << _W("Error occurred while eval ") << buf << _W(", ")
				<< e.what() << _W("...") << endl;
			k.start();
        }
		buf.clear();
        prompt();
    }
    return 0;
}
