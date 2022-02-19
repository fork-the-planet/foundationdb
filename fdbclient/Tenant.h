/*
 * Tenant.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FDBCLIENT_TENANT_H
#define FDBCLIENT_TENANT_H
#pragma once

#include "fdbclient/FDBTypes.h"
#include "fdbclient/VersionedMap.h"
#include "flow/flat_buffers.h"

typedef StringRef TenantNameRef;
typedef Standalone<TenantNameRef> TenantName;

struct TenantMapEntry {
	constexpr static FileIdentifier file_identifier = 12247338;

private:
	static Key idToPrefix(int64_t id) {
		int64_t swapped = bigEndian64(id);
		return StringRef(reinterpret_cast<const uint8_t*>(&swapped), 8);
	}

public:
	int64_t id;
	Key prefix;

	TenantMapEntry() : id(-1) {}
	TenantMapEntry(int64_t id) : id(id), prefix(idToPrefix(id)) { ASSERT(id >= 0); }

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar, id);
		if (ar.isDeserializing) {
			prefix = idToPrefix(id);
		}
	}
};

typedef VersionedMap<TenantName, TenantMapEntry> TenantMap;
typedef VersionedMap<KeyRef, TenantNameRef> TenantPrefixIndex;

#endif