#ifndef WS_DTO_H_
#define WS_DTO_H_

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class WsDataCollectionInt : public oatpp::DTO {

	DTO_INIT(WsDataCollectionInt, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, time_source);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, id_tag);
	DTO_FIELD(oatpp::Vector<oatpp::Int32>, value);
	DTO_FIELD(oatpp::String, quality);

};

class WsDataCollectionFloat : public oatpp::DTO {

	DTO_INIT(WsDataCollectionFloat, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, time_source);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, id_tag);
	DTO_FIELD(oatpp::Vector<oatpp::Float32>, value);
	DTO_FIELD(oatpp::String, quality);

};

class WsDataCollectionDouble : public oatpp::DTO {

	DTO_INIT(WsDataCollectionDouble, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, time_source);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, id_tag);
	DTO_FIELD(oatpp::Vector<oatpp::Float64>, value);
	DTO_FIELD(oatpp::String, quality);

};

class WsDataCollectionChar : public oatpp::DTO {

	DTO_INIT(WsDataCollectionChar, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, time_source);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, id_tag);
	DTO_FIELD(oatpp::Vector<oatpp::String>, value);
	DTO_FIELD(oatpp::String, quality);

};

class WsDataDto : public oatpp::DTO {

	DTO_INIT(WsDataDto, DTO)

	DTO_FIELD(oatpp::Int64, time_service);
	DTO_FIELD(oatpp::Object<WsDataCollectionInt>, data_collection_int);
	DTO_FIELD(oatpp::Object<WsDataCollectionFloat>, data_collection_float);
	DTO_FIELD(oatpp::Object<WsDataCollectionDouble>, data_collection_double);
	DTO_FIELD(oatpp::Object<WsDataCollectionChar>, data_collection_char);

};

#include OATPP_CODEGEN_END(DTO) //!End DTO codegen section

#endif //!WS_DTO_H_