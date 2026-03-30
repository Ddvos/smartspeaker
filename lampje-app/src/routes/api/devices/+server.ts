import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { formatDeviceRow } from '$lib/server/device-utils';

const DEFAULT_CONFIG = {
	ledColors: {},
	micSensitivity: 70,
	speakerVolume: 65,
	displayBrightness: 80,
	voice: 'puck',
	systemPrompt: null,
	geminiModel: 'gemini-2.0-flash-live',
	sttProvider: 'whisper',
	ttsProvider: 'elevenlabs'
} as const;

export const GET: RequestHandler = async ({ locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const rows = await db
		.select()
		.from(devices)
		.leftJoin(deviceConfigs, eq(deviceConfigs.deviceId, devices.id))
		.where(eq(devices.userId, session.user.id));

	const result = rows.map((row) =>
		formatDeviceRow(row.device, row.device_config ?? DEFAULT_CONFIG)
	);

	return json(result);
};
