#ifndef ABSTRACT_SUBSCRIBER_H_
#define ABSTRACT_SUBSCRIBER_H_

#include "Lib/DdsService/Config/Configure.h"

namespace scada_ate::dds::subscriber
{
class AbstractSubscriber
{
public:
	virtual ~AbstractSubscriber(){};
	virtual bool init() = 0;
	virtual void setConfig(const Configure& config) = 0;
};
} // namespace scada_ate::dds::subscriber

#endif //!ABSTRACT_SUBSCRIBER_H_