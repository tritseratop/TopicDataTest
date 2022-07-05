#ifndef ABSTRACT_SUBSCRIBER_H_
#define ABSTRACT_SUBSCRIBER_H_

#include "Utilities/DdsCommonClasses.h"

class AbstractDdsSubscriber
{
public:
	virtual ~AbstractDdsSubscriber(){};
	virtual bool init() = 0;
	virtual void setConfig(const SubscriberConfig& config) = 0;
};

#endif //!ABSTRACT_SUBSCRIBER_H_