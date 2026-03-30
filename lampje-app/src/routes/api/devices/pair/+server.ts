import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs, devicePairingCodes, userSettings } from '$lib/server/schema';
import { eq, and, gt } from 'drizzle-orm';
import { formatDeviceRow } from '$lib/server/device-utils';
import { getSystemPrompt } from '$lib/server/prompt-utils';
import { trackEvent } from '$lib/server/posthog';

export const POST: RequestHandler = async ({ request, locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const body = await request.json();
	const { code } = body;

	if (!code || typeof code !== 'string') {
		return json({ error: 'Pairing code is required' }, { status: 400 });
	}

	// Find valid, unused pairing code
	const [pairingCode] = await db
		.select()
		.from(devicePairingCodes)
		.where(
			and(
				eq(devicePairingCodes.code, code),
				eq(devicePairingCodes.used, false),
				gt(devicePairingCodes.expiresAt, new Date())
			)
		)
		.limit(1);

	if (!pairingCode) {
		return json({ error: 'Ongeldige of verlopen code' }, { status: 400 });
	}

	// Check if device is already paired to another user
	const [device] = await db
		.select()
		.from(devices)
		.where(eq(devices.id, pairingCode.deviceId))
		.limit(1);

	if (!device) {
		return json({ error: 'Device not found' }, { status: 404 });
	}

	if (device.userId && device.userId !== session.user.id) {
		return json({ error: 'Dit apparaat is al gekoppeld aan een ander account' }, { status: 409 });
	}

	// Mark code as used
	await db
		.update(devicePairingCodes)
		.set({ used: true })
		.where(eq(devicePairingCodes.id, pairingCode.id));

	// Link device to user
	await db
		.update(devices)
		.set({ userId: session.user.id, updatedAt: new Date() })
		.where(eq(devices.id, device.id));

	trackEvent(session.user.id, 'device_paired', { device_id: device.id });

	// Apply user defaults to device config
	const [settings] = await db
		.select()
		.from(userSettings)
		.where(eq(userSettings.userId, session.user.id))
		.limit(1);

	if (settings) {
		await db
			.update(deviceConfigs)
			.set({
				geminiModel: settings.defaultModel,
				voice: settings.defaultVoice,
				systemPrompt: getSystemPrompt(settings.defaultSystemPrompt),
				updatedAt: new Date()
			})
			.where(eq(deviceConfigs.deviceId, device.id));
	}

	// Get config
	const [config] = await db
		.select()
		.from(deviceConfigs)
		.where(eq(deviceConfigs.deviceId, device.id))
		.limit(1);

	return json(
		formatDeviceRow(
			{ ...device, userId: session.user.id },
			config
		)
	);
};
