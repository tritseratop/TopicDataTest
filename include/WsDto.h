#ifndef WS_DTO_H_
#define WS_DTO_H_

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class WsDataCollectionInt : public oatpp::DTO {

	DTO_INIT(WsDataCollectionInt, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, tsrc);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, tag);
	DTO_FIELD(oatpp::Vector<oatpp::Int32>, val);
	DTO_FIELD(oatpp::String, qlt);

};

class WsDataCollectionFloat : public oatpp::DTO {

	DTO_INIT(WsDataCollectionFloat, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, tsrc);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, tag);
	DTO_FIELD(oatpp::Vector<oatpp::Float32>, val);
	DTO_FIELD(oatpp::String, qlt);

};

class WsDataCollectionDouble : public oatpp::DTO {

	DTO_INIT(WsDataCollectionDouble, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, tsrc);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, tag);
	DTO_FIELD(oatpp::Vector<oatpp::Float64>, val);
	DTO_FIELD(oatpp::String, qlt);

};

class WsDataCollectionChar : public oatpp::DTO {

	DTO_INIT(WsDataCollectionChar, DTO)

	DTO_FIELD(oatpp::Vector<oatpp::Int64>, tsrc);
	DTO_FIELD(oatpp::Vector<oatpp::UInt32>, tag);
	DTO_FIELD(oatpp::Vector<oatpp::String>, val);
	DTO_FIELD(oatpp::String, qlt);

};

class WsDataDto : public oatpp::DTO {

	DTO_INIT(WsDataDto, DTO)

	DTO_FIELD(oatpp::Int64, tsrv);
	DTO_FIELD(oatpp::Int64, disp);
	DTO_FIELD(oatpp::Object<WsDataCollectionInt>, di);
	DTO_FIELD(oatpp::Object<WsDataCollectionFloat>, df);
	DTO_FIELD(oatpp::Object<WsDataCollectionDouble>, dd);
	DTO_FIELD(oatpp::Object<WsDataCollectionChar>, dc);

};

#include OATPP_CODEGEN_END(DTO) //!End DTO codegen section

#endif //!WS_DTO_H_