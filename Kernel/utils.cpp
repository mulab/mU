#include <mU/utils.h>
#include <gmp.h>

#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable:4996)
#endif

namespace mU {
wstring date() {
	time_t t;
	struct tm *lt;
	time(&t);
	lt = localtime(&t);
	wostringstream oss;
	oss << std::setfill(_W('0'))
		<< std::setw(4) << lt->tm_year + 1900 << _W('-') 
		<< std::setw(2) << lt->tm_mon + 1 << _W('-')
		<< std::setw(2) << lt->tm_mday << _W(' ')
		<< std::setw(2) << lt->tm_hour << _W(':')
		<< std::setw(2) << lt->tm_min << _W(':')
		<< std::setw(2) << lt->tm_sec;
	return oss.str();
	
	/*wchar buf[1024];
	wcsftime(buf, 1024, _W("%04d-%02d-%02d %02d:%02d:%02d"), lt);
	return buf;
	*/
}
wstring path() {
#ifdef _WIN32
    wchar buf[0x100];
    memset(buf, 0, 0x100);
    GetModuleFileNameW(NULL, buf, 0x100);
    wcsrchr(buf, _W('\\'))[1] = 0;
    return buf;
#else
    char buf[0x100];
    memset(buf, 0, 0x100);
    readlink("/proc/self/exe", buf, 0x100);
    strrchr(buf, L'/')[1] = 0;
    return wstring(buf, buf + strlen(buf));
#endif
}
bool shell(wcs x) {
#ifdef _WIN32
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    // Start the child process.
    if (CreateProcessW(NULL,	// No module name (use command line)
                      const_cast<wchar*>(x),	// Command line
                      NULL,           // Process handle not inheritable
                      NULL,           // Thread handle not inheritable
                      FALSE,          // Set handle inheritance to FALSE
                      0,              // No creation flags
                      NULL,           // Use parent's environment block
                      NULL,           // Use parent's starting directory
                      &si,            // Pointer to STARTUPINFO structure
                      &pi )           // Pointer to PROCESS_INFORMATION structure
       ) {
        // Wait until child process exits.
        WaitForSingleObject( pi.hProcess, INFINITE );
        // Close process and thread handles.
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
        return true;
    }
    return false;
#else
    return system(string(x, x + wcslen(x)).c_str()) == 0;
#endif
}
wstring mbs2wcs(const char* x) {
	// wcstombs(r, x, strlen(x));
	/*wostringstream oss;
	oss << x;
	return oss.str();
	*/
	return wstring(x, x + strlen(x));
}
string wcs2mbs(wcs x) {
	// wcstombs(r, x, wcslen(x));
	/*
	ostringstream oss;
	oss << x;
	return oss.str();
	*/
	return string(x, x + wcslen(x));
}
void _fdebugf(FILE * fp, const char * funcname, const char * filename, const int line, char *fmt, ...) {
	char buf[1024];
	time_t t;
	struct tm * now;
	va_list ap;

	time(&t);
	now = localtime(&t);
	va_start(ap, fmt);
	fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] -- %s - %s(%d): DEBUG:@\"", 
		now -> tm_year + 1900, now -> tm_mon + 1, now -> tm_mday, now -> tm_hour, 
		now -> tm_min, now -> tm_sec, funcname, filename, line);
	gmp_vsprintf(buf, fmt, ap);
	fprintf(fp, "%s\"@\n", buf);
	va_end(ap);
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif