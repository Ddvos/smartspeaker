import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { deviceConfigs } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { authenticateDevice } from '$lib/server/device-auth';
import { GEMINI_API_KEY } from '$env/static/private';

const DEFAULT_SYSTEM_PROMPT =
	'Je bent Lampje, een vriendelijke en behulpzame slimme lamp. Je spreekt Nederlands en helpt de gebruiker met vragen, taken en gezellig kletsen. Houd je antwoorden kort en natuurlijk.';

export const POST: RequestHandler = async ({ params, request }) => {
	const device = await authenticateDevice(request);
	if (!device || device.id !== params.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	if (!device.userId) {
		return json({ error: 'Device not paired' }, { status: 403 });
	}

	if (!GEMINI_API_KEY) {
		return json({ error: 'Gemini API key not configured' }, { status: 500 });
	}

	// Get device config for voice/prompt settings
	const [config] = await db
		.select()
		.from(deviceConfigs)
		.where(eq(deviceConfigs.deviceId, params.id))
		.limit(1);

	const model = config?.geminiModel ?? 'gemini-3.1-flash-live-preview';
	const voice = config?.voice ?? 'puck';
	const systemPrompt = config?.systemPrompt ?? DEFAULT_SYSTEM_PROMPT;

	// Pass API key directly to device (over TLS, authenticated by device token).
	// TODO: Switch to ephemeral tokens for production.
	return json({
		token: GEMINI_API_KEY,
		expiresAt: new Date(Date.now() + 30 * 60 * 1000).toISOString(),
		model,
		voice,
		systemPrompt,
		speakerVolume: config?.speakerVolume ?? 65,
		micSensitivity: config?.micSensitivity ?? 70
	});
};
