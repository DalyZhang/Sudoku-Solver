#include <windows.h>
class Timer {
public:
	enum Status {S_START, S_STOP, S_PAUSE};
private:
	static LARGE_INTEGER pFreq;
	static LARGE_INTEGER initializePFreq();
	Status status = S_STOP;
	LARGE_INTEGER startTime;
	double record = 0;
	double getTimeFromStart();
public:
	double start();
	double pause();
	double stop();
	double getTime();
	Status getStatus() const;
};

LARGE_INTEGER Timer::pFreq = initializePFreq();

LARGE_INTEGER Timer::initializePFreq() {
	LARGE_INTEGER frequent;
	QueryPerformanceFrequency(&frequent);
	return frequent;
}

double Timer::getTimeFromStart() {
	static LARGE_INTEGER cacheTime;
	QueryPerformanceCounter(&cacheTime);
	return (double)(cacheTime.QuadPart - startTime.QuadPart) / (double)pFreq.QuadPart;
}

double Timer::start() {
	if (status == S_STOP) {
		record = 0;
	}
	QueryPerformanceCounter(&startTime);
	status = S_START;
	return record * 1000;
}

double Timer::pause() {
	if (status != S_START) {
		return record * 1000;
	}
	record += getTimeFromStart();
	status = S_PAUSE;
	return record * 1000;
}

double Timer::stop() {
	if (status != S_START) {
		status = S_STOP;
		return record * 1000;
	}
	record += getTimeFromStart();
	status = S_STOP;
	return record * 1000;
}

double Timer::getTime() {
	if (status == S_START) {
		return getTimeFromStart() * 1000;
	} else {
		return record * 1000;
	}
}

Timer::Status Timer::getStatus() const {
	return status;
}