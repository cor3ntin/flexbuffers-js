#include <emscripten/bind.h>
#include <flatbuffers/flexbuffers.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(flexbuffers)
{
    enum_<flexbuffers::Type>("Type")
        .value("NULL", flexbuffers::FBT_NULL)
        .value("INT", flexbuffers::FBT_INT)
        .value("UINT", flexbuffers::FBT_UINT)
        .value("FLOAT", flexbuffers::FBT_FLOAT)
        .value("KEY", flexbuffers::FBT_KEY)
        .value("STRING", flexbuffers::FBT_STRING)
        .value("INDIRECT_INT", flexbuffers::FBT_INDIRECT_INT)
        .value("INDIRECT_UINT", flexbuffers::FBT_INDIRECT_UINT)
        .value("INDIRECT_FLOAT", flexbuffers::FBT_INDIRECT_FLOAT)
        .value("MAP", flexbuffers::FBT_MAP)
        .value("VECTOR", flexbuffers::FBT_VECTOR)
        .value("VECTOR_INT", flexbuffers::FBT_VECTOR_INT)
        .value("VECTOR_UINT", flexbuffers::FBT_VECTOR_UINT)
        .value("VECTOR_FLOAT", flexbuffers::FBT_VECTOR_FLOAT)
        .value("VECTOR_KEY", flexbuffers::FBT_VECTOR_KEY)
        .value("VECTOR_STRING", flexbuffers::FBT_VECTOR_STRING)
        .value("VECTOR_INT2", flexbuffers::FBT_VECTOR_INT2)
        .value("VECTOR_UINT2", flexbuffers::FBT_VECTOR_UINT2)
        .value("VECTOR_FLOAT2", flexbuffers::FBT_VECTOR_FLOAT2)
        .value("VECTOR_INT3", flexbuffers::FBT_VECTOR_INT3)
        .value("VECTOR_UINT3", flexbuffers::FBT_VECTOR_UINT3)
        .value("VECTOR_FLOAT3", flexbuffers::FBT_VECTOR_FLOAT3)
        .value("VECTOR_INT4", flexbuffers::FBT_VECTOR_INT4)
        .value("VECTOR_UINT4", flexbuffers::FBT_VECTOR_UINT4)
        .value("VECTOR_FLOAT4", flexbuffers::FBT_VECTOR_FLOAT4)
        .value("BLOB", flexbuffers::FBT_BLOB)
        .value("BOOL", flexbuffers::FBT_BOOL)
        .value("VECTOR_BOOL", flexbuffers::FBT_VECTOR_BOOL);

    function("getRoot",
        optional_override([](unsigned char* data, size_t size) {
            return flexbuffers::GetRoot(data, size);
        }),
        allow_raw_pointers());

    class_<flexbuffers::Reference>("Reference")
        .property("type", &flexbuffers::Reference::GetType)
        .function("isNull", &flexbuffers::Reference::IsNull)
        .function("isBool", &flexbuffers::Reference::IsBool)
        .function("isNumeric", &flexbuffers::Reference::IsNumeric)
        .function("isString", &flexbuffers::Reference::IsString)
        .function("isKey", &flexbuffers::Reference::IsKey)
        .function("isVector", &flexbuffers::Reference::IsVector)
        .function("isMap", &flexbuffers::Reference::IsMap)
        .function("isBlob", &flexbuffers::Reference::IsBlob)
        .function("asNumber", &flexbuffers::Reference::AsDouble)
        .function("asBool", &flexbuffers::Reference::AsBool)
        .function("asKey", optional_override([](const flexbuffers::Reference& ref) {
            return std::string(ref.AsKey());
        }))
        .function("asVector", &flexbuffers::Reference::AsVector)
        .function("asBlob",
            optional_override([](const flexbuffers::Reference& ref) {
                const auto blob = ref.AsBlob();
                return val(typed_memory_view(blob.size(), blob.data()));
            }))

        .function("asTypedVector", &flexbuffers::Reference::AsTypedVector)
        .function("asFixedTypedVector", &flexbuffers::Reference::AsFixedTypedVector)
        .function("asMap", &flexbuffers::Reference::AsMap)
        .function(
            "toString",
            select_overload<std::string()>(&flexbuffers::Reference::ToString));

    class_<flexbuffers::Sized>("Sized").property("size",
        &flexbuffers::Sized::size);

    class_<flexbuffers::Vector, base<flexbuffers::Sized>>("Vector").function(
        "get", &flexbuffers::Vector::operator[]);

    class_<flexbuffers::TypedVector, base<flexbuffers::Sized>>("TypedVector")
        .function("type", &flexbuffers::TypedVector::ElementType)
        .function("get", &flexbuffers::TypedVector::operator[]);

    class_<flexbuffers::FixedTypedVector>("FixedTypedVector")
        .function("size", &flexbuffers::FixedTypedVector::size)
        .function("type", &flexbuffers::FixedTypedVector::ElementType)
        .function("get", &flexbuffers::FixedTypedVector::operator[]);

    class_<flexbuffers::Map>("Map")
        .property("keys", &flexbuffers::Map::Keys)
        .property("values", &flexbuffers::Map::Values)
        .function("get",
            select_overload<flexbuffers::Reference(const std::string&) const>(
                &flexbuffers::Map::operator[]));

    class_<flexbuffers::Builder>("Builder")
        .constructor()
        .property("size", &flexbuffers::Builder::GetSize)
        .property("buffer", optional_override([](const flexbuffers::Builder& b) {
            const auto& buffer = b.GetBuffer();
            return val(typed_memory_view(buffer.size(), buffer.data()));
        }))
        .function("clear", select_overload<void()>(&flexbuffers::Builder::Clear))
        .function("finish", &flexbuffers::Builder::Finish)

        .function("addNull",
            optional_override([](flexbuffers::Builder& b) { b.Null(); }))
        .function(
            "addNull",
            optional_override([](flexbuffers::Builder& b, const std::string& s) {
                b.Null(s.c_str());
            }))

        .function(
            "addInt",
            optional_override([](flexbuffers::Builder& b, int64_t i) { b.Int(i); }))
        .function(
            "addInt",
            optional_override([](flexbuffers::Builder& b, const std::string& s, int64_t i) { b.Int(s.c_str(), i); }))

        .function("addUint", optional_override([](flexbuffers::Builder& b, uint64_t i) { b.UInt(i); }))
        .function("addUint",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, uint64_t i) { b.UInt(s.c_str(), i); }))

        .function("addFloat", optional_override([](flexbuffers::Builder& b, float f) { b.Float(f); }))
        .function("addFloat",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, float f) { b.Float(s.c_str(), f); }))

        .function("addDouble", optional_override([](flexbuffers::Builder& b, float d) { b.Double(d); }))
        .function("addDouble",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, double d) { b.Double(s.c_str(), d); }))

        .function("addBool", optional_override([](flexbuffers::Builder& b, bool d) { b.Bool(d); }))
        .function("addBool",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, bool d) { b.Bool(s.c_str(), d); }))

        .function("addString", select_overload<size_t(const std::string&)>(&flexbuffers::Builder::String))
        .function("addString",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, const std::string& v) { return b.String(s.c_str(), v); }))

        .function("addKey", select_overload<size_t(const std::string&)>(&flexbuffers::Builder::Key))

        .function("startVector", select_overload<size_t()>(&flexbuffers::Builder::StartVector))
        .function("startVector",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, bool d) { return b.StartVector(s.c_str()); }))
        .function("endVector", &flexbuffers::Builder::EndVector)

        .function("startMap", select_overload<size_t()>(&flexbuffers::Builder::StartMap))
        .function("startMap",
            optional_override(
                [](flexbuffers::Builder& b, const std::string& s, bool d) { return b.StartMap(s.c_str()); }))
        .function("endMap", &flexbuffers::Builder::EndMap);
}