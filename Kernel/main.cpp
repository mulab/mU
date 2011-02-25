#include <typeinfo>
#include <mU/Exceptions.h>
#include <mU/Kernel.h>
#include <mU/Parser.h>
using namespace mU;

inline void prompt() { wcout<<L"mU> "; }
inline void newline() { wcout<<L"  > "; }
int main(int argc,char *argv[])
{
	setlocale(LC_ALL,"");
	//wcerr.rdbuf(0);
	Initialize();
	ParseFile(Path() + L"mU.ini");
	if(argc > 1)
	{
		for(size_t i = 1; i < argc; ++i)
			ParseFile(wstring(argv[i],argv[i] + strlen(argv[i])));
		return 0;
	}
    wcin.imbue(std::locale(""));
    wcout.imbue(std::locale(""));
	wcout<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *    \n"<<
		std::setw(45)<<L"*         *   *\n"<<
		std::setw(45)<<L"* *     *  * * \n"<<
		std::setw(45)<<L"*   * *        \n"<<
		std::setw(45)<<L"*              \n";
	prompt();
	var r;
	parser p;
	wstring buf, line;
	while(std::getline(wcin,line))
	{
		if(line.empty())
		{
			buf.clear();
			prompt();
			continue;
		}
		buf += line;
		try
		{
			wistringstream stream(buf);
			p.start(stream);
			p.parse();
			r = Eval(Optimi(p.result()));
		}
		catch (std::bad_cast &)
		{
			wcout << L"wrong type of object encountered\n" << std::flush;
		}
		catch (UnexpectedTokenException &)
		{
			wcout << std::setw(p.column + 4) << L"^\n" << std::flush;
		}
		catch (MoreInputNeededException &)
		{
			newline();
			buf += L'\n';
			continue;
		}
		catch(std::exception &e)
		{
			stringstream w(e.what());
			string t;
			std::getline(w,t,':');
			wcerr
				<< L"main:"
				<< e.what()	<< L'='
				<< buf << std::endl;
			buf.clear();
			prompt();
			continue;
		}
		buf.clear();
		if(r && r != Null) Println(r);
		prompt();
	}
	return 0;
}
