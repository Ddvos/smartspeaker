import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devicePairingCodes } from '$lib/server/schema';
import { eq, and } from 'drizzle-orm';
import { authenticateDevice, generatePairingCode } from '$lib/server/device-auth';

export const POST: RequestHandler = async ({ params, request }) => {
	const device = await authenticateDevice(request);
	if (!device || device.id !== params.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	// Invalidate all existing unused codes for this device
	await db
		.update(devicePairingCodes)
		.set({ used: true })
		.where(
			and(
				eq(devicePairingCodes.deviceId, params.id),
				eq(devicePairingCodes.used, false)
			)
		);

	// Generate new code
	const code = generatePairingCode();
	const expiresAt = new Date(Date.now() + 10 * 60 * 1000);

	await db.insert(devicePairingCodes).values({
		deviceId: params.id,
		code,
		expiresAt
	});

	return json({
		code,
		expiresAt: expiresAt.toISOString()
	});
};
