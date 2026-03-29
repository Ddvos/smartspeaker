import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs, devicePairingCodes } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { generateDeviceToken, generatePairingCode } from '$lib/server/device-auth';

export const POST: RequestHandler = async ({ request }) => {
	const body = await request.json();
	const { chipId, firmwareVersion } = body;

	if (!chipId || typeof chipId !== 'string') {
		return json({ error: 'chipId is required' }, { status: 400 });
	}

	// Idempotent: if device already registered, return existing token
	const [existing] = await db
		.select()
		.from(devices)
		.where(eq(devices.chipId, chipId))
		.limit(1);

	if (existing) {
		// Get or create a fresh pairing code
		const code = generatePairingCode();
		const expiresAt = new Date(Date.now() + 10 * 60 * 1000);

		await db.insert(devicePairingCodes).values({
			deviceId: existing.id,
			code,
			expiresAt
		});

		return json({
			deviceId: existing.id,
			deviceToken: existing.deviceToken,
			pairingCode: code,
			pairingExpiresAt: expiresAt.toISOString()
		});
	}

	// New device registration
	const deviceToken = generateDeviceToken();

	const [device] = await db
		.insert(devices)
		.values({
			chipId,
			firmwareVersion: firmwareVersion ?? null,
			deviceToken
		})
		.returning();

	// Create default config
	await db.insert(deviceConfigs).values({
		deviceId: device.id
	});

	// Create pairing code
	const code = generatePairingCode();
	const expiresAt = new Date(Date.now() + 10 * 60 * 1000);

	await db.insert(devicePairingCodes).values({
		deviceId: device.id,
		code,
		expiresAt
	});

	return json({
		deviceId: device.id,
		deviceToken: device.deviceToken,
		pairingCode: code,
		pairingExpiresAt: expiresAt.toISOString()
	});
};
