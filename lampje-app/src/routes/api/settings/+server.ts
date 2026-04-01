import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { userSettings } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { voices } from '$lib/data/settings-constants';
import { log } from '$lib/server/logger';

const ALLOWED_MODELS = ['gemini-2.0-flash-live'];
const ALLOWED_VOICES = voices.map((v) => v.id);

function maskApiKey(key: string): string {
	if (key.length <= 8) return '••••••••';
	return key.slice(0, 4) + '••••' + key.slice(-4);
}

function formatSettings(row: typeof userSettings.$inferSelect) {
	return {
		hasGeminiApiKey: !!row.geminiApiKey,
		geminiApiKeyMasked: row.geminiApiKey ? maskApiKey(row.geminiApiKey) : '',
		geminiApiKeyStatus: row.geminiApiKeyStatus,
		defaultModel: row.defaultModel,
		defaultVoice: row.defaultVoice,
		defaultSystemPrompt: row.defaultSystemPrompt ?? ''
	};
}

async function getOrCreateSettings(userId: string) {
	const [existing] = await db
		.select()
		.from(userSettings)
		.where(eq(userSettings.userId, userId))
		.limit(1);

	if (existing) return existing;

	log('info', 'Created default settings for new user', { userId });

	const [created] = await db.insert(userSettings).values({ userId }).returning();

	return created;
}

export const GET: RequestHandler = async ({ locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const settings = await getOrCreateSettings(session.user.id);
	return json(formatSettings(settings));
};

export const PUT: RequestHandler = async ({ request, locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const body = await request.json();
	const updates: Record<string, unknown> = { updatedAt: new Date() };

	if ('geminiApiKey' in body) {
		if (typeof body.geminiApiKey === 'string') {
			if (body.geminiApiKey === '') {
				updates.geminiApiKey = null;
				updates.geminiApiKeyStatus = 'untested';
				log('info', 'Gemini API key removed', { userId: session.user.id });
			} else {
				updates.geminiApiKey = body.geminiApiKey;
				updates.geminiApiKeyStatus = 'untested';
				log('info', 'Gemini API key updated', { userId: session.user.id });
			}
		}
	}

	if ('defaultModel' in body && ALLOWED_MODELS.includes(body.defaultModel)) {
		updates.defaultModel = body.defaultModel;
	}

	if ('defaultVoice' in body && ALLOWED_VOICES.includes(body.defaultVoice)) {
		updates.defaultVoice = body.defaultVoice;
	}

	if ('defaultSystemPrompt' in body && typeof body.defaultSystemPrompt === 'string') {
		updates.defaultSystemPrompt = body.defaultSystemPrompt || null;
	}

	// Ensure row exists
	const settings = await getOrCreateSettings(session.user.id);

	const [updated] = await db
		.update(userSettings)
		.set(updates)
		.where(eq(userSettings.userId, session.user.id))
		.returning();

	const fieldsUpdated = Object.keys(updates).filter(
		(k) => k !== 'updatedAt' && k !== 'geminiApiKey'
	);
	if (fieldsUpdated.length > 0) {
		log('info', 'User settings updated', {
			userId: session.user.id,
			fieldsUpdated: fieldsUpdated.join(', ')
		});
	}

	return json(formatSettings(updated ?? settings));
};
