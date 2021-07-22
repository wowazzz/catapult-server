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

#include "Observers.h"
#include "catapult/config/CatapultDataDirectory.h"
#include "catapult/io/FileQueue.h"
#include "catapult/io/PodIoUtils.h"
#include "priceUtil.h"

namespace catapult { namespace observers {

	namespace {
		using Notification = model::PriceMessageNotification;
		const std::array<uint8_t, 32> byteArray = {
			83, 100, 169, 75, 106, 36, 168, 7, 123, 184, 234, 67, 250, 158,
			178, 4, 126, 246, 156, 245, 68, 36, 169, 224, 201, 65, 226, 192,
			189, 224, 218, 253
		};
	}

	DEFINE_OBSERVER(PriceMessage, Notification, [](
		const Notification& notification,
		const ObserverContext& context) {

		Key publisher = Key(byteArray);
		
		if (notification.SenderPublicKey == publisher) {
			catapult::plugins::processPriceTransaction(notification.blockHeight, notification.lowPrice,
				notification.highPrice, context.Mode == NotifyMode::Rollback);
		}
	})
}}
