/**
*** Copyright (c) 2016-2019, Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp.
*** Copyright (c) 2020-present, Jaguar0625, gimre, BloodyRookie.
*** All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "PricePlugin.h"
#include "PriceTransactionPlugin.h"
#include "src/config/PriceConfiguration.h"
#include "src/observers/Observers.h"
#include "src/validators/Validators.h"
#include "catapult/config/CatapultDataDirectory.h"
#include "catapult/config/CatapultKeys.h"
#include "catapult/crypto/OpensslKeyUtils.h"
#include "catapult/model/Address.h"
#include "catapult/plugins/PluginManager.h"

namespace catapult { namespace plugins {

	void RegisterPriceSubsystem(PluginManager& manager) {
		manager.addTransactionSupport(CreatePriceTransactionPlugin());

		auto config = model::LoadPluginConfiguration<config::PriceConfiguration>(manager.config(), "catapult.plugins.price");
		manager.addStatelessValidatorHook([config](auto& builder) {
			builder.add(validators::CreatePriceMessageValidator(config.MaxMessageSize));
		});

		if (!manager.userConfig().EnableDelegatedHarvestersAutoDetection)
			return;

		//auto encryptionPrivateKeyPemFilename = config::GetNodePrivateKeyPemFilename(manager.userConfig().CertificateDirectory);
		//auto encryptionPublicKey = crypto::ReadPublicKeyFromPrivateKeyPemFile(encryptionPrivateKeyPemFilename);
		//auto recipient = model::PublicKeyToAddress(encryptionPublicKey, manager.config().Network.Identifier);
		auto dataDirectory = config::CatapultDataDirectory(manager.userConfig().DataDirectory);
		manager.addObserverHook([dataDirectory](auto& builder) {
			builder.add(observers::CreatePriceMessageObserver(0xE201735761802AFE, dataDirectory.dir("transfer_message")));
		});
	}
}}

extern "C" PLUGIN_API
void RegisterSubsystem(catapult::plugins::PluginManager& manager) {
	catapult::plugins::RegisterPriceSubsystem(manager);
}
