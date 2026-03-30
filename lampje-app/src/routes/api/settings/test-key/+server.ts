import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import { db } from '$lib/server/db';
import { userSettings } from '$lib/server/schema';
import { eq } from 'drizzle-orm';
import { trackEvent } from '$lib/server/posthog';

export const POST: RequestHandler = async ({ locals }) => {
	const session = await locals.auth();
	if (!session?.user?.id) {
		return json({ error: 'Unauthorized' }, { status: 401 });
	}

	const [settings] = await db
		.select()
		.from(userSettings)
		.where(eq(userSettings.userId, session.user.id))
		.limit(1);

	if (!settings?.geminiApiKey) {
		return json({ error: 'No API key configured' }, { status: 400 });
	}

	try {
		const response = await fetch(
			`https://generativelanguage.googleapis.com/v1beta/models?key=${settings.geminiApiKey}`
		);

		const valid = response.ok;
		const newStatus = valid ? 'valid' : 'invalid';

		await db
			.update(userSettings)
			.set({ geminiApiKeyStatus: newStatus, updatedAt: new Date() })
			.where(eq(userSettings.userId, session.user.id));

		trackEvent(session.user.id, 'api_key_tested', { valid });

		return json({ valid });
	} catch {
		await db
			.update(userSettings)
			.set({ geminiApiKeyStatus: 'invalid', updatedAt: new Date() })
			.where(eq(userSettings.userId, session.user.id));

		return json({ valid: false });
	}
};
