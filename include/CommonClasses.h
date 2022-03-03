#ifndef COMMON_CLASSES_H_
#define COMMON_CLASSES_H_

struct DataDto
{
	uint64_t time_service;
};

struct AlarmDto
{
	uint64_t time_service;
};

class IServer
{
public:
	virtual bool sendData(DataDto data) = 0;
	virtual bool sendAlarm(AlarmDto data) = 0;
};

#endif //!COMMON_CLASSES_H_