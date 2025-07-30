#define DUCKDB_EXTENSION_MAIN

#include "eats_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void EatsScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Eats " + name.GetString() + " 🐥");
	});
}

inline void EatsOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Eats " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(DatabaseInstance &instance) {
	// Register a scalar function
	auto eats_scalar_function = ScalarFunction("eats", {LogicalType::VARCHAR}, LogicalType::VARCHAR, EatsScalarFun);
	ExtensionUtil::RegisterFunction(instance, eats_scalar_function);

	// Register another scalar function
	auto eats_openssl_version_scalar_function = ScalarFunction("eats_openssl_version", {LogicalType::VARCHAR},
	                                                            LogicalType::VARCHAR, EatsOpenSSLVersionScalarFun);
	ExtensionUtil::RegisterFunction(instance, eats_openssl_version_scalar_function);
}

void EatsExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string EatsExtension::Name() {
	return "eats";
}

std::string EatsExtension::Version() const {
#ifdef EXT_VERSION_EATS
	return EXT_VERSION_EATS;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void eats_init(duckdb::DatabaseInstance &db) {
	duckdb::DuckDB db_wrapper(db);
	db_wrapper.LoadExtension<duckdb::EatsExtension>();
}

DUCKDB_EXTENSION_API const char *eats_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
