import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { deviceConfigs, userSettings } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { authenticateDevice } from '$lib/server/device-auth';
import { GEMINI_API_KEY } from '$env/static/private';
import { trackEvent } from '$lib/server/posthog';

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

	// Try to get user's own API key first
	let apiKey: string | null = null;

	const [settings] = await db
		.select()
		.from(userSettings)
		.where(eq(userSettings.userId, device.userId))
		.limit(1);

	if (settings?.geminiApiKey && settings.geminiApiKeyStatus === 'valid') {
		apiKey = settings.geminiApiKey;
	}

	// Fall back to global key
	if (!apiKey) {
		apiKey = GEMINI_API_KEY || null;
	}

	if (!apiKey) {
		return json({ error: 'No Gemini API key configured' }, { status: 500 });
	}

	// Get device config for voice/prompt settings
	const [config] = await db
		.select()
		.from(deviceConfigs)
		.where(eq(deviceConfigs.deviceId, params.id))
		.limit(1);

	const model = config?.geminiModel ?? 'gemini-2.0-flash-live';
	const voice = config?.voice ?? 'puck';
	const systemPrompt = config?.systemPrompt ?? DEFAULT_SYSTEM_PROMPT;

	trackEvent(device.userId, 'gemini_token_issued', { model });

	return json({
		token: apiKey,
		expiresAt: new Date(Date.now() + 30 * 60 * 1000).toISOString(),
		model,
		voice,
		systemPrompt,
		speakerVolume: config?.speakerVolume ?? 65,
		micSensitivity: config?.micSensitivity ?? 70
	});
};
