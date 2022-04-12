#ifndef WS_COMMON_CLASSES_H_
#define WS_COMMON_CLASSES_H_

#include "WsDto.h"

class IServer
{
public:
	virtual bool sendData(WsDataDto::Wrapper data) = 0;
	//virtual bool sendAlarm(AlarmDto data) = 0;
};

#endif //!WS_COMMON_CLASSES_H_