import type { PageServerLoad } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { formatDeviceRow } from '$lib/server/device-utils';

export const load: PageServerLoad = async (event) => {
	const session = await event.locals.auth();

	const rows = await db
		.select()
		.from(devices)
		.leftJoin(deviceConfigs, eq(deviceConfigs.deviceId, devices.id))
		.where(eq(devices.userId, session!.user!.id!))
		.limit(1);

	const primaryDevice =
		rows.length > 0
			? formatDeviceRow(
					rows[0].device,
					rows[0].device_config ?? {
						ledColors: {},
						micSensitivity: 70,
						speakerVolume: 65,
						displayBrightness: 80,
						voice: 'puck',
						sttProvider: 'whisper',
						ttsProvider: 'elevenlabs'
					}
				)
			: null;

	return { primaryDevice };
};
