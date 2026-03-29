import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { authenticateDevice } from '$lib/server/device-auth';

export const POST: RequestHandler = async ({ params, request }) => {
	const device = await authenticateDevice(request);
	if (!device || device.id !== params.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const body = await request.json();

	await db
		.update(devices)
		.set({
			lastHeartbeat: new Date(),
			firmwareVersion: body.firmwareVersion ?? device.firmwareVersion,
			wifiRssi: body.wifiRssi ?? device.wifiRssi,
			ipAddress: body.ipAddress ?? device.ipAddress,
			updatedAt: new Date()
		})
		.where(eq(devices.id, params.id));

	// Return configUpdatedAt so device can decide whether to re-fetch
	const [config] = await db
		.select({ updatedAt: deviceConfigs.updatedAt })
		.from(deviceConfigs)
		.where(eq(deviceConfigs.deviceId, params.id))
		.limit(1);

	return json({
		configUpdatedAt: config?.updatedAt?.toISOString() ?? null
	});
};
