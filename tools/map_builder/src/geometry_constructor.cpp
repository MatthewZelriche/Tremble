#include "geometry_constructor.hpp"

using namespace TR;

GeometryConstructor::GeometryConstructor(const MapFileParser &parsedMap) {}

SerializableData &&GeometryConstructor::GetBuiltData() { return std::move(mData); }