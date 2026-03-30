import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { authenticateDevice } from '$lib/server/device-auth';
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

export const GET: RequestHandler = async ({ params, request, locals }) => {
	// Try session auth first, then device token
	const session = await locals.auth();
	const deviceAuth = !session?.user?.id ? await authenticateDevice(request) : null;

	if (!session?.user?.id && !deviceAuth) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const [row] = await db
		.select()
		.from(devices)
		.leftJoin(deviceConfigs, eq(deviceConfigs.deviceId, devices.id))
		.where(eq(devices.id, params.id))
		.limit(1);

	if (!row) {
		return json({ error: 'Device not found' }, { status: 404 });
	}

	// Authorization: session user must own the device, or device token must match
	if (session?.user?.id && row.device.userId !== session.user.id) {
		return json({ error: 'Forbidden' }, { status: 403 });
	}
	if (deviceAuth && deviceAuth.id !== params.id) {
		return json({ error: 'Forbidden' }, { status: 403 });
	}

	return json(
		formatDeviceRow(row.device, row.device_config ?? DEFAULT_CONFIG)
	);
};
