#ifndef ADAPTER_CONFIG_H_
#define ADAPTER_CONFIG_H_

namespace scada_ate
{
enum class CacheMode
{
	WHOLE_DATASET,
	UPDATES_ONLY,
	DIFFERENCE
};

enum class GatewayMode
{
	PERIODICALLY,
	ON_UPDATE
};

} // namespace scada_ate

#endif //!ADAPTER_CONFIG_H_