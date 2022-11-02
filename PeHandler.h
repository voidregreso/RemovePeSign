#ifndef _PEHANDLER_H_
#define _PEHANDLER_H_

extern "C" {
	BOOL IsInvalidPe(LPVOID pBase);

	LPVOID MapPeFile(LPCWSTR pwzFile, PLARGE_INTEGER lpFileSize);

	void UnmapPeFile(LPVOID pBase);

	BOOL GetPeSignOffsetAndSize(LPVOID pBase, LPDWORD lpOffset, LPDWORD lpSize);

	BOOL RemovePeSign(LPVOID pBase, PLARGE_INTEGER lpFileSize, LPCWSTR pwzNewFile);
}


#endif // _PEHANDLER_H_
