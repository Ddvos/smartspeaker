import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { devices, deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';

export const PUT: RequestHandler = async ({ params, request, locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	// Verify ownership
	const [device] = await db
		.select()
		.from(devices)
		.where(eq(devices.id, params.id))
		.limit(1);

	if (!device) {
		return json({ error: 'Device not found' }, { status: 404 });
	}
	if (device.userId !== session.user.id) {
		return json({ error: 'Forbidden' }, { status: 403 });
	}

	const body = await request.json();

	// Only allow known config fields
	const allowedFields = [
		'ledColors',
		'micSensitivity',
		'speakerVolume',
		'displayBrightness',
		'voice',
		'systemPrompt',
		'geminiModel',
		'sttProvider',
		'ttsProvider'
	] as const;

	const updates: Record<string, unknown> = { updatedAt: new Date() };
	for (const field of allowedFields) {
		if (field in body) {
			updates[field] = body[field];
		}
	}

	const [updated] = await db
		.update(deviceConfigs)
		.set(updates)
		.where(eq(deviceConfigs.deviceId, params.id))
		.returning();

	if (!updated) {
		return json({ error: 'Config not found' }, { status: 404 });
	}

	return json({
		ledColors: updated.ledColors,
		micSensitivity: updated.micSensitivity,
		speakerVolume: updated.speakerVolume,
		displayBrightness: updated.displayBrightness,
		voice: updated.voice,
		systemPrompt: updated.systemPrompt,
		geminiModel: updated.geminiModel,
		sttProvider: updated.sttProvider,
		ttsProvider: updated.ttsProvider
	});
};
